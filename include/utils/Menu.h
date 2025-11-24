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
        Drum,
        Led,
        About,
        Reset,
        Bootsel,

        DrumDebounceDelay,
        DrumTriggerThresholds,
        DrumTriggerThresholdsManual,
        DrumTriggerThresholdsAuto,
        DrumTriggerThresholdsReset,
        DrumBigHitArcade,
        DrumBigHitEnable,
        SimulTap,
        
        // Taiko-Tune pages
        TaikoTuneKaLeft,
        TaikoTuneDonLeft,
        TaikoTuneDonRight,
        TaikoTuneKaRight,
        TaikoTuneAllDrums,
        TaikoTuneResults,

        DrumTriggerThresholdKaLeft,
        DrumTriggerThresholdDonLeft,
        DrumTriggerThresholdDonRight,
        DrumTriggerThresholdKaRight,

        DrumBigHitThreshold,

        LedBrightness,
        LedEnablePlayerColor,

        BootselMsg,
        RebootMsg,
    };

    struct State {
        Page page;
        uint16_t selected_value;
        uint16_t original_value;
    };

    struct Descriptor {
        enum class Type : uint8_t {
            Menu,
            Selection,
            Value,
            Toggle,
            RebootInfo,
            RebootCountdown,  // NEW: For countdown screen
        };

        enum class Action : uint8_t {
            None,
            GotoParent,

            GotoPageDeviceMode,
            GotoPageDrum,
            GotoPageLed,
            GotoPageAbout,
            GotoPageReset,
            GotoPageBootsel,

            GotoPageDrumDebounceDelay,
            GotoPageDrumBigHitArcade,
            GotoPageDrumBigHitEnable,
            GotoPageDrumTriggerThresholds,
            GotoPageDrumTriggerThresholdsManual,
            GotoPageDrumTriggerThresholdsAuto,
            GotoPageDrumTriggerThresholdsReset,
            GotoPageDrumBigHitThreshold,
            GotoPageSimulTap,
            
            // Taiko-Tune actions
            GotoPageTaikoTuneKaLeft,
            GotoPageTaikoTuneDonLeft,
            GotoPageTaikoTuneDonRight,
            GotoPageTaikoTuneKaRight,
            GotoPageTaikoTuneAllDrums,
            StartTaikoTuneAnalysis,
            ApplyTaikoTuneRecommendation,

            GotoPageDrumTriggerThresholdKaLeft,
            GotoPageDrumTriggerThresholdDonLeft,
            GotoPageDrumTriggerThresholdDonRight,
            GotoPageDrumTriggerThresholdKaRight,

            GotoPageLedBrightness,
            GotoPageLedEnablePlayerColor,

            SetUsbMode,

            SetDrumDebounceDelay,

            SetBigHitEnable,

            SetDrumTriggerThresholdKaLeft,
            SetDrumTriggerThresholdDonLeft,
            SetDrumTriggerThresholdDonRight,
            SetDrumTriggerThresholdKaRight,

            SetDrumBigHitThreshold,

            SetLedBrightness,
            SetLedEnablePlayerColor,

            SetSimulTap,

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

        void update(const InputState::Controller &state);
        void reset();  // NEW: Reset all button states
        [[nodiscard]] bool getPressed(Id id) const;
    };

    std::shared_ptr<SettingsStore> m_store;
    Buttons m_buttons;
    bool m_active{false};
    bool m_taikotune_start_requested{false};
    Page m_taikotune_requested_page{Page::Main};
    
    // NEW: Ignore inputs for first few frames after activation
    uint8_t m_ignore_input_frames{0};
    static constexpr uint8_t IGNORE_FRAMES_ON_ACTIVATE = 10;  // Ignore first 10 frames (~160ms)

    std::stack<State> m_state_stack{{{.page = Page::Main, .selected_value = 0, .original_value = 0}}};
    
    // NEW: Wait for ALL buttons to be released before processing any input
    bool m_waiting_for_button_release{false};

    uint16_t getCurrentValue(Page page);
    void gotoPage(Page page);
    void gotoParent(bool do_restore);

    void performAction(Descriptor::Action action, uint16_t value);

  public:
    Menu(std::shared_ptr<SettingsStore> settings_store);

    void activate();
    void setWaitingForButtonRelease(bool waiting);  // NEW: Set initial state
    void deactivate();
    void update(const InputState::Controller &controller_state);
    [[nodiscard]] bool active() const;
    [[nodiscard]] State getState() const;
    [[nodiscard]] bool isTaikoTuneStartRequested();
    [[nodiscard]] Page getTaikoTuneRequestedPage() const;
};
} // namespace OuchiTaiko::Utils

#endif // UTILS_MENU_H_

// End of file Menu.h