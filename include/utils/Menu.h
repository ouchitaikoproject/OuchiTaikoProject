// Beginning of file Menu.h

#ifndef UTILS_MENU_H_
#define UTILS_MENU_H_

#include "utils/InputState.h"
#include "utils/SettingsStore.h"

#include <map>
#include <memory>
#include <stack>
#include <string>
#include <vector>

namespace OuchiTaiko::Utils {

class Menu {
  public:
    enum class Page : uint8_t {
        Main,

        DeviceMode,
        DrumTuning,
        Gameplay,  // NEW: Gameplay Mode Mods submenu
        Advanced,
        About,
        Reset,
        Bootsel,

        DrumDebounceDelay,
        DrumPerformanceProfile,
        DrumTriggerThresholds,
        DrumTriggerThresholdsManual,
        DrumTriggerThresholdsAuto,
        DrumTriggerThresholdsReset,

        // Taiko Tantrum calibration
        TaikoTantrum,

        DrumTriggerThresholdKaLeft,
        DrumTriggerThresholdDonLeft,
        DrumTriggerThresholdDonRight,
        DrumTriggerThresholdKaRight,

        LedBrightness,

        BootselMsg,
        RebootMsg,
    };

    struct State {
        Page page;
        uint16_t selected_value;
        uint16_t original_value;
        bool editing_value = false; 
    };

    struct Descriptor {
        enum class Type : uint8_t {
            Menu,
            Selection,
            Value,
            Toggle,
            RebootInfo,
            RebootCountdown,  // NEW: For countdown screen
            UnifiedThresholds,  // NEW: For unified threshold tuning page
        };

        enum class Action : uint8_t {
            None,
            GotoParent,

            GotoPageDeviceMode,
            GotoPageDrumTuning,
            GotoPageGameplay,  // NEW: Goto Gameplay submenu
            GotoPageAdvanced,
            GotoPageAbout,
            GotoPageReset,
            GotoPageBootsel,

            GotoPageDrumDebounceDelay,
            GotoPageDrumPerformanceProfile,
            GotoPageDrumTriggerThresholds,
            GotoPageDrumTriggerThresholdsManual,
            GotoPageDrumTriggerThresholdsAuto,
            GotoPageDrumTriggerThresholdsReset,

            // Taiko Tantrum calibration actions
            GotoPageTaikoTantrum,
            StartTaikoTantrum,

            GotoPageDrumTriggerThresholdKaLeft,
            GotoPageDrumTriggerThresholdDonLeft,
            GotoPageDrumTriggerThresholdDonRight,
            GotoPageDrumTriggerThresholdKaRight,

            GotoPageLedBrightness,

            SetUsbMode,

            SetDrumDebounceDelay,
            SetPerformanceProfile,

            SetDrumTriggerThresholdKaLeft,
            SetDrumTriggerThresholdDonLeft,
            SetDrumTriggerThresholdDonRight,
            SetDrumTriggerThresholdKaRight,

            SetLedBrightness,

            DoReset,
            DoRebootToBootsel,
            DoResetThresholds,
        };

        Type type;
        std::string name;
        std::vector<std::pair<std::string, Action>> items;
        uint16_t max_value;
    };

    const static std::map<Page, const Descriptor> descriptors;

  private:
    class Buttons {
      public:
        enum class Id : uint8_t { Up, Down, Left, Right, Confirm, Back };

      private:
        struct State {
            enum class Repeat : uint8_t {
                Idle,
                RepeatDelay,
                Repeat,
                FastRepeat,
            };
            Repeat repeat;
            uint32_t pressed_since;
            uint32_t last_repeat;
            bool pressed;
        };

        std::map<Id, State> m_states;

      public:
        Buttons();

        void update(const InputState::Controller &state, Descriptor::Type page_type);
        void reset();  // NEW: Reset all button states
        [[nodiscard]] bool getPressed(Id id) const;
    };

    std::shared_ptr<SettingsStore> m_store;
    Buttons m_buttons;
    bool m_active{false};
    bool m_tantrum_start_requested{false};
    
    // NEW: Ignore inputs for first few frames after activation
    uint8_t m_ignore_input_frames{0};
    static constexpr uint8_t IGNORE_FRAMES_ON_ACTIVATE = 10;  // Ignore first 10 frames (~160ms)

    std::stack<State> m_state_stack{{{.page = Page::Main, .selected_value = 0, .original_value = 0}}};

    // NEW: Wait for ALL buttons to be released before processing any input
    bool m_waiting_for_button_release{false};
    uint32_t m_confirm_unlock_until_ms{0};
    uint32_t m_last_input_accept_ms{0};
    static constexpr uint32_t CONFIRM_GUARD_AFTER_NAV_MS = 220;
    static constexpr uint32_t CONFIRM_GUARD_AFTER_PAGE_MS = 300;
    static constexpr uint32_t INPUT_ACCEPT_DEBOUNCE_MS = 120;

    // Unified threshold page: Store original values for cancel operation
    Peripherals::Drum::Config::Thresholds m_unified_thresholds_original{};

    uint16_t getCurrentValue(Page page);
    void gotoPage(Page page);
    void gotoParent(bool do_restore);

    void performAction(Descriptor::Action action, uint16_t value);

  public:
    Menu(std::shared_ptr<SettingsStore> settings_store);

    void activate();
    void setWaitingForButtonRelease(bool waiting);  // NEW: Set initial state
    void enterBootloaderSplash();  // Trigger BootselMsg splash from outside menu (e.g. HOME hold)
    void deactivate();
    void goBackToParent();  // NEW: Navigate back one level without restoring values
    void update(const InputState::Controller &controller_state);
    [[nodiscard]] bool active() const;
    [[nodiscard]] State getState() const;
    [[nodiscard]] bool isTantrumStartRequested();
};
} // namespace OuchiTaiko::Utils

#endif // UTILS_MENU_H_

// End of file Menu.h
