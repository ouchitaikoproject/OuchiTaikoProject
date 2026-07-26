// Beginning of file Drum.h

#ifndef PERIPHERALS_DRUM_H_
#define PERIPHERALS_DRUM_H_

#include "utils/InputState.h"
#include "usb/device_driver.h"

#include "hardware/spi.h"
#include "pico/time.h"

#include <array>
#include <cstddef>
#include <memory>
#include <stdint.h>
#include <variant>

namespace OuchiTaiko::Peripherals {

class Drum {
  public:
    enum class Id {
        DON_LEFT  = 0,
        KA_LEFT   = 1,
        DON_RIGHT = 2,
        KA_RIGHT  = 3,
    };

    // Converts Id enum to array index (0-3)
    static constexpr uint8_t idToIndex(Id id) { return static_cast<uint8_t>(id); }

    struct Config {
        struct Thresholds {
            uint16_t don_left;
            uint16_t ka_left;
            uint16_t don_right;
            uint16_t ka_right;
        };

        struct AdcChannels {
            uint8_t don_left;
            uint8_t ka_left;
            uint8_t don_right;
            uint8_t ka_right;
        };

        struct InternalAdc {
            uint8_t sample_count;
        };

        struct ExternalAdc {
            spi_inst_t *spi_block;
            uint spi_speed_hz;
            uint8_t spi_mosi_pin;
            uint8_t spi_miso_pin;
            uint8_t spi_sclk_pin;
            uint8_t spi_scsn_pin;
            uint8_t spi_level_shifter_enable_pin;
        };

        Thresholds trigger_thresholds;
        uint16_t hit_hold_ms;
        uint16_t analog_peak_hold_ms;
        uint32_t roll_counter_timeout_ms;
        float analog_gain;  // Analog mode gain multiplier (compensates for missing OpAmp circuit)
        AdcChannels adc_channels;
        std::variant<InternalAdc, ExternalAdc> adc_config;
    };

    // ============================================================================
    // GUIDED CALIBRATE - per-pad isolate / correct / retry workflow
    //
    // Flow per pad:
    //   - 5 normal hits
    //   - 1 hard hit
    //   - watch the other 3 pads for bleed
    //   - raise only the offending thresholds by +5
    //   - repeat the same pad until clean
    // ============================================================================

    struct GuidedCalState {
        static constexpr uint8_t PAD_ORDER[4] = {1, 0, 2, 3};
        static constexpr const char* PAD_NAMES[4] = {"Ka Left", "Don Left", "Don Right", "Ka Right"};
        static constexpr const char* PAD_INDEX_NAMES[4] = {"Don Left", "Ka Left", "Don Right", "Ka Right"};
        static constexpr uint16_t START_THRESHOLD_KA_LEFT = 265;
        static constexpr uint16_t START_THRESHOLD_DON_LEFT = 190;
        static constexpr uint16_t START_THRESHOLD_DON_RIGHT = 190;
        static constexpr uint16_t START_THRESHOLD_KA_RIGHT = 265;

        enum class Mode {
            Inactive,
            Welcome,
            Instructions,
            PadNormal,
            PadHard,
            BleedDetected,
            Review,
            Saving,
            Complete,
            CancelConfirm,
            Cancelled,
        };

        enum class WatchSuccessAction : uint8_t {
            None,
            AdvanceToHard,
            FinishPad,
        };

        Mode current_mode{Mode::Inactive};
        uint8_t current_pad{0};
        uint16_t recommended_thresholds[4]{};
        Mode mode_before_cancel{Mode::Inactive};

        uint8_t normal_hits_done{0};
        bool hard_hit_done{false};
        uint32_t last_target_hit_time{0};
        uint32_t phase_start{0};

        bool bleed_watch_active{false};
        uint32_t bleed_watch_until{0};
        uint8_t bleed_target_index{0};
        uint16_t bleed_watch_peaks[4]{};
        uint8_t last_offender_mask{0};
        uint16_t last_threshold_before_raise[4]{};
        uint8_t retry_count_for_pad{0};
        uint8_t prev_hit_mask{0};
        uint8_t prev_cross_mask{0};
        uint8_t prev_arb_mask{0};
        uint8_t prev_held_mask{0};
        WatchSuccessAction watch_success_action{WatchSuccessAction::None};

