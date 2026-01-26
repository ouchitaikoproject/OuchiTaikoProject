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

#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/util/queue.h"

#include <cstdio>

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

        // CRITICAL: Remove any stale data first, then add fresh data
        // This ensures Core 0 always gets the latest controller state
        queue_try_remove(&controller_input_queue, nullptr);
        queue_add_blocking(&controller_input_queue, &input_state.controller);
        
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
        
        // Handle Tantrum calibration display updates
        // Check if Tantrum is active and show appropriate screen
        static bool was_tantrum_active = false;
        static bool was_showing_instructions = false;
        
        if (drum_ptr && drum_ptr->isTantrumActive()) {
            const auto& ts = drum_ptr->getTantrumState();
            
            // Show instructions screen when in Instructions mode
            if (ts.current_mode == Peripherals::Drum::TantrumState::Mode::Instructions && !was_showing_instructions) {
                display.showTantrumInstructions();
                was_showing_instructions = true;
                was_tantrum_active = true;
            }
            // Show countdown when countdown starts
            else if (ts.current_mode == Peripherals::Drum::TantrumState::Mode::Countdown && was_showing_instructions) {
                display.showTantrumCountdown();
                was_showing_instructions = false;
            }
            // Show initial screen when Tantrum first becomes active (fallback)
            else if (!was_tantrum_active) {
                display.showTantrumInstructions();
                was_tantrum_active = true;
                was_showing_instructions = true;
            }

            // The draw functions in Display will auto-transition between states
            // based on the current TantrumState mode
        } else if (was_tantrum_active) {
            // Tantrum just finished - display will have auto-returned to menu
            was_tantrum_active = false;
            was_showing_instructions = false;
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
    queue_init(&controller_input_queue, sizeof(Utils::InputState::Controller), 1);
    queue_init(&auth_challenge_queue, sizeof(std::array<uint8_t, Utils::PS4AuthProvider::SIGNATURE_LENGTH>), 1);
    queue_init(&auth_signed_challenge_queue, sizeof(std::array<uint8_t, Utils::PS4AuthProvider::SIGNATURE_LENGTH>), 1);
    queue_init(&drum_reference_queue, sizeof(Peripherals::Drum*), 1);
    
    // NEW: Initialize queue for fresh threshold data
    queue_init(&thresholds_queue, sizeof(Peripherals::Drum::Config::Thresholds), 1);

    stdio_init_all();

    Peripherals::Drum drum(Config::Default::drum_config);

    Utils::InputReport input_report;
    Utils::InputState input_state;
    
    // NEW: Hotkey state variables are now local to main, allowing manual reset
    uint32_t select_menu_hold_start = 0;
    bool select_menu_was_held = false;

    // NEW: Function to manually reset the hotkey state
    const auto resetHotkeyState = [&]() {
        select_menu_hold_start = 0;
        select_menu_was_held = false;
    };
    
    // Hold SELECT for menu (1 second hold) with debouncing
    const auto checkHoldSelect = [&input_state]() {
        static uint32_t select_hold_start = 0;
        static bool was_held = false;
        static uint8_t press_count = 0;  // Debounce counter
        static const uint32_t HOLD_DURATION_MS = 1000;
        static const uint8_t DEBOUNCE_THRESHOLD = 3;  // Must see pressed 3 frames in a row to start

        bool select_pressed = input_state.controller.buttons.select;
        uint32_t current_time = to_ms_since_boot(get_absolute_time());

        if (select_pressed) {
            press_count++;

            // Start timer once we've seen button pressed for multiple frames (debounced)
            if (press_count >= DEBOUNCE_THRESHOLD && select_hold_start == 0) {
                select_hold_start = current_time;
                was_held = false;
            }

            // Check if hold duration met
            if (select_hold_start != 0 && !was_held && (current_time - select_hold_start) >= HOLD_DURATION_MS) {
                was_held = true;
                press_count = 0;
                select_hold_start = 0;
                return true;
            }
        } else {
            // Button released - reset everything
            press_count = 0;
            select_hold_start = 0;
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
    uint32_t results_display_start = 0;
    bool results_displaying = false;

    while (true) {
        drum.updateInputState(input_state, mode);

        // CRITICAL: Always try to get fresh controller data
        // If queue is empty, keep last known state - edge detection in menu handles repeats
        // Don't clear stale data as it creates false rising edges
        queue_try_remove(&controller_input_queue, &input_state.controller);

        // NEW: Send fresh threshold data to Display on every frame when menu is active
        // This ensures Display always shows current values from SettingsStore
        if (menu.active()) {
            auto fresh_thresholds = settings_store->getTriggerThresholds();
            queue_try_remove(&thresholds_queue, nullptr); // Remove stale data
            queue_try_add(&thresholds_queue, &fresh_thresholds);
        }

        // Check for button presses during Tantrum calibration
        const auto& tantrum_state = drum.getTantrumState();
        bool tantrum_active = tantrum_state.isActive();
        
        // Check for A button press to start countdown from instructions screen
        // Track when we entered Instructions mode to debounce button
        static uint32_t instructions_start_time = 0;
        static bool in_instructions = false;
        
        if (tantrum_state.current_mode == Peripherals::Drum::TantrumState::Mode::Instructions) {
            if (!in_instructions) {
                // Just entered Instructions mode - record time
                instructions_start_time = to_ms_since_boot(get_absolute_time());
                in_instructions = true;
            }
            
            // Only accept A button after 300ms delay to prevent accidental skip
            uint32_t now = to_ms_since_boot(get_absolute_time());
            if (input_state.controller.buttons.east && (now - instructions_start_time) >= 300) {
                // User pressed A after debounce period - start the countdown
                drum.startTantrumCountdown();
                in_instructions = false;
            }
        } else {
            // Not in Instructions mode anymore
            in_instructions = false;
        }
        
        // Check for B button press to cancel Tantrum calibration
        if (tantrum_active && input_state.controller.buttons.south) {
            drum.cancelTaikoTantrum();
            
            // CRITICAL: Reset ALL state tracking so Hold START works again immediately
            last_analysis_active = false;
            results_displaying = false;
            results_display_start = 0;
            
            // Return to idle screen (not menu - menu is already closed)
            ControlMessage ctrl_message = {.command = ControlCommand::ExitMenu, .data = {}};
            queue_add_blocking(&control_queue, &ctrl_message);
        }

        // Detect Tantrum completion: finished showing results, apply thresholds and save
        bool just_finished_tantrum = last_analysis_active && !tantrum_active &&
                                     (tantrum_state.current_mode == Peripherals::Drum::TantrumState::Mode::ShowingResults ||
                                      tantrum_state.current_mode == Peripherals::Drum::TantrumState::Mode::Inactive);

        // Handle Tantrum completion: apply thresholds and save
        if (just_finished_tantrum) {
            // Apply the calculated thresholds
            drum.applyTantrumRecommendations();

            // Save to persistent storage
            settings_store->setTriggerThresholds(drum.getCurrentThresholds());
            settings_store->store();
            readSettings();

            // Start the results display timer
            results_display_start = to_ms_since_boot(get_absolute_time());
            results_displaying = true;
        }
        
        if (results_displaying) {
            uint32_t now = to_ms_since_boot(get_absolute_time());
            if ((now - results_display_start) >= 3000) {  // 3 seconds instead of 5
                // Results have been displayed long enough - return to Drum Tuning menu
                menu.activate();
                menu.goBackToParent();  // Pop TaikoTantrum page to return to Drum Tuning
                
                // Tell display to show menu
                ControlMessage ctrl_message = {.command = ControlCommand::EnterMenu, .data = {}};
                queue_add_blocking(&control_queue, &ctrl_message);
                
                // Send menu state to display
                const auto display_msg = menu.getState();
                queue_add_blocking(&menu_display_queue, &display_msg);
                
                // Reset for next calibration
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

                // Exit menu and show Tantrum countdown screen on Core 1
                ControlMessage ctrl_message = {.command = ControlCommand::ExitMenu, .data = {}};
                queue_add_blocking(&control_queue, &ctrl_message);

                // Brief delay to let display state update
                sleep_ms(100);

                // Core 1's display.update() will automatically show the Tantrum screens
                // based on the Drum's TantrumState (countdown → recording → results)
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
            
            // CRITICAL: Clear all input state after menu processing
            // This prevents held buttons from carrying over to next frame
            input_state.releaseAll();
        }
        
        // Menu just closed during update() - no special handling needed
        if (was_menu_active && !menu.active()) {
            // The menu's deactivate() already handles cleanup
        }

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