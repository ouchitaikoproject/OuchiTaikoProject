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

// Queue for Taiko-Tune commands from core 1 to core 0
queue_t taikotune_command_queue;

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

// Taiko-Tune V2 command structure - UPDATED for double-pass
enum class TaikoTuneCommand : uint8_t {
    StartAnalysis,
    ShowAnalysisScreen,
    ExitAnalysisScreen,
    ShowCancelled,        // NEW: Show cancelled screen
    ShowSplash,           // NEW: Show initial splash screen
    ShowPassTransition,   // NEW: Show transition between passes
    SetPass,              // NEW: Set current pass number (1 or 2)
    ShowComplete,         // NEW: Show completion screen
};

struct TaikoTuneMessage {
    TaikoTuneCommand command;
    Peripherals::Drum::Id pad_id;
    uint8_t pass_number;  // For SetPass command
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
    TaikoTuneMessage taikotune_msg{};

    // Receive drum pointer from core 0
    Peripherals::Drum *drum_ptr = nullptr;
    queue_remove_blocking(&drum_reference_queue, &drum_ptr);
    display.setDrumReference(drum_ptr);

    while (true) {
        controller.updateInputState(input_state);

        // CRITICAL: Remove any stale data first, then add fresh data
        // This ensures Core 0 always gets the latest controller state
        queue_try_remove(&controller_input_queue, nullptr);
        queue_add_blocking(&controller_input_queue, &input_state.controller);
        
        queue_try_remove(&drum_input_queue, &input_state.drum);

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
        
        // Handle commands from core 0 - UPDATED for double-pass
        if (queue_try_remove(&taikotune_command_queue, &taikotune_msg)) {
            switch (taikotune_msg.command) {
            case TaikoTuneCommand::ShowAnalysisScreen:
                display.showTaikoTuneAnalysis();
                break;
            case TaikoTuneCommand::ExitAnalysisScreen:
                display.showMenu();
                break;
            case TaikoTuneCommand::ShowCancelled:
                display.showTaikoTuneCancelled();
                break;
            case TaikoTuneCommand::ShowSplash:
                display.showTaikoTuneAllDrumsSplash();
                break;
            case TaikoTuneCommand::ShowPassTransition:
                display.showTaikoTunePassTransition();
                break;
            case TaikoTuneCommand::SetPass:
                display.setCurrentPass(taikotune_msg.pass_number);
                break;
            case TaikoTuneCommand::ShowComplete:
                display.showTaikoTuneComplete();
                break;
            default:
                break;
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
    queue_init(&controller_input_queue, sizeof(Utils::InputState::Controller), 1);
    queue_init(&auth_challenge_queue, sizeof(std::array<uint8_t, Utils::PS4AuthProvider::SIGNATURE_LENGTH>), 1);
    queue_init(&auth_signed_challenge_queue, sizeof(std::array<uint8_t, Utils::PS4AuthProvider::SIGNATURE_LENGTH>), 1);
    
    // Initialize Taiko-Tune queues
    queue_init(&drum_reference_queue, sizeof(Peripherals::Drum*), 1);
    queue_init(&taikotune_command_queue, sizeof(TaikoTuneMessage), 1);

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
    
    // Hold SELECT for menu (1 second CONTINUOUS hold)
    const auto checkHoldSelect = [&input_state]() {
        static uint32_t select_hold_start = 0;
        static bool was_held = false;
        static bool was_pressed_last_frame = false;
        static const uint32_t HOLD_DURATION_MS = 1000;
        
        bool select_pressed = input_state.controller.buttons.select;
        uint32_t current_time = to_ms_since_boot(get_absolute_time());
        
        // CRITICAL: If button was released since last frame, reset the timer
        if (!select_pressed && was_pressed_last_frame) {
            select_hold_start = 0;
            was_held = false;
        }
        
        if (select_pressed) {
            if (select_hold_start == 0) {
                select_hold_start = current_time;
                was_held = false;
            } else if (!was_held && (current_time - select_hold_start) >= HOLD_DURATION_MS) {
                was_held = true;
                was_pressed_last_frame = select_pressed;
                return true;
            }
        } else {
            select_hold_start = 0;
            was_held = false;
        }
        
        was_pressed_last_frame = select_pressed;
        return false;
    };
    
    // Hold START for All 4 Drums calibration (1 second CONTINUOUS hold)
    const auto checkHoldStart = [&input_state]() {
        static uint32_t start_hold_start = 0;
        static bool was_held = false;
        static bool was_pressed_last_frame = false;
        static const uint32_t HOLD_DURATION_MS = 1000;
        
        bool start_pressed = input_state.controller.buttons.start;
        uint32_t current_time = to_ms_since_boot(get_absolute_time());
        
        // CRITICAL: If button was released since last frame, reset the timer
        if (!start_pressed && was_pressed_last_frame) {
            start_hold_start = 0;
            was_held = false;
        }
        
        if (start_pressed) {
            if (start_hold_start == 0) {
                start_hold_start = current_time;
                was_held = false;
            } else if (!was_held && (current_time - start_hold_start) >= HOLD_DURATION_MS) {
                was_held = true;
                was_pressed_last_frame = start_pressed;
                return true;
            }
        } else {
            start_hold_start = 0;
            was_held = false;
        }
        
        was_pressed_last_frame = start_pressed;
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
        drum.setBigHitEnable(settings_store->getBigHitEnable());
        drum.setBigHitThreshold(settings_store->getBigHitThreshold());
        drum.setSimulTap(settings_store->getSimulTap());
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
    
    // Taiko-Tune state tracking for auto-save and sequential mode
    bool last_analysis_active = false;
    
    // All 4 Drums sequential mode state - UPDATED for double-pass (8 drums)
    bool all_drums_mode_active = false;
    uint8_t current_drum_index = 0;
    uint8_t last_processed_drum_index = 255;  // Guard against duplicate processing
    const Peripherals::Drum::Id drum_sequence[8] = {
        // PASS 1: Forward (Left to Right)
        Peripherals::Drum::Id::KA_LEFT,
        Peripherals::Drum::Id::DON_LEFT,
        Peripherals::Drum::Id::DON_RIGHT,
        Peripherals::Drum::Id::KA_RIGHT,
        // PASS 2: Reverse (Right to Left) - for perfect crosstalk compensation
        Peripherals::Drum::Id::KA_RIGHT,
        Peripherals::Drum::Id::DON_RIGHT,
        Peripherals::Drum::Id::DON_LEFT,
        Peripherals::Drum::Id::KA_LEFT
    };

    while (true) {
        drum.updateInputState(input_state);
        
        // CRITICAL: Always wait for fresh controller data
        // queue_try_remove can fail and leave stale data in input_state.controller
        // This causes menu freeze on second entry because buttons are "stuck"
        queue_remove_blocking(&controller_input_queue, &input_state.controller);

        // Check for B button press to cancel analysis
        const auto& tt_state = drum.getTaikoTuneState();
        bool current_analysis_active = tt_state.isActive();

        if (current_analysis_active && input_state.controller.buttons.south) {
            drum.cancelTaikoTuneAnalysis();
        }
        
        // Detect cancellation: went from active to inactive WITHOUT entering ShowingResults
        bool just_cancelled = last_analysis_active && !current_analysis_active && 
                              tt_state.current_mode == Peripherals::Drum::TaikoTuneState::Mode::Inactive;
        
        // Detect completion: went from active to inactive AND entered ShowingResults
        bool just_finished = last_analysis_active && !current_analysis_active && 
                             tt_state.current_mode == Peripherals::Drum::TaikoTuneState::Mode::ShowingResults;

        if (just_cancelled) {
            // Analysis was cancelled - clean up state
            if (all_drums_mode_active) {
                all_drums_mode_active = false;
                current_drum_index = 0;
            }
            
            // Restore backed up thresholds
            readSettings();
            
            // Show cancelled screen for 2 seconds
            TaikoTuneMessage cancelled_msg{
                .command = TaikoTuneCommand::ShowCancelled,
                .pad_id = Peripherals::Drum::Id::KA_LEFT,
                .pass_number = 0
            };
            queue_try_add(&taikotune_command_queue, &cancelled_msg);
            
            // Wait for cancelled screen
            sleep_ms(2000);
            
            // Check if we came from menu or triple-tap
            if (menu.active()) {
                // Came from menu - stay in menu, just exit the analysis screen back to menu
                TaikoTuneMessage exit_msg{
                    .command = TaikoTuneCommand::ExitAnalysisScreen,
                    .pad_id = Peripherals::Drum::Id::KA_LEFT,
                    .pass_number = 0
                };
                queue_try_add(&taikotune_command_queue, &exit_msg);
            } else {
                // Came from triple-tap - go back to idle
                ControlMessage ctrl_message{.command = ControlCommand::ExitMenu, .data = {}};
                queue_add_blocking(&control_queue, &ctrl_message);
            }
            // CRITICAL FIX: Ensure hotkey is reset if analysis was cancelled outside the menu
            resetHotkeyState(); 
        }

        // CRITICAL: All 4 Drums mode - skip results display, go straight to next drum!
        // This completely bypasses the Display's autonomous timeout issue
        // SIMPLIFIED: Check ONLY mode flags, no transition detection
        if (all_drums_mode_active &&
            tt_state.current_mode == Peripherals::Drum::TaikoTuneState::Mode::ShowingResults &&
            current_drum_index != last_processed_drum_index) {
            // Just completed a drum we haven't processed yet
            // Mark as processed to prevent duplicate handling
            last_processed_drum_index = current_drum_index;

            // Save thresholds immediately
            settings_store->setTriggerThresholds(drum.getCurrentThresholds());
            settings_store->store();
            readSettings();

            // UX: Display results for 2.5 seconds so user can see the threshold value
            // Screen shows "Next drum starting..." so they know it's not frozen
            sleep_ms(2500);

            current_drum_index++;

            if (current_drum_index < 8) {
                // More drums to go - check if we need to show pass transition first

                // CRITICAL FIX: Show pass transition BEFORE starting next drum
                // This ensures user sees the transition screen and next drum has proper countdown
                if (current_drum_index == 4) {
                    // Just finished Pass 1 (drums 0-3), about to start Pass 2 (drums 4-7)
                    TaikoTuneMessage transition_msg{
                        .command = TaikoTuneCommand::ShowPassTransition,
                        .pad_id = Peripherals::Drum::Id::KA_LEFT,
                        .pass_number = 0
                    };
                    queue_try_add(&taikotune_command_queue, &transition_msg);
                    sleep_ms(3000);  // Display transition screen for 3 seconds
                }

                // Update pass number for display
                uint8_t pass_number = (current_drum_index < 4) ? 1 : 2;
                TaikoTuneMessage pass_msg{
                    .command = TaikoTuneCommand::SetPass,
                    .pad_id = Peripherals::Drum::Id::KA_LEFT,
                    .pass_number = pass_number
                };
                queue_try_add(&taikotune_command_queue, &pass_msg);

                // Now start the next drum analysis
                drum.startTaikoTuneAnalysis(drum_sequence[current_drum_index]);

                // Brief delay to let state change propagate to Core 1
                sleep_ms(100);

                // Tell Display to show analysis screen
                TaikoTuneMessage show_msg{
                    .command = TaikoTuneCommand::ShowAnalysisScreen,
                    .pad_id = drum_sequence[current_drum_index],
                    .pass_number = 0
                };
                queue_try_add(&taikotune_command_queue, &show_msg);
            } else {
                // All 8 drums complete! Show completion screen
                TaikoTuneMessage complete_msg{
                    .command = TaikoTuneCommand::ShowComplete,
                    .pad_id = Peripherals::Drum::Id::KA_LEFT,
                    .pass_number = 0
                };
                queue_try_add(&taikotune_command_queue, &complete_msg);
                sleep_ms(5000);  // Show completion screen for 5 seconds

                // Then exit back to idle (we used START shortcut, not menu)
                all_drums_mode_active = false;
                current_drum_index = 0;
                resetHotkeyState();

                // Go back to idle screen
                ControlMessage ctrl_message{.command = ControlCommand::ExitMenu, .data = {}};
                queue_add_blocking(&control_queue, &ctrl_message);
            }
        } else if (just_finished && !all_drums_mode_active) {
            // SINGLE DRUM MODE ONLY (All 4 Drums handled above)
            // Save thresholds to flash
            settings_store->setTriggerThresholds(drum.getCurrentThresholds());
            settings_store->store();
            readSettings();

            // Show results, then return to menu
            sleep_ms(3000);

            if (menu.active()) {
                TaikoTuneMessage exit_msg{
                    .command = TaikoTuneCommand::ExitAnalysisScreen,
                    .pad_id = Peripherals::Drum::Id::KA_LEFT,
                    .pass_number = 0
                };
                queue_try_add(&taikotune_command_queue, &exit_msg);
            } else {
                ControlMessage ctrl_message{.command = ControlCommand::ExitMenu, .data = {}};
                queue_add_blocking(&control_queue, &ctrl_message);
            }
        }

        last_analysis_active = current_analysis_active;
        
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
        
        if (menu.active()) {
            menu.update(input_state.controller);
            
            // Check if Taiko-Tune start was requested
            if (menu.isTaikoTuneStartRequested()) {
                auto requested_page = menu.getTaikoTuneRequestedPage();
                
                // Check if this is "All 4 Drums" mode (double-pass)
                if (requested_page == Utils::Menu::Page::TaikoTuneAllDrums) {
                    // Start sequential mode
                    all_drums_mode_active = true;
                    current_drum_index = 0;
                    last_processed_drum_index = 255;  // Reset guard
                    
                    // Show splash screen first
                    TaikoTuneMessage splash_msg{
                        .command = TaikoTuneCommand::ShowSplash,
                        .pad_id = Peripherals::Drum::Id::KA_LEFT,
                        .pass_number = 0
                    };
                    queue_try_add(&taikotune_command_queue, &splash_msg);

                    // Clear spurious inputs before splash screen display
                    queue_try_remove(&controller_input_queue, nullptr);

                    // Wait for splash screen (3 seconds)
                    sleep_ms(3000);
                    
                    // Set initial pass to 1
                    TaikoTuneMessage pass_msg{
                        .command = TaikoTuneCommand::SetPass,
                        .pad_id = Peripherals::Drum::Id::KA_LEFT,
                        .pass_number = 1
                    };
                    queue_try_add(&taikotune_command_queue, &pass_msg);
                    
                    // Now start with first drum (Ka Left)
                    drum.startTaikoTuneAnalysis(drum_sequence[current_drum_index]);
                    
                    // Tell core 1 to show the analysis screen
                    TaikoTuneMessage show_msg{
                        .command = TaikoTuneCommand::ShowAnalysisScreen,
                        .pad_id = drum_sequence[current_drum_index],
                        .pass_number = 0
                    };
                    queue_try_add(&taikotune_command_queue, &show_msg);
                } else {
                    // Single drum mode
                    all_drums_mode_active = false;
                    
                    // Determine which pad based on current page
                    Peripherals::Drum::Id pad_id;
                    bool valid_page = true;
                    
                    switch (requested_page) {
                        case Utils::Menu::Page::TaikoTuneKaLeft:
                            pad_id = Peripherals::Drum::Id::KA_LEFT;
                            break;
                        case Utils::Menu::Page::TaikoTuneDonLeft:
                            pad_id = Peripherals::Drum::Id::DON_LEFT;
                            break;
                        case Utils::Menu::Page::TaikoTuneDonRight:
                            pad_id = Peripherals::Drum::Id::DON_RIGHT;
                            break;
                        case Utils::Menu::Page::TaikoTuneKaRight:
                            pad_id = Peripherals::Drum::Id::KA_RIGHT;
                            break;
                        default:
                            valid_page = false;
                            break;
                    }
                    
                    if (valid_page) {
                        // Start the analysis on core 0
                        drum.startTaikoTuneAnalysis(pad_id);
                        
                        // Tell core 1 to show the analysis screen
                        TaikoTuneMessage show_msg{
                            .command = TaikoTuneCommand::ShowAnalysisScreen,
                            .pad_id = pad_id,
                            .pass_number = 0
                        };
                        queue_try_add(&taikotune_command_queue, &show_msg);
                    }
                }
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
        
        // Check for special inputs when menu is NOT active
        if (!menu.active()) {
            if (checkHoldStart()) {
                // HOLD START DETECTED - Launch All 4 Drums calibration instantly!
                all_drums_mode_active = true;
                current_drum_index = 0;
                last_processed_drum_index = 255;  // Reset guard
                
                // Show splash screen first
                TaikoTuneMessage splash_msg{
                    .command = TaikoTuneCommand::ShowSplash,
                    .pad_id = Peripherals::Drum::Id::KA_LEFT,
                    .pass_number = 0
                };
                queue_try_add(&taikotune_command_queue, &splash_msg);

                // Clear spurious inputs before splash screen display
                queue_try_remove(&controller_input_queue, nullptr);

                // Wait for splash screen (3 seconds)
                sleep_ms(3000);
                
                // Set initial pass to 1
                TaikoTuneMessage pass_msg{
                    .command = TaikoTuneCommand::SetPass,
                    .pad_id = Peripherals::Drum::Id::KA_LEFT,
                    .pass_number = 1
                };
                queue_try_add(&taikotune_command_queue, &pass_msg);
                
                // Start with first drum (Ka Left)
                drum.startTaikoTuneAnalysis(drum_sequence[current_drum_index]);
                
                // Tell core 1 to show the analysis screen
                TaikoTuneMessage show_msg{
                    .command = TaikoTuneCommand::ShowAnalysisScreen,
                    .pad_id = drum_sequence[current_drum_index],
                    .pass_number = 0
                };
                queue_try_add(&taikotune_command_queue, &show_msg);
                
            } else if (checkHoldSelect()) {
                // HOLD SELECT DETECTED - Open menu
                menu.activate();

                ControlMessage ctrl_message{.command = ControlCommand::EnterMenu, .data = {}};
                queue_add_blocking(&control_queue, &ctrl_message);
            }
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