        static constexpr uint8_t REQUIRED_NORMAL_HITS = 5;
        static constexpr uint16_t MIN_TARGET_HIT_STRENGTH = 30;
        static constexpr uint16_t TARGET_DOMINANCE_SLACK = 16;
        static constexpr uint32_t HIT_COOLDOWN_MS = 80;
        static constexpr uint32_t BLEED_WATCH_MS = 140;
        static constexpr uint16_t BLEED_PRETRIGGER_MARGIN = 110;
        static constexpr uint16_t THRESHOLD_STEP = 5;
        static constexpr uint16_t THRESHOLD_MAX = 1500;
        static constexpr uint32_t COMPLETE_DISPLAY_MS = 2400;
        static constexpr uint32_t SAVING_DISPLAY_MS = 800;
        static constexpr uint32_t CANCELLED_DISPLAY_MS = 1200;

        void reset() {
            current_mode = Mode::Inactive;
            mode_before_cancel = Mode::Inactive;
            current_pad = 0;
            normal_hits_done = 0;
            hard_hit_done = false;
            last_target_hit_time = 0;
            phase_start = 0;
            bleed_watch_active = false;
            bleed_watch_until = 0;
            bleed_target_index = 0;
            last_offender_mask = 0;
            retry_count_for_pad = 0;
            prev_hit_mask = 0;
            prev_cross_mask = 0;
            prev_arb_mask = 0;
            prev_held_mask = 0;
            watch_success_action = WatchSuccessAction::None;
            for (uint8_t i = 0; i < 4; ++i) {
                recommended_thresholds[i] = 0;
                bleed_watch_peaks[i] = 0;
                last_threshold_before_raise[i] = 0;
            }
        }

        void startWelcome() { reset(); current_mode = Mode::Welcome; }
        void startInstructions() { current_mode = Mode::Instructions; }
        void startPadNormal() {
            current_mode = Mode::PadNormal;
            normal_hits_done = 0;
            hard_hit_done = false;
            last_target_hit_time = 0;
            bleed_watch_active = false;
            bleed_watch_until = 0;
            last_offender_mask = 0;
            watch_success_action = WatchSuccessAction::None;
            for (uint8_t i = 0; i < 4; ++i) {
                bleed_watch_peaks[i] = 0;
                last_threshold_before_raise[i] = recommended_thresholds[i];
            }
            phase_start = to_ms_since_boot(get_absolute_time());
        }
        void startPadHard() {
            current_mode = Mode::PadHard;
            hard_hit_done = false;
            last_target_hit_time = 0;
            bleed_watch_active = false;
            bleed_watch_until = 0;
            watch_success_action = WatchSuccessAction::None;
            for (uint8_t i = 0; i < 4; ++i) {
                bleed_watch_peaks[i] = 0;
            }
            phase_start = to_ms_since_boot(get_absolute_time());
        }
        void startBleedDetected() { current_mode = Mode::BleedDetected; phase_start = to_ms_since_boot(get_absolute_time()); }
        void startReview() { current_mode = Mode::Review; phase_start = to_ms_since_boot(get_absolute_time()); }
        void startSaving() { current_mode = Mode::Saving; phase_start = to_ms_since_boot(get_absolute_time()); }
        void startComplete() { current_mode = Mode::Complete; phase_start = to_ms_since_boot(get_absolute_time()); }
        void startCancelConfirm() { mode_before_cancel = current_mode; current_mode = Mode::CancelConfirm; phase_start = to_ms_since_boot(get_absolute_time()); }
        void startCancelled() { current_mode = Mode::Cancelled; phase_start = to_ms_since_boot(get_absolute_time()); }

        [[nodiscard]] bool isActive() const { return current_mode != Mode::Inactive; }

        [[nodiscard]] uint8_t currentPadIndex() const { return PAD_ORDER[current_pad]; }
        [[nodiscard]] const char* currentPadName() const { return PAD_NAMES[current_pad]; }

        [[nodiscard]] bool hasRecommendations() const {
            for (uint8_t i = 0; i < 4; ++i) { if (recommended_thresholds[i] > 0) return true; }
            return false;
        }
    };

  private:
    enum class SideWinner : uint8_t {
        None = 0,
        Don,
        Ka,
    };

    // ============================================================================
    // Pad: Fixed circular buffer - ZERO heap allocation, deterministic timing
    // ============================================================================
    class Pad {
      private:
        static constexpr uint8_t BUFFER_CAPACITY = 32;

        struct BufferEntry {
            uint16_t value;
            uint32_t timestamp;
        };

        // Circular buffer - stack-allocated, fixed size
        BufferEntry m_buffer[BUFFER_CAPACITY]{};
        uint8_t m_head{0};   // index of oldest entry
        uint8_t m_count{0};  // number of valid entries

        uint8_t m_channel;
        uint32_t m_last_change{0};
        bool m_active{false};

