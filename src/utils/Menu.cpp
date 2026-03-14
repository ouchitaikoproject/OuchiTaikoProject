// Beginning of file Menu.cpp

#include "utils/Menu.h"
#include "GlobalConfiguration.h"

#include "peripherals/Drum.h"

namespace OuchiTaiko::Utils {

const std::map<Menu::Page, const Menu::Descriptor> Menu::descriptors = {
    // NEW MAIN MENU - 6 items (Gameplay removed - it's empty)
   {Menu::Page::Main,
 {Menu::Descriptor::Type::Menu,
  "Main Menu",
  {{"Controller\nMode", Menu::Descriptor::Action::GotoPageDeviceMode},
   {"Drum\nTuning", Menu::Descriptor::Action::GotoPageDrumTuning},
   {"Advanced", Menu::Descriptor::Action::GotoPageAdvanced},
   {"USB Flash\nMode", Menu::Descriptor::Action::GotoPageBootsel},
   {"About", Menu::Descriptor::Action::GotoPageAbout}},
  0}},

    // Controller Mode (unchanged)
    {Menu::Page::DeviceMode,
     {Menu::Descriptor::Type::Selection,
      "Device:",
      {{"Switch\nTatacon", Menu::Descriptor::Action::SetUsbMode},
       {"Switch\nPro", Menu::Descriptor::Action::SetUsbMode},
       {"Sony PS3\nDualshock3", Menu::Descriptor::Action::SetUsbMode},
       {"PS4\nTatacon", Menu::Descriptor::Action::SetUsbMode},
       {"Sony PS4\nDualshock4", Menu::Descriptor::Action::SetUsbMode},
       {"Keyboard\nPlayer 1", Menu::Descriptor::Action::SetUsbMode},
       {"Keyboard\nPlayer 2", Menu::Descriptor::Action::SetUsbMode},
       {"Xbox 360\nController", Menu::Descriptor::Action::SetUsbMode},
       {"Android\n(XInput)", Menu::Descriptor::Action::SetUsbMode},
       {"iOS\n(XInput)", Menu::Descriptor::Action::SetUsbMode},
       {"Analog\nPlayer 1", Menu::Descriptor::Action::SetUsbMode},
       {"Analog\nPlayer 2", Menu::Descriptor::Action::SetUsbMode},
       {"MIDI\nController", Menu::Descriptor::Action::SetUsbMode},
       {"Calibrate\n/Debug", Menu::Descriptor::Action::SetUsbMode}},
      0}},

    // Drum Tuning submenu (2 items) - Pure tuning
    {Menu::Page::DrumTuning,
 {Menu::Descriptor::Type::Menu,
  "Drum\nTuning",
  {{"Auto\nCalibrate", Menu::Descriptor::Action::GotoPageTaikoTantrum},
   {"Manual\nThresholds", Menu::Descriptor::Action::GotoPageDrumTriggerThresholdsManual},
   {"Reset\nThresholds", Menu::Descriptor::Action::GotoPageDrumTriggerThresholdsReset}},
  0}},

    {Menu::Page::Gameplay,
     {Menu::Descriptor::Type::Menu,
      "Gameplay\nMods",
      {},  // Empty - no items
      0}},



    // Advanced submenu (2 items - technical settings)
   {Menu::Page::Advanced,
 {Menu::Descriptor::Type::Menu,
  "Advanced",
  { {"Reset ALL\nSettings", Menu::Descriptor::Action::GotoPageReset},
    {"Hold Time\n(Debounce)", Menu::Descriptor::Action::GotoPageDrumDebounceDelay}},
  0}},

    // Unified Manual Thresholds Page (all 4 thresholds with live drum animations)
    {Menu::Page::DrumTriggerThresholdsManual,
     {Menu::Descriptor::Type::UnifiedThresholds,
      "Manual\nThresholds",
      {{"KaL", Menu::Descriptor::Action::None},
       {"DonL", Menu::Descriptor::Action::None},
       {"DonR", Menu::Descriptor::Action::None},
       {"KaR", Menu::Descriptor::Action::None}},
      0}},

    // Reset Thresholds confirmation (unchanged)
    {Menu::Page::DrumTriggerThresholdsReset,
     {Menu::Descriptor::Type::Toggle,
      "Reset All\nThresholds?",
      {{"Yes / No", Menu::Descriptor::Action::DoResetThresholds}},
      0}},

    // Taiko Tantrum Calibration
    {Menu::Page::TaikoTantrum,
     {Menu::Descriptor::Type::Selection,
      "Auto\nCalibrate",
      {{"Start\nWizard", Menu::Descriptor::Action::StartTaikoTantrum}},
      0}},

    // Value adjustments (unchanged)
    {Menu::Page::DrumDebounceDelay,
     {Menu::Descriptor::Type::Value,
      "Hold Time (ms)",
      {{"", Menu::Descriptor::Action::SetDrumDebounceDelay}},
      UINT8_MAX}},

    {Menu::Page::DrumPerformanceProfile,
     {Menu::Descriptor::Type::Selection,
      "Fast Roll\nMode",
      {{"Safe\n(25ms)", Menu::Descriptor::Action::SetPerformanceProfile},
       {"Normal\n(12ms)", Menu::Descriptor::Action::SetPerformanceProfile},
       {"EXTREME\n(8ms)", Menu::Descriptor::Action::SetPerformanceProfile}},
      1}},  // Default to option 1 (Normal)

    {Menu::Page::DrumTriggerThresholdKaLeft,
     {Menu::Descriptor::Type::Value,
      "Trigger L. Ka",
      {{"", Menu::Descriptor::Action::SetDrumTriggerThresholdKaLeft}},
      4095}},

    {Menu::Page::DrumTriggerThresholdDonLeft,
     {Menu::Descriptor::Type::Value,
      "Trigger L. Don",
      {{"", Menu::Descriptor::Action::SetDrumTriggerThresholdDonLeft}},
      4095}},

    {Menu::Page::DrumTriggerThresholdDonRight,
     {Menu::Descriptor::Type::Value,
      "Trigger R. Don",
      {{"", Menu::Descriptor::Action::SetDrumTriggerThresholdDonRight}},
      4095}},

    {Menu::Page::DrumTriggerThresholdKaRight,
     {Menu::Descriptor::Type::Value,
      "Trigger R. Ka",
      {{"", Menu::Descriptor::Action::SetDrumTriggerThresholdKaRight}},
      4095}},

    /*
    // LED Brightness - NOW DIRECT (no submenu)
    {Menu::Page::LedBrightness,
     {Menu::Descriptor::Type::Value,
      "LED\nBrightness",
      {{"", Menu::Descriptor::Action::SetLedBrightness}},
      UINT8_MAX}},
    */

    // Reset confirmation (unchanged)
    {Menu::Page::Reset,
     {Menu::Descriptor::Type::Toggle,
      "Reset All\nSettings?",
      {{"Yes / No", Menu::Descriptor::Action::DoReset}},
      0}},

    // Bootsel confirmation (unchanged)
    {Menu::Page::Bootsel,
     {Menu::Descriptor::Type::Toggle,
      "Reboot to\nFlash Mode?",
      {{"Yes / No", Menu::Descriptor::Action::DoRebootToBootsel}},
      0}},

    // About (firmware version changed to "Menu")
    {Menu::Page::About,
     {Menu::Descriptor::Type::Selection,
      "About",
      {{"OuchiTaiko\nby KillerQ", Menu::Descriptor::Action::None},
       {"OuchiTaiko\n.com", Menu::Descriptor::Action::None},
       {"Firmware:\nv12.5", Menu::Descriptor::Action::None},
        {"Build:\n53 Harbor", Menu::Descriptor::Action::None},
       {"Based on:\nDonCon2040", Menu::Descriptor::Action::None},
       {"& HIDtaiko", Menu::Descriptor::Action::None}},
      0}},

    // Reboot messages (unchanged)
    {Menu::Page::BootselMsg,
     {Menu::Descriptor::Type::RebootCountdown,
      "Bootloader",
      {{"Countdown", Menu::Descriptor::Action::None}},
      0}},

    {Menu::Page::RebootMsg,
     {Menu::Descriptor::Type::RebootCountdown,
      "Rebooting...",
      {{"Countdown", Menu::Descriptor::Action::None}},
      0}},
};

Menu::Buttons::Buttons()
    : m_states({{Id::Up, {}}, {Id::Down, {}}, {Id::Left, {}}, {Id::Right, {}}, {Id::Confirm, {}}, {Id::Back, {}}}) {}

void Menu::Buttons::update(const InputState::Controller &controller_state, Descriptor::Type page_type) {
    (void)page_type;
    // Repeat timing constants
    static constexpr uint32_t REPEAT_INITIAL_DELAY_MS = 380;
    static constexpr uint32_t REPEAT_INTERVAL_MS = 140;
    static constexpr uint32_t FAST_REPEAT_START_MS = 1200;
    static constexpr uint32_t FAST_REPEAT_INTERVAL_MS = 60;

    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    // Repeat disabled globally to enforce single-step navigation/input.
    bool enable_lr_repeat = false;
    bool enable_ud_repeat = false;

    // Edge-detect handler (for most buttons and page types)
    auto handle_button_edge = [](State &button_state, bool input_state) {
        if (input_state && button_state.repeat == State::Repeat::Idle) {
            // Button just pressed (was idle, now pressed)
            button_state.pressed = true;
            button_state.repeat = State::Repeat::RepeatDelay;
        } else if (!input_state) {
            // Button released - reset to idle
            button_state.pressed = false;
            button_state.repeat = State::Repeat::Idle;
        } else {
            // Button still held - don't fire again
            button_state.pressed = false;
        }
    };

    // Repeat handler (for Left/Right on Value/UnifiedThresholds pages only)
    // Note: constexpr values used directly, only capture runtime variable
    auto handle_button_repeat = [current_time](State &button_state, bool input_state) {
        if (input_state) {
            if (button_state.repeat == State::Repeat::Idle) {
                // Button just pressed - fire immediately and start delay timer
                button_state.pressed = true;
                button_state.repeat = State::Repeat::RepeatDelay;
                button_state.pressed_since = current_time;
                button_state.last_repeat = 0;
            } else if (button_state.repeat == State::Repeat::RepeatDelay) {
                // Waiting for initial delay
                if (current_time - button_state.pressed_since >= REPEAT_INITIAL_DELAY_MS) {
                    // Initial delay elapsed - start repeating
                    button_state.pressed = true;
                    button_state.repeat = State::Repeat::Repeat;
                    button_state.last_repeat = current_time;
                } else {
                    button_state.pressed = false;
                }
            } else if (button_state.repeat == State::Repeat::Repeat) {
                const uint32_t held_ms = current_time - button_state.pressed_since;
                if (held_ms >= FAST_REPEAT_START_MS) {
                    button_state.pressed = true;
                    button_state.repeat = State::Repeat::FastRepeat;
                    button_state.last_repeat = current_time;
                } else if (current_time - button_state.last_repeat >= REPEAT_INTERVAL_MS) {
                    button_state.pressed = true;
                    button_state.last_repeat = current_time;
                } else {
                    button_state.pressed = false;
                }
            } else if (button_state.repeat == State::Repeat::FastRepeat) {
                if (current_time - button_state.last_repeat >= FAST_REPEAT_INTERVAL_MS) {
                    button_state.pressed = true;
                    button_state.last_repeat = current_time;
                } else {
                    button_state.pressed = false;
                }
            }
        } else {
            // Button released - reset to idle
            button_state.pressed = false;
            button_state.repeat = State::Repeat::Idle;
            button_state.pressed_since = 0;
            button_state.last_repeat = 0;
        }
    };

    // Handle each button based on whether repeat is enabled
    if (enable_ud_repeat) {
        handle_button_repeat(m_states.at(Id::Up), controller_state.dpad.up);
        handle_button_repeat(m_states.at(Id::Down), controller_state.dpad.down);
    } else {
        handle_button_edge(m_states.at(Id::Up), controller_state.dpad.up);
        handle_button_edge(m_states.at(Id::Down), controller_state.dpad.down);
    }

    if (enable_lr_repeat) {
        handle_button_repeat(m_states.at(Id::Left), controller_state.dpad.left);
        handle_button_repeat(m_states.at(Id::Right), controller_state.dpad.right);
    } else {
        handle_button_edge(m_states.at(Id::Left), controller_state.dpad.left);
        handle_button_edge(m_states.at(Id::Right), controller_state.dpad.right);
    }

    handle_button_edge(m_states.at(Id::Confirm), controller_state.buttons.east);
    handle_button_edge(m_states.at(Id::Back), controller_state.buttons.south);
}

bool Menu::Buttons::getPressed(Id id) const { return m_states.at(id).pressed; }

void Menu::Buttons::reset() {
    // Reset all button states to idle
    for (auto &[id, state] : m_states) {
        state.pressed = false;
        state.repeat = State::Repeat::Idle;
        state.pressed_since = 0;
        state.last_repeat = 0;
    }
}

Menu::Menu(std::shared_ptr<SettingsStore> settings_store) : m_store(std::move(settings_store)) {}

void Menu::enterBootloaderSplash() {
    m_store->scheduleReboot(true);
    activate();
    gotoPage(Page::BootselMsg);
}

void Menu::activate() {
    m_active = true;
    m_buttons.reset();  // Start with clean button state
    m_waiting_for_button_release = true;
    m_ignore_input_frames = 3;  // Suppress input briefly on menu open
    m_confirm_unlock_until_ms = to_ms_since_boot(get_absolute_time()) + CONFIRM_GUARD_AFTER_PAGE_MS;
}

void Menu::setWaitingForButtonRelease(bool waiting) {
    m_waiting_for_button_release = waiting;
}

void Menu::deactivate() {
    m_active = false;
    // CRITICAL FIX FOR SECOND ENTRY: This resets the button state machine when exiting, 
    // ensuring no lingering button states cause a freeze on the next entry.
    m_state_stack = std::stack<State>({{.page = Page::Main, .selected_value = 0, .original_value = 0}});
    m_buttons.reset();
    m_ignore_input_frames = 0;
    m_waiting_for_button_release = false;
    m_confirm_unlock_until_ms = 0;
}

void Menu::goBackToParent() {
    // Simply pop the current page without restore - used after Tantrum completion
    if (m_state_stack.size() > 1) {
        m_state_stack.pop();
    }
}


uint16_t Menu::getCurrentValue(Menu::Page page) {
    switch (page) {
    case Page::DeviceMode:
        return static_cast<uint16_t>(m_store->getUsbMode());
    case Page::DrumDebounceDelay:
        return m_store->getDebounceDelay();
    case Page::DrumPerformanceProfile:
        // Performance Profile feature removed - return 0
        return 0;
    case Page::DrumTriggerThresholdKaLeft:
        return m_store->getTriggerThresholds().ka_left;
    case Page::DrumTriggerThresholdDonLeft:
        return m_store->getTriggerThresholds().don_left;
    case Page::DrumTriggerThresholdDonRight:
        return m_store->getTriggerThresholds().don_right;
    case Page::DrumTriggerThresholdKaRight:
        return m_store->getTriggerThresholds().ka_right;
    case Page::LedBrightness:
        return m_store->getLedBrightness();
    case Page::DrumTriggerThresholdsManual:
        // Unified threshold page: Start with first threshold (KaLeft) selected
        return 0;
    case Page::Main:
    case Page::DrumTuning:
    case Page::Gameplay:
    case Page::Advanced:
    case Page::DrumTriggerThresholds:
    case Page::DrumTriggerThresholdsAuto:
    case Page::DrumTriggerThresholdsReset:
    case Page::About:
    case Page::Reset:
    case Page::Bootsel:
    case Page::BootselMsg:
    case Page::RebootMsg:
    case Page::TaikoTantrum:
        break;
    }

    return 0;
}

void Menu::gotoPage(Menu::Page page) {
    const auto current_value = getCurrentValue(page);

    // Special handling for UnifiedThresholds page - store original values for cancel
    if (page == Page::DrumTriggerThresholdsManual) {
        m_unified_thresholds_original = m_store->getTriggerThresholds();
    }

    m_state_stack.push({page, current_value, current_value});
    m_ignore_input_frames = 3;  // Suppress input briefly after page transition
    m_waiting_for_button_release = true;
    m_confirm_unlock_until_ms = to_ms_since_boot(get_absolute_time()) + CONFIRM_GUARD_AFTER_PAGE_MS;
}

void Menu::gotoParent(bool do_restore) {
    const auto current_state = m_state_stack.top();

    if (current_state.page == Page::Main) {
        deactivate();  // CRITICAL FIX: Call deactivate() to reset stack
        return;  // Exit early since deactivate() sets m_active = false
    }

    if (do_restore) {
        switch (current_state.page) {
        case Page::DeviceMode:
            m_store->setUsbMode(static_cast<usb_mode_t>(current_state.original_value));
            break;
        case Page::DrumDebounceDelay:
            m_store->setDebounceDelay(current_state.original_value);
            break;
        case Page::DrumPerformanceProfile:
            // Performance Profile feature removed - do nothing
            break;
        case Page::DrumTriggerThresholdKaLeft: {
            auto thresholds = m_store->getTriggerThresholds();
            thresholds.ka_left = current_state.original_value;
            m_store->setTriggerThresholds(thresholds);
        } break;
        case Page::DrumTriggerThresholdDonLeft: {
            auto thresholds = m_store->getTriggerThresholds();
            thresholds.don_left = current_state.original_value;
            m_store->setTriggerThresholds(thresholds);
        } break;
        case Page::DrumTriggerThresholdDonRight: {
            auto thresholds = m_store->getTriggerThresholds();
            thresholds.don_right = current_state.original_value;
            m_store->setTriggerThresholds(thresholds);
        } break;
        case Page::DrumTriggerThresholdKaRight: {
            auto thresholds = m_store->getTriggerThresholds();
            thresholds.ka_right = current_state.original_value;
            m_store->setTriggerThresholds(thresholds);
        } break;
        case Page::LedBrightness:
            m_store->setLedBrightness(current_state.original_value);
            break;
        case Page::Main:
        case Page::DrumTuning:
        case Page::Gameplay:
        case Page::Advanced:
        case Page::DrumTriggerThresholds:
        case Page::DrumTriggerThresholdsManual:
        case Page::DrumTriggerThresholdsAuto:
        case Page::DrumTriggerThresholdsReset:
        case Page::About:
        case Page::Reset:
        case Page::Bootsel:
        case Page::BootselMsg:
        case Page::RebootMsg:
        case Page::TaikoTantrum:
            break;
        }
    }

    m_state_stack.pop();
    m_waiting_for_button_release = true;
    m_confirm_unlock_until_ms = to_ms_since_boot(get_absolute_time()) + CONFIRM_GUARD_AFTER_PAGE_MS;
}

void Menu::performAction(Descriptor::Action action, uint16_t value) {
    switch (action) {
    case Descriptor::Action::None:
        break;
    case Descriptor::Action::GotoParent:
        gotoParent(false);
        break;
    case Descriptor::Action::GotoPageDeviceMode:
        gotoPage(Page::DeviceMode);
        break;
    case Descriptor::Action::GotoPageDrumTuning:
        gotoPage(Page::DrumTuning);
        break;
    case Descriptor::Action::GotoPageGameplay:
        gotoPage(Page::Gameplay);
        break;
    case Descriptor::Action::GotoPageAdvanced:
        gotoPage(Page::Advanced);
        break;
    case Descriptor::Action::GotoPageDrumTriggerThresholds:
        gotoPage(Page::DrumTriggerThresholds);
        break;
    case Descriptor::Action::GotoPageDrumTriggerThresholdsManual:
        gotoPage(Page::DrumTriggerThresholdsManual);
        break;
    case Descriptor::Action::GotoPageDrumTriggerThresholdsAuto:
        gotoPage(Page::DrumTriggerThresholdsAuto);
        break;
    case Descriptor::Action::GotoPageDrumTriggerThresholdsReset:
        gotoPage(Page::DrumTriggerThresholdsReset);
        break;
    case Descriptor::Action::GotoPageAbout:
        gotoPage(Page::About);
        break;
    case Descriptor::Action::GotoPageReset:
        gotoPage(Page::Reset);
        break;
    case Descriptor::Action::GotoPageBootsel:
        gotoPage(Page::Bootsel);
        break;
    case Descriptor::Action::GotoPageDrumDebounceDelay:
        gotoPage(Page::DrumDebounceDelay);
        break;
    case Descriptor::Action::GotoPageDrumPerformanceProfile:
        gotoPage(Page::DrumPerformanceProfile);
        break;
    case Descriptor::Action::GotoPageDrumTriggerThresholdKaLeft:
        gotoPage(Page::DrumTriggerThresholdKaLeft);
        break;
    case Descriptor::Action::GotoPageDrumTriggerThresholdDonLeft:
        gotoPage(Page::DrumTriggerThresholdDonLeft);
        break;
    case Descriptor::Action::GotoPageDrumTriggerThresholdDonRight:
        gotoPage(Page::DrumTriggerThresholdDonRight);
        break;
    case Descriptor::Action::GotoPageDrumTriggerThresholdKaRight:
        gotoPage(Page::DrumTriggerThresholdKaRight);
        break;
    case Descriptor::Action::GotoPageLedBrightness:
        gotoPage(Page::LedBrightness);
        break;
    case Descriptor::Action::GotoPageTaikoTantrum:
        gotoPage(Page::TaikoTantrum);
        break;
    case Descriptor::Action::StartTaikoTantrum:
        // This action is flagged and handled in Main.cpp - don't do anything here
        break;
    case Descriptor::Action::SetUsbMode:
        m_store->setUsbMode(static_cast<usb_mode_t>(value));
        break;
    case Descriptor::Action::SetDrumDebounceDelay:
        m_store->setDebounceDelay(value);
        break;
    case Descriptor::Action::SetPerformanceProfile:
        // Performance Profile feature removed - do nothing
        break;
    case Descriptor::Action::SetDrumTriggerThresholdKaLeft: {
        auto thresholds = m_store->getTriggerThresholds();
        thresholds.ka_left = value;
        m_store->setTriggerThresholds(thresholds);
    } break;
    case Descriptor::Action::SetDrumTriggerThresholdDonLeft: {
        auto thresholds = m_store->getTriggerThresholds();
        thresholds.don_left = value;
        m_store->setTriggerThresholds(thresholds);
    } break;
    case Descriptor::Action::SetDrumTriggerThresholdDonRight: {
        auto thresholds = m_store->getTriggerThresholds();
        thresholds.don_right = value;
        m_store->setTriggerThresholds(thresholds);
    } break;
    case Descriptor::Action::SetDrumTriggerThresholdKaRight: {
        auto thresholds = m_store->getTriggerThresholds();
        thresholds.ka_right = value;
        m_store->setTriggerThresholds(thresholds);
    } break;
    case Descriptor::Action::SetLedBrightness:
        m_store->setLedBrightness(value);
        break;
    case Descriptor::Action::DoReset:
        m_store->reset();
        // Confirmation screen will be shown by Main.cpp which monitors menu state
        gotoPage(Page::RebootMsg);
        break;
    case Descriptor::Action::DoRebootToBootsel:
        m_store->scheduleReboot(true);
        gotoPage(Page::BootselMsg);
        break;
    case Descriptor::Action::DoResetThresholds: {
        // Reset all thresholds to factory defaults (from GlobalConfiguration.h)
        // Using Config::Default::drum_config ensures this always matches the firmware flash default.
        m_store->setTriggerThresholds(Config::Default::drum_config.trigger_thresholds);
        // Confirmation screen will be shown by Main.cpp which monitors menu state
        gotoParent(false);
    } break;
    }
}


void Menu::update(const InputState::Controller &controller_state) {
    State &current_state = m_state_stack.top();
    
    auto descriptor_it = descriptors.find(current_state.page);
    if (descriptor_it == descriptors.end()) {
        assert(false);
        return;
    }

    const bool any_direction_pressed = controller_state.dpad.up || controller_state.dpad.down ||
                                       controller_state.dpad.left || controller_state.dpad.right;
    const bool any_face_pressed = controller_state.buttons.east || controller_state.buttons.south;
    if (m_waiting_for_button_release) {
        if (any_direction_pressed || any_face_pressed) {
            m_buttons.reset();
            return;
        }
        m_waiting_for_button_release = false;
        m_buttons.reset();
        return;
    }

    // Ignore input for a few frames after a page transition or menu open.
    // This prevents the A button press that navigated TO this page from
    // immediately firing again on the new page (caused by releaseAll() in
    // Main.cpp creating a one-frame gap that resets the edge-detect state).
    if (m_ignore_input_frames > 0) {
        m_ignore_input_frames--;
        m_buttons.reset();
        return;
    }

    // Update buttons with page type to enable repeat for Value/UnifiedThresholds
    m_buttons.update(controller_state, descriptor_it->second.type);
    
    // Throttle menu updates to prevent too-fast scrolling
    // EXCEPT for UnifiedThresholds Left/Right adjustments (those should feel responsive)
    static uint32_t last_update_time = 0;
    //static constexpr uint32_t UPDATE_INTERVAL_MS = 16;  // ~60 updates per second
    
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    
// Throttle navigation, but allow value adjustments to be more responsive
bool is_value_adjustment = (descriptor_it->second.type == Descriptor::Type::Value || 
                            descriptor_it->second.type == Descriptor::Type::UnifiedThresholds) &&
                           (m_buttons.getPressed(Buttons::Id::Left) || 
                            m_buttons.getPressed(Buttons::Id::Right));

    uint32_t throttle_time = is_value_adjustment ? 0 : 100;  // No throttle for values, 100ms for navigation (reduced from 150ms)

if (current_time - last_update_time < throttle_time) {
    return;
}

last_update_time = current_time;

    const bool any_action_pressed =
        m_buttons.getPressed(Buttons::Id::Left) ||
        m_buttons.getPressed(Buttons::Id::Right) ||
        m_buttons.getPressed(Buttons::Id::Up) ||
        m_buttons.getPressed(Buttons::Id::Down) ||
        m_buttons.getPressed(Buttons::Id::Back) ||
        m_buttons.getPressed(Buttons::Id::Confirm);

    if (any_action_pressed &&
        (current_time - m_last_input_accept_ms) < INPUT_ACCEPT_DEBOUNCE_MS) {
        return;
    }

    if (m_buttons.getPressed(Buttons::Id::Left) ||
        m_buttons.getPressed(Buttons::Id::Right) ||
        m_buttons.getPressed(Buttons::Id::Up) ||
        m_buttons.getPressed(Buttons::Id::Down)) {
        m_confirm_unlock_until_ms = current_time + CONFIRM_GUARD_AFTER_NAV_MS;
    }

    // RebootInfo pages should just deactivate menu and let the store handle reboot
    if (descriptor_it->second.type == Descriptor::Type::RebootInfo) {
        m_active = false;
        return;
    }
    
    // NEW: RebootCountdown pages trigger the countdown screen in Display
    if (descriptor_it->second.type == Descriptor::Type::RebootCountdown) {
        m_active = false;
        return;
    }
    
    if (m_buttons.getPressed(Buttons::Id::Left)) {
        m_last_input_accept_ms = current_time;
        switch (descriptor_it->second.type) {
        case Descriptor::Type::Toggle:
            current_state.selected_value = current_state.selected_value == 0 ? 1 : 0;
            // Only perform action for non-confirmation toggles (not Bootsel, Reset, or ResetThresholds)
            if (current_state.page != Page::Bootsel && 
                current_state.page != Page::Reset &&
                current_state.page != Page::DrumTriggerThresholdsReset) {
                performAction(descriptor_it->second.items.at(0).second, current_state.selected_value);
            }
            break;
        case Descriptor::Type::Selection:
            if (current_state.selected_value == 0) {
                current_state.selected_value = descriptor_it->second.items.size() - 1;
            } else {
                current_state.selected_value--;
            }
            // Don't apply action for DeviceMode - wait for confirmation
            if (current_state.page != Page::DeviceMode) {
                performAction(descriptor_it->second.items.at(current_state.selected_value).second,
                              current_state.selected_value);
            }
            break;
        case Descriptor::Type::Menu:
            if (current_state.selected_value == 0) {
                current_state.selected_value = descriptor_it->second.items.size() - 1;
            } else {
                current_state.selected_value--;
            }
            break;
        case Descriptor::Type::Value:
            // LEFT = decrease value (horizontal slider)
            if (current_state.selected_value > 0) {
                current_state.selected_value--;
                performAction(descriptor_it->second.items.at(0).second, current_state.selected_value);
            }
            break;
   case Descriptor::Type::UnifiedThresholds: {
    // LEFT = decrease value

    auto thresholds = m_store->getTriggerThresholds();
    
           
    uint16_t* selected_threshold = nullptr;
    switch (current_state.selected_value) {
        case 0: selected_threshold = &thresholds.ka_left; break;
        case 1: selected_threshold = &thresholds.don_left; break;
        case 2: selected_threshold = &thresholds.don_right; break;
        case 3: selected_threshold = &thresholds.ka_right; break;
    }
    if (selected_threshold && *selected_threshold > 0) {
        (*selected_threshold)--;
      
        m_store->setTriggerThresholds(thresholds);
    } else {
       
    }
    break;
}
        case Descriptor::Type::RebootInfo:
        case Descriptor::Type::RebootCountdown:
            break;
        }  // <-- This closes the LEFT button switch
    } else if (m_buttons.getPressed(Buttons::Id::Right)) {
         m_last_input_accept_ms = current_time;
         switch (descriptor_it->second.type) {
        case Descriptor::Type::Toggle:
            current_state.selected_value = current_state.selected_value == 0 ? 1 : 0;
            // Only perform action for non-confirmation toggles (not Bootsel, Reset, or ResetThresholds)
            if (current_state.page != Page::Bootsel && 
                current_state.page != Page::Reset &&
                current_state.page != Page::DrumTriggerThresholdsReset) {
                performAction(descriptor_it->second.items.at(0).second, current_state.selected_value);
            }
            break;
        case Descriptor::Type::Selection:
            if (current_state.selected_value == descriptor_it->second.items.size() - 1) {
                current_state.selected_value = 0;
            } else {
                current_state.selected_value++;
            }
            // Don't apply action for DeviceMode - wait for confirmation
            if (current_state.page != Page::DeviceMode) {
                performAction(descriptor_it->second.items.at(current_state.selected_value).second,
                              current_state.selected_value);
            }
            break;
        case Descriptor::Type::Menu:
            if (current_state.selected_value == descriptor_it->second.items.size() - 1) {
                current_state.selected_value = 0;
            } else {
                current_state.selected_value++;
            }
            break;
        case Descriptor::Type::Value:
            // RIGHT = increase value (horizontal slider)
            if (current_state.selected_value < descriptor_it->second.max_value) {
                current_state.selected_value++;
                performAction(descriptor_it->second.items.at(0).second, current_state.selected_value);
            }
            break;
      case Descriptor::Type::UnifiedThresholds: {
    // RIGHT = increase value
    auto thresholds = m_store->getTriggerThresholds();
    uint16_t* selected_threshold = nullptr;
    switch (current_state.selected_value) {
        case 0: selected_threshold = &thresholds.ka_left; break;
        case 1: selected_threshold = &thresholds.don_left; break;
        case 2: selected_threshold = &thresholds.don_right; break;
        case 3: selected_threshold = &thresholds.ka_right; break;
    }
    if (selected_threshold && *selected_threshold < 4095) {
        (*selected_threshold)++;
        m_store->setTriggerThresholds(thresholds);
    }
    break;
}
        case Descriptor::Type::RebootInfo:
        case Descriptor::Type::RebootCountdown:
            break;
        }
    } else if (m_buttons.getPressed(Buttons::Id::Up)) {
        m_last_input_accept_ms = current_time;
        switch (descriptor_it->second.type) {
        case Descriptor::Type::Value:
            // Values use LEFT/RIGHT, not UP/DOWN
            break;
        case Descriptor::Type::UnifiedThresholds:
            // UP = select previous threshold (wrap around)
            if (current_state.selected_value == 0) {
                current_state.selected_value = 3;
            } else {
                current_state.selected_value--;
            }
            break;
        case Descriptor::Type::Toggle:
        case Descriptor::Type::Selection:
        case Descriptor::Type::Menu:
        case Descriptor::Type::RebootInfo:
        case Descriptor::Type::RebootCountdown:
            break;
        }
    } else if (m_buttons.getPressed(Buttons::Id::Down)) {
        m_last_input_accept_ms = current_time;
        switch (descriptor_it->second.type) {
        case Descriptor::Type::Value:
            // Values use LEFT/RIGHT, not UP/DOWN
            break;
        case Descriptor::Type::UnifiedThresholds:
            // DOWN = select next threshold (wrap around)
            if (current_state.selected_value == 3) {
                current_state.selected_value = 0;
            } else {
                current_state.selected_value++;
            }
            break;
        case Descriptor::Type::Toggle:
        case Descriptor::Type::Selection:
        case Descriptor::Type::Menu:
        case Descriptor::Type::RebootInfo:
        case Descriptor::Type::RebootCountdown:
            break;
        }
    } else if (m_buttons.getPressed(Buttons::Id::Back)) {
        m_last_input_accept_ms = current_time;
        switch (descriptor_it->second.type) {
        case Descriptor::Type::Value:
        case Descriptor::Type::Toggle:
        case Descriptor::Type::Selection:
            gotoParent(true);
            break;
 case Descriptor::Type::UnifiedThresholds:
    // Restore original thresholds and exit page
    m_store->setTriggerThresholds(m_unified_thresholds_original);
    gotoParent(false);
    break;
        case Descriptor::Type::Menu:
            gotoParent(false);
            break;
        case Descriptor::Type::RebootInfo:
        case Descriptor::Type::RebootCountdown:
            break;
        }
    } else if (m_buttons.getPressed(Buttons::Id::Confirm)) {
        m_last_input_accept_ms = current_time;
        if (current_time < m_confirm_unlock_until_ms) {
            return;
        }
        switch (descriptor_it->second.type) {
        case Descriptor::Type::Value:
            gotoParent(false);
            break;
        case Descriptor::Type::Toggle:
            // Special handling for confirmation toggles
            if (current_state.selected_value == 1) {  // User selected Yes
                if (current_state.page == Page::Bootsel) {
                    // Trigger reboot to bootsel
                    m_store->scheduleReboot(true);
                    gotoPage(Page::BootselMsg);
                } else if (current_state.page == Page::Reset) {
                    // Trigger reset settings
                    performAction(Descriptor::Action::DoReset, 1);
                } else if (current_state.page == Page::DrumTriggerThresholdsReset) {
                    // Trigger threshold reset
                    performAction(Descriptor::Action::DoResetThresholds, 1);
                } else {
                    // Normal toggle - already handled by Left/Right, just go back
                    gotoParent(false);
                }
            } else {
                // User selected No - just go back without doing anything
                gotoParent(false);
            }
            break;
        case Descriptor::Type::Selection:
            // Check if this is a Taiko-Tune page before normal handling
            if (current_state.page == Page::TaikoTantrum) {
                // Don't go to parent - stay on this page
                // The calibration start will be handled by Main.cpp
                // Just flag that confirm was pressed
                m_tantrum_start_requested = true;
            } else if (current_state.page == Page::DeviceMode && 
                current_state.selected_value != current_state.original_value) {
                m_store->setUsbMode(static_cast<usb_mode_t>(current_state.selected_value));
                m_store->scheduleReboot();
                gotoPage(Page::RebootMsg);
            } else {
                gotoParent(false);
            }
            break;
        case Descriptor::Type::Menu:
            performAction(descriptor_it->second.items.at(current_state.selected_value).second,
                          current_state.selected_value);
            break;
        case Descriptor::Type::UnifiedThresholds:
    // A button confirms and exits
    gotoParent(false);
    break;
        case Descriptor::Type::RebootInfo:
        case Descriptor::Type::RebootCountdown:
            break;
        }
    }
}

bool Menu::active() const { return m_active; }

Menu::State Menu::getState() const { return m_state_stack.top(); }

bool Menu::isTantrumStartRequested() {
    bool result = m_tantrum_start_requested;
    m_tantrum_start_requested = false; // Clear flag after reading
    return result;
}

} // namespace OuchiTaiko::Utils

// End of file Menu.cpp
