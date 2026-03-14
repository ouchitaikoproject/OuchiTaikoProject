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
        uint16_t debounce_delay_ms;
        uint32_t roll_counter_timeout_ms;
        float analog_gain;  // Analog mode gain multiplier (compensates for missing OpAmp circuit)
        AdcChannels adc_channels;
        std::variant<InternalAdc, ExternalAdc> adc_config;
    };

    // ============================================================================
    // ============================================================================
    // AUTO CALIBRATE - Guided per-pad wizard (matches web tool algorithm exactly)
    //
    // Flow per pad (4 pads total, order: DON_LEFT, DON_RIGHT, KA_LEFT, KA_RIGHT):
    //   Step 0 - Normal hits x3  -> median -> normalRef[pad]
    //   Step 1 - Hard hits x3    -> median -> maxHit[pad]
    //   Step 2 - Roll 3s         -> crosstalk on other 3 sensors -> maxCrosstalk[pad]
    //   Threshold = max(maxCrosstalk + margin, normalRef * 0.45)
    //               capped at min(normalRef * 0.55, maxHit * 0.50)
    // ============================================================================

    struct TantrumState {
        // Natural physical order, left-to-right across the drum:
        // Ka Left, Don Left, Don Right, Ka Right
        static constexpr uint8_t PAD_ORDER[4] = {1, 0, 2, 3};
        static constexpr const char* PAD_NAMES[4] = {
            "Ka Left", "Don Left", "Don Right", "Ka Right"
        };

        enum class Mode {
            Inactive,
            Welcome,
            PadNormal,
            PadHard,
            PadRoll,
            PhaseTransition,  // 2s buffer between phases -- prevents accidental double-hit advancing
            PadDone,
            Overview,
            Saving,
            Complete,
            Error,
        };

        Mode current_mode{Mode::Inactive};
        uint8_t current_pad{0};
        uint8_t current_step{0};

        static constexpr uint8_t MAX_HITS = 5;
        uint16_t hit_peaks[MAX_HITS]{};
        uint8_t  hit_count{0};
        uint32_t last_hit_time{0};

        uint32_t roll_start{0};
        bool roll_started{false};  // true once first hit detected during roll phase

        uint16_t normal_ref[4]{};
        uint16_t max_hit[4]{};
        uint16_t max_crosstalk[4]{};
        uint16_t recommended_thresholds[4]{};
        bool     pad_done[4]{};

        bool high_crosstalk_warning{false};
        const char* error_msg{nullptr};

        static constexpr uint32_t ROLL_DURATION_MS          = 3000;
        static constexpr uint32_t ROLL_START_TIMEOUT_MS     = 4000;
        static constexpr uint32_t PAD_DONE_DISPLAY_MS        = 1500;
        static constexpr uint32_t OVERVIEW_DISPLAY_MS        = 2200;
        static constexpr uint32_t COMPLETE_DISPLAY_MS        = 3000;
        static constexpr uint32_t SAVING_DISPLAY_MS          = 800;
        static constexpr uint32_t PHASE_TRANSITION_DELAY_MS  = 2000;  // 2s buffer between phases

        // Phase transition: what's coming next (points to string literal)
        const char* transition_next_label{nullptr};
        Mode transition_next_mode{Mode::PadNormal};

        static constexpr uint16_t MIN_HIT_STRENGTH    = 30;
        static constexpr uint16_t MIN_ACCEPTABLE_MAX  = 200;
        static constexpr uint16_t MIN_STRONG_DELTA    = 35;
        static constexpr uint32_t HIT_COOLDOWN_MS     = 80;
        static constexpr uint16_t SAFETY_MARGIN_DON   = 20;
        static constexpr uint16_t SAFETY_MARGIN_KA    = 35;
        static constexpr uint16_t MIN_THRESHOLD_DON   = 70;
        static constexpr uint16_t MIN_THRESHOLD_KA    = 95;

        uint32_t phase_start{0};

        void reset() {
            current_mode  = Mode::Inactive;
            current_pad   = 0;
            current_step  = 0;
            hit_count     = 0;
            last_hit_time = 0;
            roll_start    = 0;
            roll_started  = false;
            phase_start   = 0;
            for (uint8_t i = 0; i < 4; ++i) {
                normal_ref[i] = max_hit[i] = max_crosstalk[i] = 0;
                recommended_thresholds[i] = 0;
                pad_done[i] = false;
            }
            for (uint8_t i = 0; i < MAX_HITS; ++i) hit_peaks[i] = 0;
            high_crosstalk_warning = false;
            error_msg = nullptr;
            transition_next_mode = Mode::PadNormal;
        }

        void startWelcome() { reset(); current_mode = Mode::Welcome; }

        void _beginHitPhase(Mode m) {
            current_mode  = m;
            hit_count     = 0;
            last_hit_time = 0;
            for (uint8_t i = 0; i < MAX_HITS; ++i) hit_peaks[i] = 0;
            phase_start = to_ms_since_boot(get_absolute_time());
        }

        void startPadNormal() { _beginHitPhase(Mode::PadNormal); }
        void startPadHard()   { _beginHitPhase(Mode::PadHard);   }

        void startPadRoll() {
            current_mode = Mode::PadRoll;
            roll_start   = 0;        // Timer starts on first hit, not on entry
            roll_started = false;
            phase_start  = to_ms_since_boot(get_absolute_time());
        }

        void startPhaseTransition(const char* next_label, Mode next_mode) {
            current_mode         = Mode::PhaseTransition;
            transition_next_label = next_label;
            transition_next_mode  = next_mode;
            phase_start          = to_ms_since_boot(get_absolute_time());
        }

        void startPadDone()  { current_mode = Mode::PadDone;   phase_start = to_ms_since_boot(get_absolute_time()); }
        void startOverview() { current_mode = Mode::Overview;  phase_start = to_ms_since_boot(get_absolute_time()); }
        void startSaving()   { current_mode = Mode::Saving;    phase_start = to_ms_since_boot(get_absolute_time()); }
        void startComplete() { current_mode = Mode::Complete;  phase_start = to_ms_since_boot(get_absolute_time()); }

        [[nodiscard]] bool isActive() const { return current_mode != Mode::Inactive; }

        [[nodiscard]] uint8_t currentPadIndex() const { return PAD_ORDER[current_pad]; }
        [[nodiscard]] const char* currentPadName() const { return PAD_NAMES[current_pad]; }

        [[nodiscard]] float getRollProgress() const {
            if (!roll_started) return 0.0f;
            uint32_t elapsed = to_ms_since_boot(get_absolute_time()) - roll_start;
            if (elapsed >= ROLL_DURATION_MS) return 1.0f;
            return static_cast<float>((elapsed * 1000u) / ROLL_DURATION_MS) * 0.001f;
        }

        [[nodiscard]] uint32_t getRollSecondsRemaining() const {
            if (!roll_started) return ROLL_DURATION_MS / 1000;
            uint32_t elapsed = to_ms_since_boot(get_absolute_time()) - roll_start;
            if (elapsed >= ROLL_DURATION_MS) return 0;
            return (ROLL_DURATION_MS - elapsed + 999) / 1000;
        }

        [[nodiscard]] uint16_t medianOfHits() const {
            uint8_t n = hit_count < 3 ? hit_count : 3;
            if (n == 0) return 0;
            if (n == 1) return hit_peaks[0];
            if (n == 2) return (hit_peaks[0] + hit_peaks[1]) / 2;
            uint16_t a = hit_peaks[0], b = hit_peaks[1], c = hit_peaks[2];
            if (a > b) { uint16_t t = a; a = b; b = t; }
            if (b > c) { uint16_t t = b; b = c; c = t; }
            if (a > b) { uint16_t t = a; a = b; b = t; }
            (void)a; (void)c;
            return b;
        }

        [[nodiscard]] bool hasRecommendations() const {
            for (uint8_t i = 0; i < 4; ++i) { if (recommended_thresholds[i] > 0) return true; }
            return false;
        }

        void computeThreshold(uint8_t i) {
            // Guard: if we have no valid hit data, use a safe default rather than 0
            if (normal_ref[i] == 0 || max_hit[i] == 0) {
                recommended_thresholds[i] = 150;  // Safe fallback - won't break detection
                return;
            }
            bool is_ka = (i == 1 || i == 3);
            uint16_t margin = is_ka ? SAFETY_MARGIN_KA : SAFETY_MARGIN_DON;
            uint16_t min_floor = is_ka ? MIN_THRESHOLD_KA : MIN_THRESHOLD_DON;
            if (max_hit[i] > 0) {
                uint32_t crosstalk_ratio_permille = (static_cast<uint32_t>(max_crosstalk[i]) * 1000u) / max_hit[i];
                if (crosstalk_ratio_permille >= 450u) {
                    margin += 24;
                } else if (crosstalk_ratio_permille >= 350u) {
                    margin += 14;
                } else if (crosstalk_ratio_permille >= 250u) {
                    margin += 8;
                }
            }
            uint16_t crosstalk_thr = max_crosstalk[i] + margin;
            uint16_t floor_thr = static_cast<uint16_t>((static_cast<uint32_t>(normal_ref[i]) * 45u) / 100u);
            if (floor_thr < min_floor) floor_thr = min_floor;
            uint16_t candidate = crosstalk_thr > floor_thr ? crosstalk_thr : floor_thr;
            uint16_t cap_normal = static_cast<uint16_t>((static_cast<uint32_t>(normal_ref[i]) * 55u) / 100u);
            uint16_t cap_hit    = max_hit[i] >> 1;
            uint16_t cap = cap_normal < cap_hit ? cap_normal : cap_hit;
            if (cap < min_floor) cap = min_floor;
            if (candidate > cap) {
                candidate = cap;
                if (max_crosstalk[i] > (max_hit[i] >> 1)) high_crosstalk_warning = true;
            }
            recommended_thresholds[i] = candidate;
        }

        // Compute all 4 thresholds at once -- called AFTER all pads have rolled
        // so max_crosstalk[] is fully populated for every pad before any threshold is computed.
        void computeAllThresholds() {
            for (uint8_t i = 0; i < 4; ++i) {
                computeThreshold(i);
            }
        }
    };

  private:
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
        void setState(bool state, uint16_t debounce_delay);

        void addToBuffer(uint16_t value, uint16_t debounce_delay);
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
        std::array<uint16_t, 4> m_baseline_values{225, 225, 390, 390};

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

    RollCounter m_roll_counter;

    TantrumState m_tantrum_state;
    uint32_t m_last_tantrum_report_version{0};
    char m_last_tantrum_report[512]{};



    void updateDigitalInputState(Utils::InputState &input_state, const std::array<uint16_t, 4> &raw_values);
    void updateAnalogInputState(Utils::InputState &input_state, const std::array<uint16_t, 4> &raw_values);
    std::array<uint16_t, 4> readInputs();
    void _finishCurrentPadRoll();
    void _advanceToNextPad();

  public:
    Drum(const Config &config);

    void updateInputState(Utils::InputState &input_state, usb_mode_t usb_mode = USB_MODE_SWITCH_TATACON);

    void setDebounceDelay(uint16_t delay);
    void setTriggerThresholds(const Config::Thresholds &thresholds);

    // Guided calibration wizard public interface
    void startTaikoTantrum();          // Entry point: begins at Welcome screen
    void updateTaikoTantrum(const std::array<uint16_t, 4> &raw_values);
    void advanceCalibWizard();         // A button: advance through wizard steps
    void cancelTaikoTantrum();         // B button: cancel at any point
    void applyTantrumRecommendations();
    [[nodiscard]] bool isTantrumActive() const { return m_tantrum_state.isActive(); }
    [[nodiscard]] const TantrumState& getTantrumState() const { return m_tantrum_state; }
    [[nodiscard]] const char* getLastTantrumReport() const { return m_last_tantrum_report; }
    [[nodiscard]] uint32_t getLastTantrumReportVersion() const { return m_last_tantrum_report_version; }

    const Config::Thresholds& getCurrentThresholds() const {
        return m_config.trigger_thresholds;
    }
};

} // namespace OuchiTaiko::Peripherals

#endif // PERIPHERALS_DRUM_H_

// End of file Drum.h
