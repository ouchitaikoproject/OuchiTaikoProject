// Beginning of file Menu.cpp

#include "utils/Menu.h"

#include "peripherals/Drum.h"

namespace OuchiTaiko::Utils {

const std::map<Menu::Page, const Menu::Descriptor> Menu::descriptors = {
    {Menu::Page::Main,
     {Menu::Descriptor::Type::Menu,
      "Settings",
      {{"Controller\nModes", Menu::Descriptor::Action::GotoPageDeviceMode},
       {"Drum\nSettings", Menu::Descriptor::Action::GotoPageDrum},
       {"LED\nSettings", Menu::Descriptor::Action::GotoPageLed},
       {"About", Menu::Descriptor::Action::GotoPageAbout},
       {"Reset", Menu::Descriptor::Action::GotoPageReset},
       {"USB Flash\nMode", Menu::Descriptor::Action::GotoPageBootsel}},
      0}},

    {Menu::Page::DeviceMode,
     {Menu::Descriptor::Type::Selection,
      "Device\nMode",
      {{"Nintendo Swtch\nTatacon Drum", Menu::Descriptor::Action::SetUsbMode},
       {"Nintendo Swtch\nPro Controller", Menu::Descriptor::Action::SetUsbMode},
       {"Sony PS3\nDualshock3", Menu::Descriptor::Action::SetUsbMode},
       {"Sony PS4\nTatacon Drum", Menu::Descriptor::Action::SetUsbMode},
       {"Sony PS4\nDualshock4", Menu::Descriptor::Action::SetUsbMode},
       {"Keyboard\nPlayer 1", Menu::Descriptor::Action::SetUsbMode},
       {"Keyboard\nPlayer 2", Menu::Descriptor::Action::SetUsbMode},
       {"Microsoft Xbox\nXbox360", Menu::Descriptor::Action::SetUsbMode},
       {"Android\n(XInput)", Menu::Descriptor::Action::SetUsbMode},
       {"iOS\n(XInput)", Menu::Descriptor::Action::SetUsbMode},
       {"Analog\nPlayer 1", Menu::Descriptor::Action::SetUsbMode},
       {"Analog\nPlayer 2", Menu::Descriptor::Action::SetUsbMode},
       {"MIDI\nController", Menu::Descriptor::Action::SetUsbMode},
       {"Debug\nMode", Menu::Descriptor::Action::SetUsbMode}},
      0}},

    {Menu::Page::Drum,
     {Menu::Descriptor::Type::Menu,
      "Drum\nSettings",
      {{"Drum\nThresholds", Menu::Descriptor::Action::GotoPageDrumTriggerThresholds},
       {"Performance\nProfile", Menu::Descriptor::Action::GotoPageDrumPerformanceProfile},
       {"Big Hit\nArcade Mode", Menu::Descriptor::Action::GotoPageDrumBigHitArcade},
       {"SimulTap\nMode", Menu::Descriptor::Action::GotoPageSimulTap},
       {"Hold\nTime", Menu::Descriptor::Action::GotoPageDrumDebounceDelay}},
      0}},

    {Menu::Page::DrumTriggerThresholds,
     {Menu::Descriptor::Type::Menu,
      "Thresholds",
      {{"Auto\nTaiko-Tune", Menu::Descriptor::Action::GotoPageDrumTriggerThresholdsAuto},
       {"Manual\nPad Adjust", Menu::Descriptor::Action::GotoPageDrumTriggerThresholdsManual},
       {"Reset\nThresholds", Menu::Descriptor::Action::GotoPageDrumTriggerThresholdsReset}},
      0}},

    {Menu::Page::DrumTriggerThresholdsManual,
     {Menu::Descriptor::Type::Menu,
      "Manual\nAdjust",
      {{"Left Ka", Menu::Descriptor::Action::GotoPageDrumTriggerThresholdKaLeft},
       {"Left Don", Menu::Descriptor::Action::GotoPageDrumTriggerThresholdDonLeft},
       {"Right Don", Menu::Descriptor::Action::GotoPageDrumTriggerThresholdDonRight},
       {"Right Ka", Menu::Descriptor::Action::GotoPageDrumTriggerThresholdKaRight}},
      0}},

    {Menu::Page::DrumTriggerThresholdsAuto,
     {Menu::Descriptor::Type::Menu,
      "Auto\nTaiko-Tune",
      {{"Analyze All\n4 Drums", Menu::Descriptor::Action::GotoPageTaikoTuneAllDrums},
       {"Analyze\nLeft Ka", Menu::Descriptor::Action::GotoPageTaikoTuneKaLeft},
       {"Analyze\nLeft Don", Menu::Descriptor::Action::GotoPageTaikoTuneDonLeft},
       {"Analyze\nRight Don", Menu::Descriptor::Action::GotoPageTaikoTuneDonRight},
       {"Analyze\nRight Ka", Menu::Descriptor::Action::GotoPageTaikoTuneKaRight}},
      0}},

    {Menu::Page::DrumTriggerThresholdsReset,
    {Menu::Descriptor::Type::Toggle,
     "Reset\nAll?",
     {{"Yes / No", Menu::Descriptor::Action::DoResetThresholds}},
     0}},

    {Menu::Page::DrumBigHitArcade,
     {Menu::Descriptor::Type::Selection,
      "Big Hit Mode",
      {{"Off", Menu::Descriptor::Action::SetBigHitEnable},
       {"Light (2000)", Menu::Descriptor::Action::SetBigHitEnable},
       {"Medium (2500)", Menu::Descriptor::Action::SetBigHitEnable},
       {"Heavy (3000)", Menu::Descriptor::Action::SetBigHitEnable},
       {"Custom...", Menu::Descriptor::Action::GotoPageDrumBigHitThreshold}},
      0}},

    {Menu::Page::SimulTap,
     {Menu::Descriptor::Type::Toggle,
      "SimulTap\nMode",
      {{"", Menu::Descriptor::Action::SetSimulTap}},
      0}},

    {Menu::Page::TaikoTuneKaLeft,
     {Menu::Descriptor::Type::Selection,
      "Analyzing L-Ka",
      {{"Start Analysis?", Menu::Descriptor::Action::StartTaikoTuneAnalysis}},
      0}},

    {Menu::Page::TaikoTuneDonLeft,
     {Menu::Descriptor::Type::Selection,
      "Analyzing L-Don",
      {{"Start Analysis?", Menu::Descriptor::Action::StartTaikoTuneAnalysis}},
      0}},

    {Menu::Page::TaikoTuneDonRight,
     {Menu::Descriptor::Type::Selection,
      "Analyzing R-Don",
      {{"Start Analysis?", Menu::Descriptor::Action::StartTaikoTuneAnalysis}},
      0}},

    {Menu::Page::TaikoTuneKaRight,
     {Menu::Descriptor::Type::Selection,
      "Analyzing R-Ka",
      {{"Start Analysis?", Menu::Descriptor::Action::StartTaikoTuneAnalysis}},
      0}},

    {Menu::Page::TaikoTuneAllDrums,
     {Menu::Descriptor::Type::Selection,
      "All 4 Drums",
      {{"Start Analysis?", Menu::Descriptor::Action::StartTaikoTuneAnalysis}},
      0}},

    {Menu::Page::TaikoTuneResults,
     {Menu::Descriptor::Type::RebootInfo,
      "Results",
      {{"Auto-Applied", Menu::Descriptor::Action::None}},
      0}},

    {Menu::Page::DrumDebounceDelay,
     {Menu::Descriptor::Type::Value,
      "Hit Hold Time (ms)",
      {{"", Menu::Descriptor::Action::SetDrumDebounceDelay}},
      UINT8_MAX}},

    {Menu::Page::DrumPerformanceProfile,
     {Menu::Descriptor::Type::Selection,
      "Performance\nProfile",
      {{"Standard\n(25ms Safe)", Menu::Descriptor::Action::SetPerformanceProfile},
       {"Competitive\n(12ms Fast)", Menu::Descriptor::Action::SetPerformanceProfile},
       {"EXTREME\n(8ms 120rps)", Menu::Descriptor::Action::SetPerformanceProfile}},
      0}},

    {Menu::Page::DrumTriggerThresholdKaLeft,
     {Menu::Descriptor::Type::Value,
      "Trigger Lvl L. Ka",
      {{"", Menu::Descriptor::Action::SetDrumTriggerThresholdKaLeft}},
      4095}},

    {Menu::Page::DrumTriggerThresholdDonLeft,
     {Menu::Descriptor::Type::Value,
      "Trigger Lvl L. Don",
      {{"", Menu::Descriptor::Action::SetDrumTriggerThresholdDonLeft}},
      4095}},

    {Menu::Page::DrumTriggerThresholdDonRight,
     {Menu::Descriptor::Type::Value,
      "Trigger Lvl R. Don",
      {{"", Menu::Descriptor::Action::SetDrumTriggerThresholdDonRight}},
      4095}},

    {Menu::Page::DrumTriggerThresholdKaRight,
     {Menu::Descriptor::Type::Value,
      "Trigger Lvl R. Ka",
      {{"", Menu::Descriptor::Action::SetDrumTriggerThresholdKaRight}},
      4095}},

    {Menu::Page::DrumBigHitThreshold,
     {Menu::Descriptor::Type::Value,
      "Big Hit Threshold",
      {{"", Menu::Descriptor::Action::SetDrumBigHitThreshold}},
      4095}},

    {Menu::Page::Led,
     {Menu::Descriptor::Type::Menu,
      "LED\nSettings",
      {{"Brightness", Menu::Descriptor::Action::GotoPageLedBrightness},
       {"Player\nColor", Menu::Descriptor::Action::GotoPageLedEnablePlayerColor}},
      0}},

    {Menu::Page::LedBrightness,
     {Menu::Descriptor::Type::Value,
      "LED\nBrightness",
      {{"", Menu::Descriptor::Action::SetLedBrightness}},
      UINT8_MAX}},

    {Menu::Page::LedEnablePlayerColor,
     {Menu::Descriptor::Type::Toggle,
      "Player\nColor (PS4)",
      {{"", Menu::Descriptor::Action::SetLedEnablePlayerColor}},
      0}},

   {Menu::Page::Reset,
    {Menu::Descriptor::Type::Toggle,
     "Reset\nSettings?",
     {{"Yes / No", Menu::Descriptor::Action::DoReset}},
     0}},

    {Menu::Page::Bootsel,
     {Menu::Descriptor::Type::Toggle,
      "Reboot\nto Flash",
      {{"Yes / No", Menu::Descriptor::Action::DoRebootToBootsel}},
      0}},

    {Menu::Page::About,
     {Menu::Descriptor::Type::Selection,
      "About",
      {{"OuchiTaiko Project\nby KillerQ", Menu::Descriptor::Action::None},
       {"Full Guide & Info:\nouchitaiko.com", Menu::Descriptor::Action::None},
       {"Hybrid v11.0\nDecember 2025", Menu::Descriptor::Action::None},
       {"Based on:\nDonCon2040 (MIT)", Menu::Descriptor::Action::None},
       {"& HIDtaiko\n(Apache 2.0)", Menu::Descriptor::Action::None}},
      0}},
      
    {Menu::Page::BootselMsg,
     {Menu::Descriptor::Type::RebootCountdown,
      "Entering Bootloader",
        {{"Countdown", Menu::Descriptor::Action::None}},
  0}},

    {Menu::Page::RebootMsg,
     {Menu::Descriptor::Type::RebootCountdown,
      "Applying Changes...",
      {{"Reboot Countdown", Menu::Descriptor::Action::None}},
      0}},
};

Menu::Buttons::Buttons()
    : m_states({{Id::Up, {}}, {Id::Down, {}}, {Id::Left, {}}, {Id::Right, {}}, {Id::Confirm, {}}, {Id::Back, {}}}) {}

void Menu::Buttons::update(const InputState::Controller &controller_state) {
    // SIMPLIFIED: Pure edge detection - press once, fire once
    // No repeat funactionality at all
    auto handle_button = [](State &button_state, bool input_state) {
        if (input_state && button_state.repeat == State::Repeat::Idle) {
            // Button just pressed (was idle, now pressed)
            button_state.pressed = true;
            button_state.repeat = State::Repeat::RepeatDelay; // Mark as "held" to prevent re-trigger
        } else if (!input_state) {
            // Button released - reset to idle
            button_state.pressed = false;
            button_state.repeat = State::Repeat::Idle;
        } else {
            // Button still held - don't fire again
            button_state.pressed = false;
        }
    };

    handle_button(m_states.at(Id::Up), controller_state.dpad.up);
    handle_button(m_states.at(Id::Down), controller_state.dpad.down);
    handle_button(m_states.at(Id::Left), controller_state.dpad.left);
    handle_button(m_states.at(Id::Right), controller_state.dpad.right);
    handle_button(m_states.at(Id::Confirm), controller_state.buttons.east);
    handle_button(m_states.at(Id::Back), controller_state.buttons.south);
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

void Menu::activate() {
    m_active = true;
    m_buttons.reset();  // Start with clean button state
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
}


uint16_t Menu::getCurrentValue(Menu::Page page) {
    switch (page) {
    case Page::DeviceMode:
        return static_cast<uint16_t>(m_store->getUsbMode());
    case Page::DrumDebounceDelay:
        return m_store->getDebounceDelay();
    case Page::DrumPerformanceProfile:
        return static_cast<uint16_t>(m_store->getPerformanceProfile());
    case Page::DrumTriggerThresholdKaLeft:
        return m_store->getTriggerThresholds().ka_left;
    case Page::DrumTriggerThresholdDonLeft:
        return m_store->getTriggerThresholds().don_left;
    case Page::DrumTriggerThresholdDonRight:
        return m_store->getTriggerThresholds().don_right;
    case Page::DrumTriggerThresholdKaRight:
        return m_store->getTriggerThresholds().ka_right;
    case Page::DrumBigHitThreshold:
        return m_store->getBigHitThreshold();
    case Page::DrumBigHitEnable:
        return static_cast<uint16_t>(m_store->getBigHitEnable());
    case Page::LedBrightness:
        return m_store->getLedBrightness();
    case Page::LedEnablePlayerColor:
        return static_cast<uint16_t>(m_store->getLedEnablePlayerColor());
    case Page::SimulTap:
        return static_cast<uint16_t>(m_store->getSimulTap());
    case Page::Main:
    case Page::Drum:
    case Page::DrumTriggerThresholds:
    case Page::DrumTriggerThresholdsManual:
    case Page::DrumTriggerThresholdsAuto:
    case Page::DrumTriggerThresholdsReset:
    case Page::DrumBigHitArcade: {
        // Return selection index based on current settings
        bool enabled = m_store->getBigHitEnable();
        uint16_t threshold = m_store->getBigHitThreshold();
        
        if (!enabled) {
            return 0;  // Off
        } else if (threshold == 2000) {
            return 1;  // Light
        } else if (threshold == 2500) {
            return 2;  // Medium
        } else if (threshold == 3000) {
            return 3;  // Heavy
        } else {
            return 4;  // Custom (any other value)
        }
    }
    case Page::Led:
    case Page::About:
    case Page::Reset:
    case Page::Bootsel:
    case Page::BootselMsg:
    case Page::RebootMsg:
    case Page::TaikoTuneKaLeft:
    case Page::TaikoTuneDonLeft:
    case Page::TaikoTuneDonRight:
    case Page::TaikoTuneKaRight:
    case Page::TaikoTuneAllDrums:
    case Page::TaikoTuneResults:
        break;
    }

    return 0;
}

void Menu::gotoPage(Menu::Page page) {
    const auto current_value = getCurrentValue(page);

    m_state_stack.push({page, current_value, current_value});
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
            m_store->setPerformanceProfile(static_cast<Peripherals::Drum::PerformanceProfile>(current_state.original_value));
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
        case Page::DrumBigHitThreshold:
            m_store->setBigHitThreshold(current_state.original_value);
            break;
        case Page::DrumBigHitEnable:
            m_store->setBigHitEnable(static_cast<bool>(current_state.original_value));
            break;
        case Page::LedBrightness:
            m_store->setLedBrightness(current_state.original_value);
            break;
        case Page::LedEnablePlayerColor:
            m_store->setLedEnablePlayerColor(static_cast<bool>(current_state.original_value));
            break;
        case Page::SimulTap:
            m_store->setSimulTap(static_cast<bool>(current_state.original_value));
            break;
        case Page::Main:
        case Page::Drum:
        case Page::DrumTriggerThresholds:
        case Page::DrumTriggerThresholdsManual:
        case Page::DrumTriggerThresholdsAuto:
        case Page::DrumTriggerThresholdsReset:
        case Page::DrumBigHitArcade:
        case Page::Led:
        case Page::About:
        case Page::Reset:
        case Page::Bootsel:
        case Page::BootselMsg:
        case Page::RebootMsg:
        case Page::TaikoTuneKaLeft:
        case Page::TaikoTuneDonLeft:
        case Page::TaikoTuneDonRight:
        case Page::TaikoTuneKaRight:
        case Page::TaikoTuneAllDrums:
        case Page::TaikoTuneResults:
            break;
        }
    }

