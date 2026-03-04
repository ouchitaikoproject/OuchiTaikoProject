// Beginning of file Drum.h

#ifndef PERIPHERALS_DRUM_H_
#define PERIPHERALS_DRUM_H_

#include "utils/InputState.h"
#include "usb/device_driver.h"

#include "hardware/spi.h"
#include "pico/time.h"

#include <array>
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
    // AUTO CALIBRATE - Single 5-second calibration phase (matches web tool algorithm)
    // ============================================================================

    struct TantrumState {
        enum class Mode {
            Inactive,
            Instructions,   // Instructions screen - wait for user to press CONFIRM
            Countdown,      // 3s - "DON'T HIT YET!" - Sampling silence
            Calibrating,    // 20s - "HIT THE DRUMS!" - Mix rolls & standard hits
            ShowingResults, // 5s - Display calculated thresholds
            NeedsRedo
        };

        Mode current_mode{Mode::Inactive};
        uint32_t countdown_start{0};
        uint32_t calibration_start{0};
        uint32_t last_hit_time{0};  // For hit cooldown tracking

        // Per-sensor tracking (indexed by Id enum value: 0=DON_LEFT, 1=KA_LEFT, 2=DON_RIGHT, 3=KA_RIGHT)
        std::array<uint16_t, 4> max_hit_value{};       // Highest value seen on this sensor
        std::array<uint16_t, 4> max_crosstalk_to{};    // Highest crosstalk seen TO this sensor

        // Results
        std::array<uint16_t, 4> recommended_thresholds{};
        uint32_t total_hits_detected{0};
        bool high_crosstalk_warning{false};
        bool needs_redo{false};
        const char* redo_reason{nullptr};  // Points to string literals only — no heap allocation

        // Single-phase timing constants — matched to web tool for consistent results
        static constexpr uint32_t COUNTDOWN_DURATION_MS = 3000;      // 3s countdown
        static constexpr uint32_t CALIBRATION_DURATION_MS = 5000;    // 5s calibration (matches web tool)
        static constexpr uint32_t RESULTS_DURATION_MS = 5000;        // 5s results display
        
        // Detection thresholds — matched to web tool algorithm
        static constexpr uint16_t MIN_HIT_STRENGTH = 30;            // Minimum to count as hit (matches web tool)
        static constexpr uint16_t MIN_ACCEPTABLE_MAX = 300;         // User must hit at least this hard
        static constexpr uint16_t SAFETY_MARGIN = 20;               // Added above crosstalk (matches web tool)
        static constexpr uint32_t HIT_COOLDOWN_MS = 80;             // 80ms between hits (matches web tool)

        void reset() {
            current_mode = Mode::Inactive;
            max_hit_value.fill(0);
            max_crosstalk_to.fill(0);
            recommended_thresholds.fill(0);
            total_hits_detected = 0;
            high_crosstalk_warning = false;
            needs_redo = false;
            redo_reason = nullptr;
        }

        void startInstructions() {
            reset();
            current_mode = Mode::Instructions;
        }

        void startCountdown() {
            current_mode = Mode::Countdown;
            countdown_start = to_ms_since_boot(get_absolute_time());
        }

        void startCalibration() {
            current_mode = Mode::Calibrating;
            calibration_start = to_ms_since_boot(get_absolute_time());
            last_hit_time = 0;  // Reset hit cooldown timer
        }

        [[nodiscard]] bool isActive() const {
            return current_mode == Mode::Instructions ||
                   current_mode == Mode::Countdown || 
                   current_mode == Mode::Calibrating;
        }

        [[nodiscard]] uint32_t getSecondsRemaining() const {
            uint32_t now = to_ms_since_boot(get_absolute_time());

            if (current_mode == Mode::Countdown) {
                uint32_t elapsed = now - countdown_start;
                if (elapsed >= COUNTDOWN_DURATION_MS) return 0;
                return (COUNTDOWN_DURATION_MS - elapsed + 999) / 1000; // Round up
            }

            if (current_mode == Mode::Calibrating) {
                uint32_t elapsed = now - calibration_start;
                if (elapsed >= CALIBRATION_DURATION_MS) return 0;
                return (CALIBRATION_DURATION_MS - elapsed + 999) / 1000;
            }

            return 0;
        }

        [[nodiscard]] float getProgress() const {
            uint32_t now = to_ms_since_boot(get_absolute_time());
            uint32_t elapsed = 0;
            uint32_t total_duration = 0;

            if (current_mode == Mode::Calibrating) {
                elapsed = now - calibration_start;
                total_duration = CALIBRATION_DURATION_MS;
            } else {
                return 0.0f;
            }

            // Clamp to [0.0, 1.0] range
            if (elapsed >= total_duration) {
                return 1.0f;
            }

            // Integer-based calculation: (elapsed * 1000) / total_duration / 1000.0f
            uint32_t progress_permille = (elapsed * 1000u) / total_duration;
            return static_cast<float>(progress_permille) * 0.001f;
        }

        // Convenience accessors for recommended thresholds (used by Display and applyTantrumRecommendations)
        [[nodiscard]] uint16_t getRecommendedThreshold(Id id) const {
            return recommended_thresholds[static_cast<uint8_t>(id)];
        }
        [[nodiscard]] bool hasRecommendations() const {
            for (auto v : recommended_thresholds) { if (v > 0) return true; }
            return false;
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

    void updateDigitalInputState(Utils::InputState &input_state, const std::array<uint16_t, 4> &raw_values);
    void updateAnalogInputState(Utils::InputState &input_state, const std::array<uint16_t, 4> &raw_values);
    std::array<uint16_t, 4> readInputs();
    void processCalibrationData(const std::array<uint16_t, 4> &raw_values);

  public:
    Drum(const Config &config);

    void updateInputState(Utils::InputState &input_state, usb_mode_t usb_mode = USB_MODE_SWITCH_TATACON);

    void setDebounceDelay(uint16_t delay);
    void setTriggerThresholds(const Config::Thresholds &thresholds);

    // Taiko Tantrum public interface
    void startTaikoTantrum();
    void startTantrumCountdown();  // Start countdown from instructions screen
    void updateTaikoTantrum(const std::array<uint16_t, 4> &raw_values);
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
