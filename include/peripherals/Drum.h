// Beginning of file Drum.h

#ifndef PERIPHERALS_DRUM_H_
#define PERIPHERALS_DRUM_H_

#include "utils/InputState.h"

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
        AdcChannels adc_channels;
        std::variant<InternalAdc, ExternalAdc> adc_config;
    };

    // ============================================================================
    // TAIKO-TUNE V2 STRUCTURES (AUTOMATED MODE WITH COUNTDOWN)
    // ============================================================================

    struct VelocityHistogram {
        static constexpr uint8_t NUM_BINS = 4;
        static constexpr uint16_t BIN_SIZE = 100;
        
        std::array<uint16_t, NUM_BINS> bins{0};
        uint32_t total_hits{0};
        uint32_t sum_velocity{0};
        uint16_t max_velocity{0};
        uint16_t min_velocity{UINT16_MAX};
        
        void recordHit(uint16_t velocity) {
            uint8_t bin_index = std::min((uint16_t)(velocity / BIN_SIZE), (uint16_t)(NUM_BINS - 1));
            bins[bin_index]++;
            total_hits++;
            sum_velocity += velocity;
            
            if (velocity > max_velocity) max_velocity = velocity;
            if (velocity < min_velocity && velocity > 0) min_velocity = velocity;
        }
        
        uint16_t getAverage() const {
            return total_hits > 0 ? (sum_velocity / total_hits) : 0;
        }
        
        uint16_t getStdDev() const {
            if (total_hits == 0) return 0;
            uint16_t avg = getAverage();
            return avg / 4;
        }
        
        void reset() {
            bins.fill(0);
            total_hits = 0;
            sum_velocity = 0;
            max_velocity = 0;
            min_velocity = UINT16_MAX;
        }
    };
    
    struct TaikoTuneState {
        enum class Mode {
            Inactive,
            AnalyzingKaLeft,
            AnalyzingDonLeft,
            AnalyzingDonRight,
            AnalyzingKaRight,
            ShowingResults,
        };
        
        Mode current_mode{Mode::Inactive};
        Id current_pad{Id::DON_LEFT};

        std::map<Id, VelocityHistogram> histograms;
        std::map<Id, std::array<uint16_t, 60>> crosstalk_data;  // Changed to 60 for TARGET_HITS

        // Two-pass crosstalk detection
        uint8_t current_pass{1};  // 1 or 2
        std::map<Id, uint16_t> pass1_max_crosstalk;  // Store Pass 1 crosstalk for comparison

        uint16_t hits_collected{0};
        static constexpr uint16_t TARGET_HITS = 60;  // Changed to 60 for double-pass
        
        // Countdown for noise sampling
        static constexpr uint32_t COUNTDOWN_DURATION_MS = 3000;
        uint32_t countdown_start{0};
        bool countdown_complete{false};
        uint16_t max_noise_level{0};
        
        // Cancellation support
        bool cancelled{false};
        Config::Thresholds backup_thresholds{0, 0, 0, 0};
        
        struct Recommendation {
            bool needs_adjustment{false};
            uint16_t suggested_threshold{0};
            uint16_t crosstalk_level{0};
            std::string message;
        };
        std::map<Id, Recommendation> recommendations;
        
        void reset(uint8_t pass = 1) {
            current_mode = Mode::Inactive;
            hits_collected = 0;
            countdown_complete = false;
            max_noise_level = 0;
            cancelled = false;

            // Smart reset: Preserve Pass 1 data when starting Pass 2
            if (pass == 1) {
                // Full reset for Pass 1
                for (auto& [id, hist] : histograms) {
                    hist.reset();
                }
                recommendations.clear();
                pass1_max_crosstalk.clear();
            }
            // Always reset current analysis buffers
            for (auto& [id, data] : crosstalk_data) {
                data.fill(0);
            }
        }

        void startAnalysis(Id pad, uint8_t pass = 1) {
            reset(pass);
            current_pad = pad;
            current_pass = pass;
            countdown_start = to_ms_since_boot(get_absolute_time());
            
            switch (pad) {
                case Id::KA_LEFT: current_mode = Mode::AnalyzingKaLeft; break;
                case Id::DON_LEFT: current_mode = Mode::AnalyzingDonLeft; break;
                case Id::DON_RIGHT: current_mode = Mode::AnalyzingDonRight; break;
                case Id::KA_RIGHT: current_mode = Mode::AnalyzingKaRight; break;
            }
        }
        
        void cancel() {
            cancelled = true;
            reset();
        }
        
        bool isActive() const {
            return current_mode != Mode::Inactive && current_mode != Mode::ShowingResults;
        }
        
        bool isComplete() const {
            return hits_collected >= TARGET_HITS;
        }
        
        uint8_t getCountdownRemaining() const {
            if (countdown_complete) return 0;
            uint32_t now = to_ms_since_boot(get_absolute_time());
            uint32_t elapsed = now - countdown_start;
            if (elapsed >= COUNTDOWN_DURATION_MS) return 0;
            return (COUNTDOWN_DURATION_MS - elapsed) / 1000 + 1;
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

      public:
        Pad(const uint8_t channel);

        [[nodiscard]] uint8_t getChannel() const { return m_channel; };
        [[nodiscard]] bool getState() const { return m_active; };
        void setState(const bool state, const uint16_t debounce_delay);

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
    
    TaikoTuneState m_taikotune_state;

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
    
    // Taiko-Tune V2 public interface
    void startTaikoTuneAnalysis(Id pad, uint8_t pass = 1);
    void updateTaikoTuneAnalysis(const std::map<Id, uint16_t> &raw_values);
    void finishTaikoTuneAnalysis();
    void cancelTaikoTuneAnalysis();  // Cancel analysis
    const TaikoTuneState& getTaikoTuneState() const { return m_taikotune_state; }
    TaikoTuneState& getTaikoTuneState() { return m_taikotune_state; }
    void applyTaikoTuneRecommendation(Id pad);
    
    const Config::Thresholds& getCurrentThresholds() const { 
        return m_config.trigger_thresholds; 
    }
};

} // namespace OuchiTaiko::Peripherals

#endif // PERIPHERALS_DRUM_H_

// End of file Drum.h