      public:
        explicit Pad(uint8_t channel) : m_channel(channel) {}

        [[nodiscard]] uint8_t getChannel() const { return m_channel; }
        [[nodiscard]] bool getState() const { return m_active; }
        void setState(bool state, uint16_t hold_ms);

        void addToBuffer(uint16_t value, uint16_t hold_ms);
        [[nodiscard]] uint16_t getMaxValueInBuffer() const;
        [[nodiscard]] uint16_t getAnalog(float gain = 1.0f) const;
        // Compute analog from an already-known raw value — avoids rescanning the buffer
        [[nodiscard]] static uint16_t computeAnalogFromRaw(uint16_t raw, float gain);
    };

    class RollCounter {
      private:
        uint32_t m_timeout_ms;
        uint32_t m_last_update;
        uint16_t m_roll_count;
        uint16_t m_previous_roll;

      public:
        RollCounter(uint32_t timeout_ms);

        void reset();

        void addHit();
        void update();

        [[nodiscard]] uint16_t getRoll() const { return m_roll_count; };
        [[nodiscard]] uint16_t getPreviousRoll() const { return m_previous_roll; };
    };

    class AdcInterface {
      public:
        virtual ~AdcInterface() = default;

        virtual std::array<uint16_t, 4> read() = 0;
    };

    class InternalAdc : public AdcInterface {
      private:
        Config::InternalAdc m_config;
        // Aug-2025 known-good initialization profile
        std::array<uint16_t, 4> m_baseline_values{400, 400, 400, 400};

      public:
        InternalAdc(const Config::InternalAdc &config);
        std::array<uint16_t, 4> read() final;
    };

    class ExternalAdc : public AdcInterface {
      public:
        ExternalAdc(const Config::ExternalAdc &config);
        std::array<uint16_t, 4> read() final;
    };

    Config m_config;
    std::unique_ptr<AdcInterface> m_adc;

    // ============================================================================
    // m_pads: Stack-allocated array - O(1) access, zero heap allocation
    // Order MUST match Id enum: [0]=DON_LEFT, [1]=KA_LEFT, [2]=DON_RIGHT, [3]=KA_RIGHT
    // ============================================================================
    std::array<Pad, 4> m_pads;
    std::array<uint32_t, 4> m_dbg_hit_until_ms{};
    std::array<uint32_t, 4> m_dbg_cross_until_ms{};
    std::array<uint32_t, 4> m_dbg_arb_until_ms{};
    std::array<uint32_t, 4> m_last_rise_ms{};
    std::array<uint32_t, 2> m_same_side_lock_until_ms{};
    std::array<SideWinner, 2> m_same_side_winner{{SideWinner::None, SideWinner::None}};

    RollCounter m_roll_counter;

    GuidedCalState m_guided_cal_state;
    uint32_t m_last_guided_cal_report_version{0};
    char m_last_guided_cal_report[512]{};

    void updateDigitalInputState(Utils::InputState &input_state, const std::array<uint16_t, 4> &raw_values);
    void updateAnalogInputState(Utils::InputState &input_state, const std::array<uint16_t, 4> &raw_values);
    std::array<uint16_t, 4> readInputs();
    void _finishCurrentPadTest();
    void _advanceToNextPad();

  public:
    Drum(const Config &config);

    void updateInputState(Utils::InputState &input_state, usb_mode_t usb_mode = USB_MODE_SWITCH_TATACON);

    void setHitHoldMs(uint16_t delay);
    void setTriggerThresholds(const Config::Thresholds &thresholds);

    // Guided calibration wizard public interface
    void startGuidedCalibration();  // Entry point: begins at guided intro
    void updateGuidedCalibration(const Utils::InputState::Drum &drum_state, const std::array<uint16_t, 4> &raw_values);
    void advanceCalibWizard();  // A button: advance through wizard steps
    void cancelGuidedCalibration();  // B button: cancel at any point
    void applyGuidedCalRecommendations();
    [[nodiscard]] bool isGuidedCalActive() const { return m_guided_cal_state.isActive(); }
    [[nodiscard]] const GuidedCalState &getGuidedCalState() const { return m_guided_cal_state; }
    [[nodiscard]] const char *getLastGuidedCalReport() const { return m_last_guided_cal_report; }
    [[nodiscard]] uint32_t getLastGuidedCalReportVersion() const { return m_last_guided_cal_report_version; }

    const Config::Thresholds &getCurrentThresholds() const {
        return m_config.trigger_thresholds;
    }
};

} // namespace OuchiTaiko::Peripherals

#endif // PERIPHERALS_DRUM_H_

// End of file Drum.h