    m_state_stack.pop();
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
    case Descriptor::Action::GotoPageDrum:
        gotoPage(Page::Drum);
        break;
    case Descriptor::Action::GotoPageDrumBigHitArcade:
        gotoPage(Page::DrumBigHitArcade);
        break;
    case Descriptor::Action::GotoPageDrumBigHitEnable:
        gotoPage(Page::DrumBigHitEnable);
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
    case Descriptor::Action::GotoPageDrumBigHitThreshold:
        gotoPage(Page::DrumBigHitThreshold);
        break;
    case Descriptor::Action::GotoPageLed:
        gotoPage(Page::Led);
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
    case Descriptor::Action::GotoPageLedEnablePlayerColor:
        gotoPage(Page::LedEnablePlayerColor);
        break;
    case Descriptor::Action::GotoPageSimulTap:
        gotoPage(Page::SimulTap);
        break;
    case Descriptor::Action::GotoPageTaikoTuneKaLeft:
        gotoPage(Page::TaikoTuneKaLeft);
        break;
    case Descriptor::Action::GotoPageTaikoTuneDonLeft:
        gotoPage(Page::TaikoTuneDonLeft);
        break;
    case Descriptor::Action::GotoPageTaikoTuneDonRight:
        gotoPage(Page::TaikoTuneDonRight);
        break;
    case Descriptor::Action::GotoPageTaikoTuneKaRight:
        gotoPage(Page::TaikoTuneKaRight);
        break;
    case Descriptor::Action::GotoPageTaikoTuneAllDrums:
        gotoPage(Page::TaikoTuneAllDrums);
        break;
    case Descriptor::Action::StartTaikoTuneAnalysis:
        // This action is flagged and handled in Main.cpp - don't do anything here
        break;
    case Descriptor::Action::ApplyTaikoTuneRecommendation:
        // This will be handled by the main loop
        break;
    case Descriptor::Action::SetUsbMode:
        m_store->setUsbMode(static_cast<usb_mode_t>(value));
        break;
    case Descriptor::Action::SetDrumDebounceDelay:
        m_store->setDebounceDelay(value);
        break;
    case Descriptor::Action::SetPerformanceProfile:
        m_store->setPerformanceProfile(static_cast<Peripherals::Drum::PerformanceProfile>(value));
        break;
    case Descriptor::Action::SetBigHitEnable:
        // Handle Big Hit Arcade Mode preset selections
        switch (value) {
        case 0:  // Off
            m_store->setBigHitEnable(false);
            break;
        case 1:  // Light (2000)
            m_store->setBigHitEnable(true);
            m_store->setBigHitThreshold(2000);
            break;
        case 2:  // Medium (2500)
            m_store->setBigHitEnable(true);
            m_store->setBigHitThreshold(2500);
            break;
        case 3:  // Heavy (3000)
            m_store->setBigHitEnable(true);
            m_store->setBigHitThreshold(3000);
            break;
        case 4:  // Custom - handled by going to threshold page
            // Don't change anything here, just let the navigation happen
            break;
        }
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
    case Descriptor::Action::SetDrumBigHitThreshold:
        m_store->setBigHitThreshold(value);
        break;
    case Descriptor::Action::SetLedBrightness:
        m_store->setLedBrightness(value);
        break;
    case Descriptor::Action::SetLedEnablePlayerColor:
        m_store->setLedEnablePlayerColor(static_cast<bool>(value));
        break;
    case Descriptor::Action::SetSimulTap:
        m_store->setSimulTap(static_cast<bool>(value));
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
        // Reset all thresholds to factory defaults (50 for all drums)
        Peripherals::Drum::Config::Thresholds default_thresholds = {
            .don_left = 75,
            .ka_left = 95,
            .don_right = 75,
            .ka_right = 95,
        };
        m_store->setTriggerThresholds(default_thresholds);
        // Confirmation screen will be shown by Main.cpp which monitors menu state
        gotoParent(false);
    } break;
    }
}

