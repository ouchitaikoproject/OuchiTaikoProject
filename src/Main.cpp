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
queue_t bringup_test_queue;

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
    Peripherals::Display::BringupTestState bringup_test_state{};

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
        if (queue_try_remove(&bringup_test_queue, &bringup_test_state)) {
            display.setBringupTestState(bringup_test_state);
            if (bringup_test_state.active) {
                display.showBringupTest();
            }
        }
        
        // Drive calibration wizard display from GuidedCalState mode.
        // Fail-safe: force the wizard screen state every frame while active so we never
        // remain on Idle/Menu due to a missed mode-change edge.
        {
            using Mode = Peripherals::Drum::GuidedCalState::Mode;
            if (drum_ptr && drum_ptr->isGuidedCalActive()) {
                const auto& ts = drum_ptr->getGuidedCalState();
                switch (ts.current_mode) {
                case Mode::Welcome:
                case Mode::Instructions:
                case Mode::CancelConfirm:
                case Mode::Cancelled:
                    display.showGuidedCalWelcome();
                    break;
                case Mode::PadNormal:
                case Mode::PadHard:
                    display.showGuidedCalPadTest();
                    break;
                case Mode::BleedDetected:
                    display.showGuidedCalError();
                    break;
                case Mode::Review:
                    display.showGuidedCalOverview();
                    break;
                case Mode::Saving:     display.showGuidedCalSaving();     break;
                case Mode::Complete:   display.showGuidedCalComplete();   break;
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
    queue_init(&bringup_test_queue, sizeof(Peripherals::Display::BringupTestState), 1);
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
    uint32_t start_menu_last_press_ms = 0;
    uint32_t select_menu_last_press_ms = 0;
    bool start_menu_prev_down = false;
    bool select_menu_prev_down = false;
    bool start_select_menu_latched = false;

    // NEW: Function to manually reset the hotkey state
    const auto resetHotkeyState = [&]() {
        start_menu_last_press_ms = 0;
        select_menu_last_press_ms = 0;
        start_menu_prev_down = false;
        select_menu_prev_down = false;
        start_select_menu_latched = false;
    };

    // Quick START + SELECT combo to enter the menu without relying on a long
    // single-button hold that can overlap with game UI shortcuts.
    const auto checkMenuCombo =
        [&input_state,
         &start_menu_last_press_ms,
         &select_menu_last_press_ms,
         &start_menu_prev_down,
         &select_menu_prev_down,
         &start_select_menu_latched]() {
            static const uint32_t COMBO_WINDOW_MS = 200;

            const bool start_down = input_state.controller.buttons.start;
            const bool select_down = input_state.controller.buttons.select;
            const bool dpad_active = input_state.controller.dpad.up ||
                                     input_state.controller.dpad.down ||
                                     input_state.controller.dpad.left ||
                                     input_state.controller.dpad.right;
            const uint32_t current_time = to_ms_since_boot(get_absolute_time());

            if (start_down && !start_menu_prev_down) {
                start_menu_last_press_ms = current_time;
            }
            if (select_down && !select_menu_prev_down) {
                select_menu_last_press_ms = current_time;
            }

            bool combo_triggered = false;
            if (!start_select_menu_latched && start_down && select_down && !dpad_active &&
                start_menu_last_press_ms != 0 && select_menu_last_press_ms != 0) {
                const uint32_t delta =
                    (start_menu_last_press_ms > select_menu_last_press_ms)
                        ? (start_menu_last_press_ms - select_menu_last_press_ms)
                        : (select_menu_last_press_ms - start_menu_last_press_ms);
                if (delta <= COMBO_WINDOW_MS) {
                    combo_triggered = true;
                    start_select_menu_latched = true;
                }
            }

            if (!start_down && !select_down) {
                start_select_menu_latched = false;
            }

            start_menu_prev_down = start_down;
            select_menu_prev_down = select_down;
            return combo_triggered;
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
    const auto clamp_u16 = [](uint16_t v, uint16_t lo, uint16_t hi) -> uint16_t {
        return (v < lo) ? lo : ((v > hi) ? hi : v);
    };
    const auto readSettings = [&]() {
        const auto sendCtrlMessage = [&](const ControlMessage &msg) { queue_add_blocking(&control_queue, &msg); };
        const auto current_mode = settings_store->getUsbMode();

        // Sanitize stored drum settings so stale/corrupt flash values cannot
        // silently choke hit detection after schema or firmware changes.
        auto thresholds = settings_store->getTriggerThresholds();
        const uint16_t raw_hit_hold = settings_store->getHitHoldMs();
        const uint16_t safe_hit_hold = clamp_u16(raw_hit_hold, 1, 30);
        thresholds.don_left  = clamp_u16(thresholds.don_left, 10, 1500);
        thresholds.ka_left   = clamp_u16(thresholds.ka_left, 10, 1500);
        thresholds.don_right = clamp_u16(thresholds.don_right, 10, 1500);
        thresholds.ka_right  = clamp_u16(thresholds.ka_right, 10, 1500);

        if (safe_hit_hold != raw_hit_hold || thresholds.don_left != settings_store->getTriggerThresholds().don_left ||
            thresholds.ka_left != settings_store->getTriggerThresholds().ka_left ||
            thresholds.don_right != settings_store->getTriggerThresholds().don_right ||
            thresholds.ka_right != settings_store->getTriggerThresholds().ka_right) {
            settings_store->setHitHoldMs(safe_hit_hold);
            settings_store->setTriggerThresholds(thresholds);
        }

        sendCtrlMessage({.command = ControlCommand::SetUsbMode, .data = {.usb_mode = current_mode}});
        sendCtrlMessage({.command = ControlCommand::SetLedBrightness,
                         .data = {.led_brightness = settings_store->getLedBrightness()}});
        sendCtrlMessage({.command = ControlCommand::SetLedEnablePlayerColor,
                         .data = {.led_enable_player_color = settings_store->getLedEnablePlayerColor()}});

        drum.setHitHoldMs(safe_hit_hold);
        drum.setTriggerThresholds(thresholds);
    };

    Utils::Menu menu(settings_store);

    static constexpr bool kEnableBringupTest = false;
    static constexpr uint8_t kBringupTargetCount = 18;
    uint8_t bringup_current_index = 0;
    bool bringup_active = kEnableBringupTest;
    Utils::InputState previous_bringup_input{};

    const auto pushBringupState = [&]() {
        Peripherals::Display::BringupTestState state{
            .active = bringup_active,
            .current_index = bringup_current_index,
        };
        queue_try_remove(&bringup_test_queue, nullptr);
        queue_try_add(&bringup_test_queue, &state);
    };

    const auto bringupTargetTriggered =
        [&](uint8_t index, const Utils::InputState &current, const Utils::InputState &previous) -> bool {
        switch (index) {
        case 0:  return current.controller.buttons.share && !previous.controller.buttons.share;
        case 1:  return current.controller.buttons.home && !previous.controller.buttons.home;
        case 2:  return current.controller.buttons.select && !previous.controller.buttons.select;
        case 3:  return current.controller.buttons.start && !previous.controller.buttons.start;
        case 4:  return current.controller.buttons.l && !previous.controller.buttons.l;
        case 5:  return current.controller.buttons.r && !previous.controller.buttons.r;
        case 6:  return current.controller.dpad.left && !previous.controller.dpad.left;
        case 7:  return current.controller.dpad.up && !previous.controller.dpad.up;
        case 8:  return current.controller.dpad.down && !previous.controller.dpad.down;
        case 9:  return current.controller.dpad.right && !previous.controller.dpad.right;
        case 10: return current.controller.buttons.west && !previous.controller.buttons.west;   // Y
        case 11: return current.controller.buttons.north && !previous.controller.buttons.north; // X
        case 12: return current.controller.buttons.south && !previous.controller.buttons.south; // B
        case 13: return current.controller.buttons.east && !previous.controller.buttons.east;   // A
        case 14: return current.drum.ka_left.triggered && !previous.drum.ka_left.triggered;
        case 15: return current.drum.don_left.triggered && !previous.drum.don_left.triggered;
        case 16: return current.drum.don_right.triggered && !previous.drum.don_right.triggered;
        case 17: return current.drum.ka_right.triggered && !previous.drum.ka_right.triggered;
        default: return false;
        }
    };

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
    pushBringupState();

    usbd_driver_init(settings_store->getUsbMode());
    usbd_driver_set_player_led_cb([](usb_player_led_t player_led) {
        const auto ctrl_message =
            ControlMessage{.command = ControlCommand::SetPlayerLed, .data = {.player_led = player_led}};
        queue_try_add(&control_queue, &ctrl_message);
    });

    readSettings();

    uint32_t ps4_auth_start_time = 0;
    
    // Guided calibration state tracking for auto-save
    bool last_guided_cal_active = false;
    
    // Guided calibration results display timing
    bool results_displaying = false;

    // Prevent the same A press used to select "Start Guided" from immediately
    // advancing Welcome -> PadNormal in the first active frame.
    bool guided_cal_wait_for_confirm_release = false;

    while (true) {
        const auto mode = settings_store->getUsbMode();
        drum.updateInputState(input_state, mode);

        // Drain queue to the most recent controller sample each frame.
        // This avoids stale "button still down" states that can block edge detection.
        Utils::InputState::Controller sampled_controller{};
        while (queue_try_remove(&controller_input_queue, &sampled_controller)) {
            latest_controller_state = sampled_controller;
        }
        input_state.controller = latest_controller_state;

        if (bringup_active) {
            if (bringup_current_index < kBringupTargetCount &&
                bringupTargetTriggered(bringup_current_index, input_state, previous_bringup_input)) {
                ++bringup_current_index;
                if (bringup_current_index >= kBringupTargetCount) {
                    bringup_active = true; // keep PASS screen visible until power cycle
                }
                pushBringupState();
            }
            previous_bringup_input = input_state;
        }

        // Keep the display and live drum thresholds in sync while editing in the menu.
        // This makes manual tuning feel immediate, while CANCEL still works because the
        // menu restores SettingsStore before exit.
        if (menu.active()) {
            auto fresh_thresholds = settings_store->getTriggerThresholds();
            drum.setTriggerThresholds(fresh_thresholds);
            queue_try_remove(&thresholds_queue, nullptr);
            queue_try_add(&thresholds_queue, &fresh_thresholds);
        }

        // Drive guided calibration A/B buttons in main loop (Core 0)
        const auto& guided_cal_state = drum.getGuidedCalState();
        bool guided_cal_active = guided_cal_state.isActive();
        using TMode = Peripherals::Drum::GuidedCalState::Mode;

        if (guided_cal_active) {
            // Debounce: track A button edge
            static bool last_east = false;
            static bool last_south = false;
            static uint32_t south_hold_start_ms = 0;
            static bool south_cancel_latched = false;
            static constexpr uint32_t SOUTH_CANCEL_HOLD_MS = 180;
            bool east_pressed  = input_state.controller.buttons.east  && !last_east;
            bool south_pressed = input_state.controller.buttons.south && !last_south;
            bool south_down = input_state.controller.buttons.south;
            uint32_t now_ms = to_ms_since_boot(get_absolute_time());
            last_east  = input_state.controller.buttons.east;
            last_south = input_state.controller.buttons.south;

            if (guided_cal_wait_for_confirm_release) {
                if (!input_state.controller.buttons.east) {
                    guided_cal_wait_for_confirm_release = false;
                }
                east_pressed = false;
            }

            // A button: advance wizard
            if (east_pressed) {
                drum.advanceCalibWizard();
            }

            // B button: cancel at any point.
            // Accept either a clean edge or a short hold, so cancel remains reliable
            // even if a frame is dropped and the edge is missed.
            if (south_down) {
                if (south_hold_start_ms == 0) {
                    south_hold_start_ms = now_ms;
                }
            } else {
                south_hold_start_ms = 0;
                south_cancel_latched = false;
            }

            bool south_hold_cancel = south_down &&
                                     south_hold_start_ms != 0 &&
                                     (now_ms - south_hold_start_ms) >= SOUTH_CANCEL_HOLD_MS;
            if (!south_cancel_latched && (south_pressed || south_hold_cancel)) {
                south_cancel_latched = true;
                drum.cancelGuidedCalibration();
                results_displaying = false;
                ControlMessage ctrl_message = {.command = ControlCommand::ExitMenu, .data = {}};
                queue_add_blocking(&control_queue, &ctrl_message);
            }
        }

        // Detect guided calibration completion: Saving mode triggers apply+save, then return to menu
        bool just_entered_saving = last_guided_cal_active &&
                                   guided_cal_state.current_mode == TMode::Saving &&
                                   !results_displaying;
        if (just_entered_saving) {
            drum.applyGuidedCalRecommendations();
            settings_store->setTriggerThresholds(drum.getCurrentThresholds());
            settings_store->setLastGuidedCalReport(
                drum.getLastGuidedCalReportVersion(),
                drum.getLastGuidedCalReport(),
                mode,
                to_ms_since_boot(get_absolute_time()));
            settings_store->store();
            readSettings();
            results_displaying = true;
        }

        if (results_displaying) {
            // Wait for Complete state to finish displaying, then return to Drum Tuning menu
            if (!guided_cal_active || guided_cal_state.current_mode == TMode::Inactive) {  // NOLINT
                menu.activate();
                menu.goBackToParent();
                ControlMessage ctrl_message = {.command = ControlCommand::EnterMenu, .data = {}};
                queue_add_blocking(&control_queue, &ctrl_message);
                const auto display_msg = menu.getState();
                queue_try_remove(&menu_display_queue, nullptr);
                queue_try_add(&menu_display_queue, &display_msg);
                results_displaying = false;
            }
        } else if (last_guided_cal_active && (!guided_cal_active || guided_cal_state.current_mode == TMode::Inactive)) {
            // Cancel/exit path: restore the Drum Tuning menu when guided calibration closes without saving.
            menu.activate();
            menu.goBackToParent();
            ControlMessage ctrl_message = {.command = ControlCommand::EnterMenu, .data = {}};
            queue_add_blocking(&control_queue, &ctrl_message);
            const auto display_msg = menu.getState();
            queue_try_remove(&menu_display_queue, nullptr);
            queue_try_add(&menu_display_queue, &display_msg);
        }

        last_guided_cal_active = guided_cal_active;

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

        if (!bringup_active) {
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

            // TAP START + SELECT together to ENTER menu (not exit - use B button to exit)
            // Check BEFORE menu processing so we can detect the combo
            if (!menu.active() && checkMenuCombo()) {
                // Menu is closed - open it
                menu.activate();
                
                ControlMessage ctrl_message = {.command = ControlCommand::EnterMenu, .data = {}};
                queue_add_blocking(&control_queue, &ctrl_message);
            }

            if (menu.active()) {
                // Pass raw controller state to Menu.cpp
                // Menu.cpp handles all edge detection and hold-to-repeat logic internally
                menu.update(input_state.controller);
                
                // Check if guided calibration start was requested
                if (menu.isGuidedCalStartRequested()) {
                    // Start guided calibration on Core 0
                    drum.startGuidedCalibration();
                    guided_cal_wait_for_confirm_release = true;

                    // Deactivate menu so it doesn't overwrite wizard screens
                    menu.deactivate();

                    // Tell display to exit menu -- wizard screens now driven by core1 state tracker
                    ControlMessage ctrl_message = {.command = ControlCommand::ExitMenu, .data = {}};
                    queue_add_blocking(&control_queue, &ctrl_message);

                    // Core 1's display.update() will automatically show the guided calibration screens
                    // based on the Drum's GuidedCalState (countdown -> recording -> results)
                }

                // Check menu.active() again BEFORE sending display state
                // If menu just closed during update(), don't send state
                if (menu.active()) {
                    // Menu is still active - send display state
                    const auto display_msg = menu.getState();
                    queue_try_remove(&menu_display_queue, nullptr);
                    queue_try_add(&menu_display_queue, &display_msg);
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
                
                // Prevent menu presses from leaking through to gameplay/USB while the
                // menu is open. Menu.cpp already consumed the raw controller state above.
                input_state.releaseAll();
            }
            
            // Menu just closed during update() - no special handling needed
            if (was_menu_active && !menu.active()) {
                // The menu's deactivate() already handles cleanup
            }
        }

        // ============================================================================
        // WEB TOOL SERIAL RECEIVE: Accept threshold packet from ouchitaiko-tool
        // Packet format: [CMD] [donL hi] [donL lo] [donR hi] [donR lo] [kaL hi] [kaL lo] [kaR hi] [kaR lo] 55
        // CMD:
        //   0xAA = apply + persist (Save to Drum)
        //   0xAD = apply only (live test, non-persistent)
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
                const char* report = settings_store->getLastGuidedCalReport();
                if (report[0] == '\0') {
                    report = drum.getLastGuidedCalReport();
                }
                char buf[640];
                if (report[0] != '\0') {
                    snprintf(buf,
                             sizeof(buf),
                             "TLG:%s;SID=%lu;MODE=%u;UP=%lu\n",
                             report,
                             static_cast<unsigned long>(settings_store->getLastGuidedCalSessionId()),
                             static_cast<unsigned>(settings_store->getLastGuidedCalUsbMode()),
                             static_cast<unsigned long>(settings_store->getLastGuidedCalUptimeMs()));
                } else {
                    snprintf(buf, sizeof(buf), "TLG:EMPTY\n");
                }
                tud_cdc_write(buf, strlen(buf));
                tud_cdc_write_flush();
            } else if (rx_pos == 0 && byte != 0xAA && byte != 0xAD) {
                // Not a start byte -- ignore and stay ready
            } else {
                rx_buf[rx_pos++] = byte;

                if (rx_pos == 10) {
                    rx_pos = 0;
                    if (rx_buf[9] == 0x55) {
                        // Valid packet -- apply thresholds
                        Peripherals::Drum::Config::Thresholds t;
                        t.don_left  = (static_cast<uint16_t>(rx_buf[1]) << 8) | rx_buf[2];
                        t.don_right = (static_cast<uint16_t>(rx_buf[3]) << 8) | rx_buf[4];
                        t.ka_left   = (static_cast<uint16_t>(rx_buf[5]) << 8) | rx_buf[6];
                        t.ka_right  = (static_cast<uint16_t>(rx_buf[7]) << 8) | rx_buf[8];

                        drum.setTriggerThresholds(t);
                        if (rx_buf[0] == 0xAA) {
                            settings_store->setTriggerThresholds(t);
                            settings_store->store();

                            // Send ACK back to web tool
                            const char* ack = "OK\n";
                            tud_cdc_write(ack, 3);
                            tud_cdc_write_flush();
                        } else if (rx_buf[0] == 0xAD) {
                            // Live apply only (no persistent write)
                            const char* ack = "LIVE\n";
                            tud_cdc_write(ack, 5);
                            tud_cdc_write_flush();
                        }
                    }
                    // Bad end byte -- silently discard, rx_pos already reset
                }
            }
        }
        // ============================================================================
        // END WEB TOOL SERIAL RECEIVE
        // ============================================================================

        Utils::InputState report_state = input_state;
        if (menu.active()) {
            report_state.releaseAll();
        }
        usbd_driver_send_report(input_report.getReport(report_state, mode));
        usbd_driver_task();

        queue_try_add(&drum_input_queue, &drum_message);

        if (queue_try_remove(&auth_signed_challenge_queue, auth_challenge_response.data())) {
            ps4_auth_set_signed_challenge(auth_challenge_response.data());
        }
    }

    return 0;
}

//End of Main.cpp





