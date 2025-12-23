// Beginning of file Drum.h

#ifndef PERIPHERALS_DRUM_H_
#define PERIPHERALS_DRUM_H_

#include "utils/InputState.h"
#include "usb/device_driver.h"

#include "hardware/spi.h"
#include "pico/time.h"

#include <array>
#include <map>
#include <memory>
#include <stdint.h>
#include <variant>
#include <deque>
#include <string>

namespace OuchiTaiko::Peripherals {

class Drum {
  public:
    enum class Id {
        DON_LEFT,
        KA_LEFT,
        DON_RIGHT,
        KA_RIGHT,
    };

    enum class PerformanceProfile {
        STANDARD,      // 25ms debounce, time-based only (current behavior)
        COMPETITIVE,   // 12ms debounce with velocity-aware filtering
        EXTREME,       // 8ms debounce with aggressive velocity filtering (120 rolls/sec capable)
    };

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
        bool big_hit_enable;
        uint16_t big_hit_threshold;
        uint16_t debounce_delay_ms;
        uint32_t roll_counter_timeout_ms;
        bool enable_simultap;
        float analog_gain;  // Analog mode gain multiplier (compensates for missing OpAmp circuit)
        PerformanceProfile performance_profile;  // Performance mode for low-latency triggering
        AdcChannels adc_channels;
        std::variant<InternalAdc, ExternalAdc> adc_config;
    };

    // ============================================================================
    // TAIKO TANTRUM CALIBRATION - Stress test for worst-case crosstalk
    // ============================================================================

    struct TantrumState {
        enum class Mode {
            Inactive,
            Countdown,
            Recording,
            ShowingResults,
            NeedsRedo
        };

        Mode current_mode{Mode::Inactive};
        uint32_t countdown_start{0};
        uint32_t recording_start{0};

        // Per-sensor tracking
        std::map<Id, uint16_t> max_hit_value;       // Highest value seen on this sensor
        std::map<Id, uint16_t> max_crosstalk_to;    // Highest crosstalk seen TO this sensor

        // Results
        std::map<Id, uint16_t> recommended_thresholds;
        uint32_t total_hits_detected{0};
        bool high_crosstalk_warning{false};
        bool needs_redo{false};
        std::string redo_reason;

        // Timing constants
        static constexpr uint32_t COUNTDOWN_DURATION_MS = 3000;     // 3 second countdown
        static constexpr uint32_t RECORDING_DURATION_MS = 20000;    // 20 second recording (doubled for comfort)
        static constexpr uint16_t MIN_HIT_STRENGTH = 150;           // Minimum to count as hit
        static constexpr uint16_t MIN_ACCEPTABLE_MAX = 300;         // User must hit at least this hard
        static constexpr uint16_t SAFETY_MARGIN = 100;              // Added above crosstalk
        static constexpr float MAX_CROSSTALK_RATIO = 0.5f;          // Warn if >50% of hit
        static constexpr uint32_t HIT_COOLDOWN_MS = 50;             // 50ms between hits (20/sec max)

        void reset() {
            current_mode = Mode::Inactive;
            max_hit_value.clear();
            max_crosstalk_to.clear();
            recommended_thresholds.clear();
            total_hits_detected = 0;
            high_crosstalk_warning = false;
            needs_redo = false;
            redo_reason.clear();

            // Initialize all sensors to 0
            for (auto id : {Id::DON_LEFT, Id::DON_RIGHT, Id::KA_LEFT, Id::KA_RIGHT}) {
                max_hit_value[id] = 0;
                max_crosstalk_to[id] = 0;
            }
        }

        void startCountdown() {
            reset();
            current_mode = Mode::Countdown;
            countdown_start = to_ms_since_boot(get_absolute_time());
        }

        void startRecording() {
            current_mode = Mode::Recording;
            recording_start = to_ms_since_boot(get_absolute_time());
        }

