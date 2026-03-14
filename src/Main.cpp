//beginning of Main.cpp

#include "peripherals/Controller.h"
#include "peripherals/Display.h"
#include "peripherals/Drum.h"
#include "peripherals/StatusLed.h"
#include "usb/device/hid/ps4_auth.h"
#include "usb/device_driver.h"
#include "utils/InputReport.h"
#include "utils/InputState.h"
#include "utils/Menu.h"
#include "utils/PS4AuthProvider.h"
#include "utils/SettingsStore.h"

#include "GlobalConfiguration.h"
#include "PS4AuthConfiguration.h"

#include "pico/bootrom.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/util/queue.h"

#include <cstdio>
#include "tusb.h"

using namespace OuchiTaiko;

namespace {

queue_t control_queue;
queue_t menu_display_queue;
queue_t drum_input_queue;
queue_t controller_input_queue;

queue_t auth_challenge_queue;
queue_t auth_signed_challenge_queue;

// Queue for sending drum pointer to core 1
queue_t drum_reference_queue;

// NEW: Queue for sending fresh threshold data to Display
queue_t thresholds_queue;

enum class ControlCommand : uint8_t {
    SetUsbMode,
    SetPlayerLed,
    SetLedBrightness,
    SetLedEnablePlayerColor,
    EnterMenu,
    ExitMenu
};

struct ControlMessage {
    ControlCommand command;
    union {
        usb_mode_t usb_mode;
        usb_player_led_t player_led;
        uint8_t led_brightness;
        bool led_enable_player_color;
        char mode_name[32];  // For Big Hit Mode confirmation
    } data;
};

void core1_task() {
    multicore_lockout_victim_init();

    gpio_set_function(Config::Default::i2c_config.sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(Config::Default::i2c_config.scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(Config::Default::i2c_config.sda_pin);
    gpio_pull_up(Config::Default::i2c_config.scl_pin);
    i2c_init(Config::Default::i2c_config.block, Config::Default::i2c_config.speed_hz);

    Peripherals::Controller controller(Config::Default::controller_config);
    Peripherals::StatusLed led(Config::Default::led_config);
    Peripherals::Display display(Config::Default::display_config);

    Utils::PS4AuthProvider ps4authprovider;
    std::array<uint8_t, Utils::PS4AuthProvider::SIGNATURE_LENGTH> auth_challenge{};

    Utils::InputState input_state;
    Utils::Menu::State menu_display_msg{};
    ControlMessage control_msg{};

    // Receive drum pointer from core 0
    Peripherals::Drum *drum_ptr = nullptr;
    queue_remove_blocking(&drum_reference_queue, &drum_ptr);
    display.setDrumReference(drum_ptr);

    // NEW: Storage for fresh threshold data
    Peripherals::Drum::Config::Thresholds fresh_thresholds{};

    while (true) {
        controller.updateInputState(input_state);

        // Queue controller samples so short taps are not overwritten before Core 0 sees them.
        if (!queue_try_add(&controller_input_queue, &input_state.controller)) {
            Utils::InputState::Controller dropped{};
            queue_try_remove(&controller_input_queue, &dropped);
            queue_try_add(&controller_input_queue, &input_state.controller);
        }
        
        queue_try_remove(&drum_input_queue, &input_state.drum);

        // NEW: Receive fresh threshold data from Core 0
        if (queue_try_remove(&thresholds_queue, &fresh_thresholds)) {
            display.setCurrentThresholds(fresh_thresholds);
        }

        if (queue_try_remove(&control_queue, &control_msg)) {
            switch (control_msg.command) {
            case ControlCommand::SetUsbMode:
                display.setUsbMode(control_msg.data.usb_mode);
                break;
            case ControlCommand::SetPlayerLed:
                switch (control_msg.data.player_led.type) {
                case USB_PLAYER_LED_ID:
                    display.setPlayerId(control_msg.data.player_led.id);
                    break;
                case USB_PLAYER_LED_COLOR:
                    led.setPlayerColor({.r = control_msg.data.player_led.red,
                                        .g = control_msg.data.player_led.green,
                                        .b = control_msg.data.player_led.blue});
                }
                break;
            case ControlCommand::SetLedBrightness:
                led.setBrightness(control_msg.data.led_brightness);
                break;
            case ControlCommand::SetLedEnablePlayerColor:
                led.setEnablePlayerColor(control_msg.data.led_enable_player_color);
                break;
            case ControlCommand::EnterMenu:
                display.showMenu();
                break;
            case ControlCommand::ExitMenu:
                display.showIdle();
                break;
            }
        }
        if (queue_try_remove(&menu_display_queue, &menu_display_msg)) {
            display.setMenuState(menu_display_msg);
        }
        
        // Drive calibration wizard display from TantrumState mode.
        // Fail-safe: force the wizard screen state every frame while active so we never
        // remain on Idle/Menu due to a missed mode-change edge.
        {
            using Mode = Peripherals::Drum::TantrumState::Mode;
            if (drum_ptr && drum_ptr->isTantrumActive()) {
                const auto& ts = drum_ptr->getTantrumState();
                switch (ts.current_mode) {
                case Mode::Welcome:    display.showTantrumWelcome();    break;
                case Mode::PadNormal:
                case Mode::PadHard:    display.showTantrumPadHitting(); break;
                case Mode::PhaseTransition: display.showTantrumPhaseTransition(); break;
                case Mode::PadRoll:    display.showTantrumPadRoll();    break;
                case Mode::PadDone:    display.showTantrumPadDone();    break;
                case Mode::Overview:   display.showTantrumOverview();   break;
                case Mode::Saving:     display.showTantrumSaving();     break;
                case Mode::Complete:   display.showTantrumComplete();   break;
                case Mode::Error:      display.showTantrumError();      break;
                default: break;
                }
            }
        }

        if (queue_try_remove(&auth_challenge_queue, auth_challenge.data())) {
            const auto signed_challenge = ps4authprovider.sign(auth_challenge);
            queue_try_remove(&auth_signed_challenge_queue, nullptr);
            queue_try_add(&auth_signed_challenge_queue, &signed_challenge);
        }

        led.setInputState(input_state);
        display.setInputState(input_state);

        led.update();
        display.update();
    }
}

} // namespace

int main() {
    queue_init(&control_queue, sizeof(ControlMessage), 1);
    queue_init(&menu_display_queue, sizeof(Utils::Menu::State), 1);
    queue_init(&drum_input_queue, sizeof(Utils::InputState::Drum), 1);
    queue_init(&controller_input_queue, sizeof(Utils::InputState::Controller), 8);
    queue_init(&auth_challenge_queue, sizeof(std::array<uint8_t, Utils::PS4AuthProvider::SIGNATURE_LENGTH>), 1);
    queue_init(&auth_signed_challenge_queue, sizeof(std::array<uint8_t, Utils::PS4AuthProvider::SIGNATURE_LENGTH>), 1);
    queue_init(&drum_reference_queue, sizeof(Peripherals::Drum*), 1);
    
    // NEW: Initialize queue for fresh threshold data
    queue_init(&thresholds_queue, sizeof(Peripherals::Drum::Config::Thresholds), 1);

    stdio_init_all();

    Peripherals::Drum drum(Config::Default::drum_config);

    Utils::InputReport input_report;
    Utils::InputState input_state;
    Utils::InputState::Controller latest_controller_state{};
    
    // NEW: Hotkey state variables are now local to main, allowing manual reset
    uint32_t select_menu_hold_start = 0;
    bool select_menu_was_held = false;
    uint8_t select_menu_press_count = 0;

    // NEW: Function to manually reset the hotkey state
    const auto resetHotkeyState = [&]() {
        select_menu_hold_start = 0;
        select_menu_was_held = false;
        select_menu_press_count = 0;
    };
    
    // Hold SELECT for menu (1 second hold) with debouncing
    const auto checkHoldSelect = [&input_state, &select_menu_hold_start, &select_menu_was_held, &select_menu_press_count]() {
        static const uint32_t HOLD_DURATION_MS = 1000;
        static const uint8_t DEBOUNCE_THRESHOLD = 3;  // Must see pressed 3 frames in a row to start

        bool select_pressed = input_state.controller.buttons.select;
        uint32_t current_time = to_ms_since_boot(get_absolute_time());

        if (select_pressed) {
            select_menu_press_count++;

            // Start timer once we've seen button pressed for multiple frames (debounced)
            if (select_menu_press_count >= DEBOUNCE_THRESHOLD && select_menu_hold_start == 0) {
                select_menu_hold_start = current_time;
                select_menu_was_held = false;
            }

            // Check if hold duration met
            if (select_menu_hold_start != 0 && !select_menu_was_held &&
                (current_time - select_menu_hold_start) >= HOLD_DURATION_MS) {
                select_menu_was_held = true;
                select_menu_press_count = 0;
                select_menu_hold_start = 0;
                return true;
            }
        } else {
            // Button released - reset everything
            select_menu_press_count = 0;
            select_menu_hold_start = 0;
            select_menu_was_held = false;
        }

        return false;
    };

    // Hold HOME for 5 seconds to reboot into bootloader (BOOTSEL) mode.
    // Saves wear on physical BOOTSEL button -- no need to hold while plugging in.
    const auto checkHoldHome = [&input_state]() {
        static uint32_t home_hold_start = 0;
        static bool was_held = false;
        static uint8_t press_count = 0;
        static const uint32_t HOLD_DURATION_MS = 5000;
        static const uint8_t DEBOUNCE_THRESHOLD = 3;

        bool home_pressed = input_state.controller.buttons.home;
        uint32_t current_time = to_ms_since_boot(get_absolute_time());

        if (home_pressed) {
            press_count++;
            if (press_count >= DEBOUNCE_THRESHOLD && home_hold_start == 0) {
                home_hold_start = current_time;
                was_held = false;
            }
            if (home_hold_start != 0 && !was_held && (current_time - home_hold_start) >= HOLD_DURATION_MS) {
                was_held = true;
                press_count = 0;
                home_hold_start = 0;
                return true;
            }
        } else {
            press_count = 0;
            home_hold_start = 0;
            was_held = false;
        }
        return false;
    };

    // Hold START for 2 seconds to reboot into bootloader (BOOTSEL) mode.
    // Same behaviour as hold HOME — goes through the BootselMsg countdown splash.
    const auto checkHoldStart = [&input_state]() {
        static uint32_t start_hold_start = 0;
        static bool was_held = false;
        static uint8_t press_count = 0;
        static const uint32_t HOLD_DURATION_MS = 2000;
        static const uint8_t DEBOUNCE_THRESHOLD = 3;

        bool start_pressed = input_state.controller.buttons.start;
        uint32_t current_time = to_ms_since_boot(get_absolute_time());

        if (start_pressed) {
            press_count++;
            if (press_count >= DEBOUNCE_THRESHOLD && start_hold_start == 0) {
                start_hold_start = current_time;
                was_held = false;
            }
            if (start_hold_start != 0 && !was_held && (current_time - start_hold_start) >= HOLD_DURATION_MS) {
                was_held = true;
                press_count = 0;
                start_hold_start = 0;
                return true;
            }
        } else {
            press_count = 0;
            start_hold_start = 0;
            was_held = false;
        }
        return false;
    };

    auto settings_store = std::make_shared<Utils::SettingsStore>();
    const auto mode = settings_store->getUsbMode();
    const auto readSettings = [&]() {
        const auto sendCtrlMessage = [&](const ControlMessage &msg) { queue_add_blocking(&control_queue, &msg); };

        sendCtrlMessage({.command = ControlCommand::SetUsbMode, .data = {.usb_mode = mode}});
        sendCtrlMessage({.command = ControlCommand::SetLedBrightness,
                         .data = {.led_brightness = settings_store->getLedBrightness()}});
        sendCtrlMessage({.command = ControlCommand::SetLedEnablePlayerColor,
                         .data = {.led_enable_player_color = settings_store->getLedEnablePlayerColor()}});

        drum.setDebounceDelay(settings_store->getDebounceDelay());
        drum.setTriggerThresholds(settings_store->getTriggerThresholds());
    };

    Utils::Menu menu(settings_store);

    std::array<uint8_t, Utils::PS4AuthProvider::SIGNATURE_LENGTH> auth_challenge_response{};
    if (Config::PS4Auth::config.enabled) {
        ps4_auth_init(Config::PS4Auth::config.key_pem.c_str(), Config::PS4Auth::config.key_pem.size() + 1,
                      Config::PS4Auth::config.serial.data(), Config::PS4Auth::config.signature.data(),
                      [](const uint8_t *challenge) { queue_try_add(&auth_challenge_queue, challenge); });
    }

    // Send drum pointer to core 1 before launching
    Peripherals::Drum *drum_ptr = &drum;
    queue_add_blocking(&drum_reference_queue, &drum_ptr);

    multicore_launch_core1(core1_task);

    usbd_driver_init(mode);
    usbd_driver_set_player_led_cb([](usb_player_led_t player_led) {
        const auto ctrl_message =
            ControlMessage{.command = ControlCommand::SetPlayerLed, .data = {.player_led = player_led}};
        queue_try_add(&control_queue, &ctrl_message);
    });

    readSettings();

    uint32_t ps4_auth_start_time = 0;
    
    // Tantrum calibration state tracking for auto-save
    bool last_analysis_active = false;
    
    // Tantrum results display timing
    bool results_displaying = false;

    // Prevent the same A press used to select "Start Wizard" from immediately
    // advancing Welcome -> PadNormal in the first active frame.
    bool tantrum_wait_for_confirm_release = false;

    while (true) {
        drum.updateInputState(input_state, mode);

        // Drain queue to the most recent controller sample each frame.
        // This avoids stale "button still down" states that can block edge detection.
        Utils::InputState::Controller sampled_controller{};
        while (queue_try_remove(&controller_input_queue, &sampled_controller)) {
            latest_controller_state = sampled_controller;
        }
        input_state.controller = latest_controller_state;

        // NEW: Send fresh threshold data to Display on every frame when menu is active
        // This ensures Display always shows current values from SettingsStore
        if (menu.active()) {
            auto fresh_thresholds = settings_store->getTriggerThresholds();
            queue_try_remove(&thresholds_queue, nullptr); // Remove stale data
            queue_try_add(&thresholds_queue, &fresh_thresholds);
        }

        // Drive calibration wizard A/B buttons in main loop (Core 0)
        const auto& tantrum_state = drum.getTantrumState();
        bool tantrum_active = tantrum_state.isActive();
        using TMode = Peripherals::Drum::TantrumState::Mode;

        if (tantrum_active) {
            // Debounce: track A button edge
            static bool last_east = false;
            static bool last_south = false;
            bool east_pressed  = input_state.controller.buttons.east  && !last_east;
            bool south_pressed = input_state.controller.buttons.south && !last_south;
            last_east  = input_state.controller.buttons.east;
            last_south = input_state.controller.buttons.south;

            if (tantrum_wait_for_confirm_release) {
                if (!input_state.controller.buttons.east) {
                    tantrum_wait_for_confirm_release = false;
                }
                east_pressed = false;
            }

            // A button: advance wizard
            if (east_pressed) {
                drum.advanceCalibWizard();
            }

            // B button: cancel at any point
            if (south_pressed) {
                drum.cancelTaikoTantrum();
                last_analysis_active = false;
                results_displaying = false;
                ControlMessage ctrl_message = {.command = ControlCommand::ExitMenu, .data = {}};
                queue_add_blocking(&control_queue, &ctrl_message);
            }
        }

        // Detect wizard completion: Saving mode triggers apply+save, then return to menu
        bool just_entered_saving = last_analysis_active &&
                                   tantrum_state.current_mode == TMode::Saving &&
                                   !results_displaying;
        if (just_entered_saving) {
            drum.applyTantrumRecommendations();
            settings_store->setTriggerThresholds(drum.getCurrentThresholds());
            settings_store->setLastTantrumReport(drum.getLastTantrumReportVersion(), drum.getLastTantrumReport());
            settings_store->store();
            readSettings();
            results_displaying = true;
        }

        if (results_displaying) {
            // Wait for Complete state to finish displaying, then return to Drum Tuning menu
            if (!tantrum_active || tantrum_state.current_mode == TMode::Inactive) {  // NOLINT
                menu.activate();
                menu.goBackToParent();
                ControlMessage ctrl_message = {.command = ControlCommand::EnterMenu, .data = {}};
                queue_add_blocking(&control_queue, &ctrl_message);
                const auto display_msg = menu.getState();
                queue_add_blocking(&menu_display_queue, &display_msg);
                results_displaying = false;
            }
        }

        last_analysis_active = tantrum_active;

        const auto drum_message = input_state.drum;

        if ((mode == USB_MODE_PS4_TATACON || mode == USB_MODE_DUALSHOCK4) && !menu.active()) {
            const uint32_t now = to_ms_since_boot(get_absolute_time());
            
            if (ps4_auth_start_time == 0) {
                ps4_auth_start_time = now;
            }
            
            if ((now - ps4_auth_start_time) >= 420000) {
                ps4_auth_reset();
                ps4_auth_start_time = now;
            }
        } else {
            ps4_auth_start_time = 0;
        }

        // Track menu state BEFORE processing to detect transitions
        bool was_menu_active = menu.active();
        
        // HOLD HOME 5s to reboot into bootloader (BOOTSEL) mode
        // Goes through the same BootselMsg splash path as the menu so the user
        // sees the countdown screen instead of the display freezing on main.
        if (checkHoldHome() || checkHoldStart()) {
            menu.enterBootloaderSplash();
            ControlMessage ctrl_message = {.command = ControlCommand::EnterMenu, .data = {}};
            queue_add_blocking(&control_queue, &ctrl_message);
        }

        // HOLD SELECT to ENTER menu (not exit - use B button to exit)
        // Check BEFORE menu processing so we can detect the hold
        if (!menu.active() && checkHoldSelect()) {
            // Menu is closed - open it
            menu.activate();
            
            ControlMessage ctrl_message = {.command = ControlCommand::EnterMenu, .data = {}};
            queue_add_blocking(&control_queue, &ctrl_message);
        }

        if (menu.active()) {
            // Pass raw controller state to Menu.cpp
            // Menu.cpp handles all edge detection and hold-to-repeat logic internally
            menu.update(input_state.controller);
            
            // Check if Tantrum calibration start was requested
            if (menu.isTantrumStartRequested()) {
                // Start Tantrum calibration on Core 0
                drum.startTaikoTantrum();
                tantrum_wait_for_confirm_release = true;

                // Deactivate menu so it doesn't overwrite wizard screens
                menu.deactivate();

                // Tell display to exit menu -- wizard screens now driven by core1 state tracker
                ControlMessage ctrl_message = {.command = ControlCommand::ExitMenu, .data = {}};
                queue_add_blocking(&control_queue, &ctrl_message);

                // Core 1's display.update() will automatically show the Tantrum screens
                // based on the Drum's TantrumState (countdown -> recording -> results)
            }

            // Check menu.active() again BEFORE sending display state
            // If menu just closed during update(), don't send state
            if (menu.active()) {
                // Menu is still active - send display state
                const auto display_msg = menu.getState();
                queue_add_blocking(&menu_display_queue, &display_msg);
            } else {
                // Menu just closed - handle exit cleanup
                settings_store->store();

                ControlMessage ctrl_message = {.command = ControlCommand::ExitMenu, .data = {}};
                queue_add_blocking(&control_queue, &ctrl_message);
                
                // Apply settings changes NOW, when menu closes
                readSettings();
                
                // CRITICAL FIX FOR SECOND ENTRY FREEZE: Manually reset the hotkey state machine
                // This prevents the lingering 'was_held=true' flag from instantly re-activating 
                // the menu or causing the internal button logic to see a pre-held button.
                resetHotkeyState(); 
                
                // The menu's own deactivate() now handles m_buttons.reset()
            }
            
            // Clear local input state after menu processing.
            // Menu keeps its own edge state; this prevents carryover into gameplay/reporting path.
            input_state.releaseAll();
            // Restore persisted controller state after local clear so held buttons
            // remain stable across frames for edge detection.
            input_state.controller = latest_controller_state;
        }
        
        // Menu just closed during update() - no special handling needed
        if (was_menu_active && !menu.active()) {
            // The menu's deactivate() already handles cleanup
        }

        // ============================================================================
        // WEB TOOL SERIAL RECEIVE: Accept threshold packet from ouchitaiko-tool
        // Packet format: AA [donL hi] [donL lo] [donR hi] [donR lo] [kaL hi] [kaL lo] [kaR hi] [kaR lo] 55
        // Only active in Debug mode so it doesn't interfere with normal USB operation
        // ============================================================================
        if (mode == USB_MODE_DEBUG && tud_cdc_available()) {
            static uint8_t rx_buf[10];
            static uint8_t rx_pos = 0;

            uint8_t byte;
            tud_cdc_read(&byte, 1);

            // 0xBB = threshold query -- respond with current thresholds so web tool shows real values
            if (byte == 0xBB && rx_pos == 0) {
                const auto& t = drum.getCurrentThresholds();
                char buf[32];
                snprintf(buf, sizeof(buf), "THR:%u,%u,%u,%u\n",
                    t.don_left, t.don_right, t.ka_left, t.ka_right);
                tud_cdc_write(buf, strlen(buf));
                tud_cdc_write_flush();
            } else if (byte == 0xBC && rx_pos == 0) {
                const char* report = drum.getLastTantrumReport();
                if (report[0] == '\0') {
                    report = settings_store->getLastTantrumReport();
                }
                char buf[640];
                if (report[0] != '\0') {
                    snprintf(buf, sizeof(buf), "TLG:%s\n", report);
                } else {
                    snprintf(buf, sizeof(buf), "TLG:EMPTY\n");
                }
                tud_cdc_write(buf, strlen(buf));
                tud_cdc_write_flush();
            } else if (rx_pos == 0 && byte != 0xAA) {
                // Not a start byte -- ignore and stay ready
            } else {
                rx_buf[rx_pos++] = byte;

                if (rx_pos == 10) {
                    rx_pos = 0;
                    if (rx_buf[9] == 0x55) {
                        // Valid packet -- apply and save thresholds
                        Peripherals::Drum::Config::Thresholds t;
                        t.don_left  = (static_cast<uint16_t>(rx_buf[1]) << 8) | rx_buf[2];
                        t.don_right = (static_cast<uint16_t>(rx_buf[3]) << 8) | rx_buf[4];
                        t.ka_left   = (static_cast<uint16_t>(rx_buf[5]) << 8) | rx_buf[6];
                        t.ka_right  = (static_cast<uint16_t>(rx_buf[7]) << 8) | rx_buf[8];

                        drum.setTriggerThresholds(t);
                        settings_store->setTriggerThresholds(t);
                        settings_store->store();

                        // Send ACK back to web tool
                        const char* ack = "OK\n";
                        tud_cdc_write(ack, 3);
                        tud_cdc_write_flush();
                    }
                    // Bad end byte -- silently discard, rx_pos already reset
                }
            }
        }
        // ============================================================================
        // END WEB TOOL SERIAL RECEIVE
        // ============================================================================

        usbd_driver_send_report(input_report.getReport(input_state, mode));
        usbd_driver_task();

        queue_try_add(&drum_input_queue, &drum_message);

        if (queue_try_remove(&auth_signed_challenge_queue, auth_challenge_response.data())) {
            ps4_auth_set_signed_challenge(auth_challenge_response.data());
        }
    }

    return 0;
}

//End of Main.cpp