void Menu::update(const InputState::Controller &controller_state) {
    m_buttons.update(controller_state);

    State &current_state = m_state_stack.top();

    auto descriptor_it = descriptors.find(current_state.page);
    if (descriptor_it == descriptors.end()) {
        assert(false);
        return;
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
            // Don't apply action for DeviceMode and DrumBigHitArcade - wait for confirmation
            if (current_state.page != Page::DeviceMode && 
                current_state.page != Page::DrumBigHitArcade) {
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
        case Descriptor::Type::RebootInfo:
        case Descriptor::Type::RebootCountdown:
            break;
        }
    } else if (m_buttons.getPressed(Buttons::Id::Right)) {
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
            // Don't apply action for DeviceMode and DrumBigHitArcade - wait for confirmation
            if (current_state.page != Page::DeviceMode && 
                current_state.page != Page::DrumBigHitArcade) {
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
        case Descriptor::Type::RebootInfo:
        case Descriptor::Type::RebootCountdown:
            break;
        }
    } else if (m_buttons.getPressed(Buttons::Id::Up)) {
        switch (descriptor_it->second.type) {
        case Descriptor::Type::Value:
            if (current_state.selected_value < descriptor_it->second.max_value) {
                current_state.selected_value++;
                performAction(descriptor_it->second.items.at(0).second, current_state.selected_value);
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
        switch (descriptor_it->second.type) {
        case Descriptor::Type::Value:
            if (current_state.selected_value > 0) {
                current_state.selected_value--;
                performAction(descriptor_it->second.items.at(0).second, current_state.selected_value);
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
        switch (descriptor_it->second.type) {
        case Descriptor::Type::Value:
        case Descriptor::Type::Toggle:
        case Descriptor::Type::Selection:
            gotoParent(true);
            break;
        case Descriptor::Type::Menu:
            gotoParent(false);
            break;
        case Descriptor::Type::RebootInfo:
        case Descriptor::Type::RebootCountdown:
            break;
        }
    } else if (m_buttons.getPressed(Buttons::Id::Confirm)) {
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
            if (current_state.page == Page::TaikoTuneKaLeft ||
                current_state.page == Page::TaikoTuneDonLeft ||
                current_state.page == Page::TaikoTuneDonRight ||
                current_state.page == Page::TaikoTuneKaRight ||
                current_state.page == Page::TaikoTuneAllDrums) {
                // Store which page was confirmed for Main.cpp to check
                m_taikotune_requested_page = current_state.page;
                // Don't go to parent - stay on this page
                // The analysis start will be handled by Main.cpp
                // Just flag that confirm was pressed
                m_taikotune_start_requested = true;
            } else if (current_state.page == Page::DeviceMode && 
                current_state.selected_value != current_state.original_value) {
                m_store->setUsbMode(static_cast<usb_mode_t>(current_state.selected_value));
                m_store->scheduleReboot();
                gotoPage(Page::RebootMsg);
            } else if (current_state.page == Page::DrumBigHitArcade) {
                // Apply Big Hit Mode selection
                if (current_state.selected_value == 4) {
                    // "Custom..." selected - navigate to threshold page
                    // First enable Big Hit mode if it's off
                    if (!m_store->getBigHitEnable()) {
                        m_store->setBigHitEnable(true);
                    }
                    performAction(descriptor_it->second.items.at(current_state.selected_value).second,
                                  current_state.selected_value);
                } else {
                    // Apply the preset (Off/Light/Medium/Heavy)
                    performAction(descriptor_it->second.items.at(current_state.selected_value).second,
                                  current_state.selected_value);
                    // Update original_value to reflect the new setting, stay on page
                    current_state.original_value = current_state.selected_value;
                    // Don't go back - stay on the page to show (Current Mode)
                }
            } else {
                gotoParent(false);
            }
            break;
        case Descriptor::Type::Menu:
            performAction(descriptor_it->second.items.at(current_state.selected_value).second,
                          current_state.selected_value);
            break;
        case Descriptor::Type::RebootInfo:
        case Descriptor::Type::RebootCountdown:
            break;
        }
    }
}

bool Menu::active() const { return m_active; }

Menu::State Menu::getState() const { return m_state_stack.top(); }

bool Menu::isTaikoTuneStartRequested() {
    bool result = m_taikotune_start_requested;
    m_taikotune_start_requested = false; // Clear flag after reading
    return result;
}

Menu::Page Menu::getTaikoTuneRequestedPage() const {
    return m_taikotune_requested_page;
}

} // namespace OuchiTaiko::Utils

// End of file Menu.cpp