        [[nodiscard]] bool isActive() const {
            return current_mode == Mode::Countdown || current_mode == Mode::Recording;
        }

        [[nodiscard]] uint32_t getSecondsRemaining() const {
            uint32_t now = to_ms_since_boot(get_absolute_time());

            if (current_mode == Mode::Countdown) {
                uint32_t elapsed = now - countdown_start;
                if (elapsed >= COUNTDOWN_DURATION_MS) return 0;
                return (COUNTDOWN_DURATION_MS - elapsed + 999) / 1000; // Round up
            }

            if (current_mode == Mode::Recording) {
                uint32_t elapsed = now - recording_start;
                if (elapsed >= RECORDING_DURATION_MS) return 0;
                return (RECORDING_DURATION_MS - elapsed + 999) / 1000;
            }

            return 0;
        }

        [[nodiscard]] float getProgress() const {
            if (current_mode != Mode::Recording) return 0.0f;

            uint32_t now = to_ms_since_boot(get_absolute_time());
            uint32_t elapsed = now - recording_start;
            return static_cast<float>(elapsed) / static_cast<float>(RECORDING_DURATION_MS);
        }
    };

  private:
    class Pad {
      private:
        struct BufferEntry {
            uint16_t value;
            uint32_t timestamp;

            // Comparison operator for std::max_element
            bool operator<(const BufferEntry& other) const {
                return value < other.value;
            }
        };

        uint8_t m_channel;
        uint32_t m_last_change;
        bool m_active;
        std::deque<BufferEntry> m_buffer;
        uint16_t m_baseline;

        // Calculate velocity rise rate (dV/dt) to detect real hits vs bounce
        [[nodiscard]] bool hasSharpVelocityRise(uint16_t current_value) const;

      public:
        Pad(const uint8_t channel);

        [[nodiscard]] uint8_t getChannel() const { return m_channel; };
        [[nodiscard]] bool getState() const { return m_active; };
        void setState(const bool state, const uint16_t debounce_delay, PerformanceProfile profile = PerformanceProfile::STANDARD);

        void addToBuffer(uint16_t value, uint16_t debounce_delay);
        [[nodiscard]] uint16_t getMaxValueInBuffer() const;
        [[nodiscard]] uint16_t getAnalog(float gain = 1.0) const;
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
    std::map<Id, Pad> m_pads;
    RollCounter m_roll_counter;

    TantrumState m_tantrum_state;

    void updateDigitalInputState(Utils::InputState &input_state, const std::map<Id, uint16_t> &raw_values);
    void updateAnalogInputState(Utils::InputState &input_state, const std::map<Id, uint16_t> &raw_values);
    std::map<Id, uint16_t> readInputs();

  public:
    Drum(const Config &config);

    void updateInputState(Utils::InputState &input_state, usb_mode_t usb_mode = USB_MODE_SWITCH_TATACON);

    void setDebounceDelay(uint16_t delay);
    void setTriggerThresholds(const Config::Thresholds &thresholds);
    void setBigHitEnable(bool enable);
    void setBigHitThreshold(uint16_t threshold);
    void setSimulTap(bool enable);
    void setPerformanceProfile(PerformanceProfile profile);
    [[nodiscard]] PerformanceProfile getPerformanceProfile() const { return m_config.performance_profile; }

    // Taiko Tantrum public interface
    void startTaikoTantrum();
    void updateTaikoTantrum(const std::map<Id, uint16_t> &raw_values);
    void finishTaikoTantrum();
    void cancelTaikoTantrum();
    void applyTantrumRecommendations();
    [[nodiscard]] bool isTantrumActive() const { return m_tantrum_state.isActive(); }
    [[nodiscard]] const TantrumState& getTantrumState() const { return m_tantrum_state; }

    const Config::Thresholds& getCurrentThresholds() const {
        return m_config.trigger_thresholds;
    }
};

} // namespace OuchiTaiko::Peripherals

#endif // PERIPHERALS_DRUM_H_

// End of file Drum.h