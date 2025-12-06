// Beginning of file Drum.cpp

#include "peripherals/Drum.h"

#include "hardware/adc.h"
#include "pico/time.h"
#include <mcp3204/Mcp3204Dma.h>

#include <algorithm>
#include <deque>

namespace OuchiTaiko::Peripherals {

Drum::InternalAdc::InternalAdc(const Config::InternalAdc &config) : m_config(config) {
    static const uint adc_base_pin = 26;

    for (uint pin = adc_base_pin; pin < adc_base_pin + 4; ++pin) {
        adc_gpio_init(pin);
    }

    adc_init();
}

std::array<uint16_t, 4> Drum::InternalAdc::read() {
    if (m_config.sample_count == 0) {
        return {};
    }

    std::array<uint32_t, 4> values{};
    for (uint8_t sample_number = 0; sample_number < m_config.sample_count; ++sample_number) {
        for (size_t idx = 0; idx < values.size(); ++idx) {
            adc_select_input(idx);
            values.at(idx) += adc_read();
        }
    }

    std::array<uint16_t, 4> raw_result{};
    std::ranges::transform(values, raw_result.begin(), [&](const auto &sample) { 
        return sample / m_config.sample_count; 
    });

    std::array<uint16_t, 4> result{};
    for (size_t idx = 0; idx < raw_result.size(); ++idx) {
        uint16_t raw_value = raw_result[idx];
        
        if (raw_value > m_baseline_values[idx]) {
            result[idx] = raw_value - m_baseline_values[idx];
        } else {
            result[idx] = 0;
        }
        
        if (result[idx] < 20) {
            m_baseline_values[idx] = (m_baseline_values[idx] * 7 + raw_value) / 8;
        } else if (result[idx] < 100) {
            m_baseline_values[idx] = (m_baseline_values[idx] * 31 + raw_value) / 32;
        }
    }

    return result;
}

Drum::ExternalAdc::ExternalAdc(const Config::ExternalAdc &config) {
    gpio_init(config.spi_level_shifter_enable_pin);
    gpio_set_dir(config.spi_level_shifter_enable_pin, (bool)GPIO_OUT);
    gpio_put(config.spi_level_shifter_enable_pin, true);

    gpio_set_function(config.spi_miso_pin, GPIO_FUNC_SPI);
    gpio_set_function(config.spi_mosi_pin, GPIO_FUNC_SPI);
    gpio_set_function(config.spi_sclk_pin, GPIO_FUNC_SPI);
    spi_init(config.spi_block, config.spi_speed_hz);

    gpio_init(config.spi_scsn_pin);
    gpio_set_dir(config.spi_scsn_pin, (bool)GPIO_OUT);

    Mcp3204Dma::run(config.spi_block, config.spi_scsn_pin);
}

std::array<uint16_t, 4> Drum::ExternalAdc::read() { 
    return Mcp3204Dma::take_maximums(); 
}

Drum::Pad::Pad(const uint8_t channel) : m_channel(channel), m_last_change(0), m_active(false), m_baseline(300){};

void Drum::Pad::setState(const bool state, const uint16_t debounce_delay) {
    if (state != m_active) {
        const uint32_t now = to_ms_since_boot(get_absolute_time());
        if ((now - m_last_change) >= debounce_delay) {
            m_active = state;
            m_last_change = now;
        }
    }
}

void Drum::Pad::addToBuffer(uint16_t value, uint16_t debounce_delay) {
    const uint32_t now = to_ms_since_boot(get_absolute_time());

    while (!m_buffer.empty() && (m_buffer.front().timestamp + debounce_delay) <= now) {
        m_buffer.pop_front();
    }

    m_buffer.push_back({value, now});

    // Update adaptive baseline (same logic as InternalAdc)
    if (value < 20) {
        m_baseline = (m_baseline * 7 + value) / 8;
    } else if (value < 100) {
        m_baseline = (m_baseline * 31 + value) / 32;
    }
}

uint16_t Drum::Pad::getMaxValueInBuffer() const {
    if (m_buffer.empty()) {
        return 0;
    }
    return std::max_element(m_buffer.begin(), m_buffer.end())->value;
}

uint16_t Drum::Pad::getAnalog(float gain) const {
    // Transform 12-bit ADC value (0-4095) to 16-bit range (0-65535)
    const auto raw_to_uint16 = [](uint16_t raw) {
        return ((raw << 4) & 0xFFF0) | ((raw >> 8) & 0x000F);
    };

    uint16_t max_value = getMaxValueInBuffer();

    // Subtract adaptive baseline and clamp to 0 if below baseline
    uint16_t baseline_subtracted;
    if (max_value > m_baseline) {
        baseline_subtracted = max_value - m_baseline;
    } else {
        baseline_subtracted = 0;
    }

    // Apply gain multiplier (compensates for missing OpAmp circuit)
    uint32_t gained_value = static_cast<uint32_t>(baseline_subtracted * gain);

    // Clamp to 12-bit max to prevent overflow before bit-shifting
    if (gained_value > 4095) {
        gained_value = 4095;
    }

    return raw_to_uint16(static_cast<uint16_t>(gained_value));
}

Drum::RollCounter::RollCounter(uint32_t timeout_ms) : m_timeout_ms(timeout_ms), m_last_update(0), m_roll_count(0), m_previous_roll(0) {};

void Drum::RollCounter::reset() {
    m_roll_count = 0;
    m_previous_roll = 0;
}

void Drum::RollCounter::addHit() {
    const uint32_t now = to_ms_since_boot(get_absolute_time());
    
    if ((now - m_last_update) > m_timeout_ms) {
        if (m_roll_count > 1) {
            m_previous_roll = m_roll_count;
        }
        m_roll_count = 0;
    }
    
    m_last_update = now;
    m_roll_count++;
}

void Drum::RollCounter::update() {
    const uint32_t now = to_ms_since_boot(get_absolute_time());
    
    if ((now - m_last_update) > m_timeout_ms) {
        if (m_roll_count > 1) {
            m_previous_roll = m_roll_count;
        }
        m_roll_count = 0;
    }
}

Drum::Drum(const Config &config) : m_config(config), m_roll_counter(config.roll_counter_timeout_ms) {

    std::visit(
        [this](auto &&config) {
            using T = std::decay_t<decltype(config)>;

            if constexpr (std::is_same_v<T, Config::InternalAdc>) {
                m_adc = std::make_unique<InternalAdc>(config);
            } else if constexpr (std::is_same_v<T, Config::ExternalAdc>) {
                m_adc = std::make_unique<ExternalAdc>(config);
            } else {
                static_assert(false, "Unknown ADC type!");
            }
        },
        m_config.adc_config);

    m_pads.emplace(Id::DON_LEFT, config.adc_channels.don_left);
    m_pads.emplace(Id::KA_LEFT, config.adc_channels.ka_left);
    m_pads.emplace(Id::DON_RIGHT, config.adc_channels.don_right);
    m_pads.emplace(Id::KA_RIGHT, config.adc_channels.ka_right);
}

std::map<Drum::Id, uint16_t> Drum::readInputs() {
    std::map<Id, uint16_t> result;

    const auto adc_values = m_adc->read();

    for (const auto &pad : m_pads) {
        result[pad.first] = adc_values[pad.second.getChannel()];
    }

    return result;
}

void Drum::updateDigitalInputState(Utils::InputState &input_state, const std::map<Drum::Id, uint16_t> &raw_values) {
    
    std::map<Id, uint16_t> filtered_raw_values;

    const auto get_threshold = [&](Id target) {
        switch (target) {
        case Id::DON_LEFT:
            return m_config.trigger_thresholds.don_left;
        case Id::DON_RIGHT:
            return m_config.trigger_thresholds.don_right;
        case Id::KA_LEFT:
            return m_config.trigger_thresholds.ka_left;
        case Id::KA_RIGHT:
            return m_config.trigger_thresholds.ka_right;
        }
        return (uint16_t)0;
    };

    for (const auto &entry : raw_values) {
        uint16_t threshold = get_threshold(entry.first);
        filtered_raw_values[entry.first] = (entry.second > threshold) ? entry.second : 0;
    }

    const auto zero_if_not_within_twin = [](auto &values, Id a, Id b) {
        if (values.at(a) == 0 || values.at(b) == 0) {
            return;
        }

        if (values.at(a) > values.at(b)) {
            if (values.at(b) < (values.at(a) >> 1)) {
                values.at(b) = 0;
            }
        } else {
            if (values.at(a) < (values.at(b) >> 1)) {
                values.at(a) = 0;
            }
        }
    };

    zero_if_not_within_twin(filtered_raw_values, Id::DON_LEFT, Id::DON_RIGHT);
    zero_if_not_within_twin(filtered_raw_values, Id::KA_LEFT, Id::KA_RIGHT);

    for (const auto &entry : filtered_raw_values) {
        m_pads.at(entry.first).setState(entry.second != 0, m_config.debounce_delay_ms);
    }

    if (m_config.big_hit_enable) {
        if (raw_values.at(Id::DON_LEFT) > m_config.big_hit_threshold) {
            m_pads.at(Id::DON_RIGHT).setState(true, m_config.debounce_delay_ms);
        }
        if (raw_values.at(Id::DON_RIGHT) > m_config.big_hit_threshold) {
            m_pads.at(Id::DON_LEFT).setState(true, m_config.debounce_delay_ms);
        }
        if (raw_values.at(Id::KA_LEFT) > m_config.big_hit_threshold) {
            m_pads.at(Id::KA_RIGHT).setState(true, m_config.debounce_delay_ms);
        }
        if (raw_values.at(Id::KA_RIGHT) > m_config.big_hit_threshold) {
            m_pads.at(Id::KA_LEFT).setState(true, m_config.debounce_delay_ms);
        }
    }

    bool don_left_rising = !input_state.drum.don_left.triggered && m_pads.at(Id::DON_LEFT).getState();
    bool don_right_rising = !input_state.drum.don_right.triggered && m_pads.at(Id::DON_RIGHT).getState();
    bool ka_left_rising = !input_state.drum.ka_left.triggered && m_pads.at(Id::KA_LEFT).getState();
    bool ka_right_rising = !input_state.drum.ka_right.triggered && m_pads.at(Id::KA_RIGHT).getState();

    if (don_left_rising || don_right_rising || ka_left_rising || ka_right_rising) {
        m_roll_counter.addHit();
    }

    if (!m_config.enable_simultap) {
        const bool ka_active = m_pads.at(Id::KA_LEFT).getState() || m_pads.at(Id::KA_RIGHT).getState();
        const bool don_active = m_pads.at(Id::DON_LEFT).getState() || m_pads.at(Id::DON_RIGHT).getState();

        if (ka_active && don_active) {
            const bool ka_was_first = (input_state.drum.ka_left.triggered || input_state.drum.ka_right.triggered) &&
                                      !(input_state.drum.don_left.triggered || input_state.drum.don_right.triggered);
            const bool don_was_first = (input_state.drum.don_left.triggered || input_state.drum.don_right.triggered) &&
                                       !(input_state.drum.ka_left.triggered || input_state.drum.ka_right.triggered);

            if (ka_was_first) {
                m_pads.at(Id::DON_LEFT).setState(false, m_config.debounce_delay_ms);
                m_pads.at(Id::DON_RIGHT).setState(false, m_config.debounce_delay_ms);
            } else if (don_was_first) {
                m_pads.at(Id::KA_LEFT).setState(false, m_config.debounce_delay_ms);
                m_pads.at(Id::KA_RIGHT).setState(false, m_config.debounce_delay_ms);
            } else {
                m_pads.at(Id::DON_LEFT).setState(false, m_config.debounce_delay_ms);
                m_pads.at(Id::DON_RIGHT).setState(false, m_config.debounce_delay_ms);
            }
        }
    }

    input_state.drum.don_left.triggered = m_pads.at(Id::DON_LEFT).getState();
    input_state.drum.ka_left.triggered = m_pads.at(Id::KA_LEFT).getState();
    input_state.drum.don_right.triggered = m_pads.at(Id::DON_RIGHT).getState();
    input_state.drum.ka_right.triggered = m_pads.at(Id::KA_RIGHT).getState();

    input_state.drum.current_roll = m_roll_counter.getRoll();
    input_state.drum.previous_roll = m_roll_counter.getPreviousRoll();
}

void Drum::updateAnalogInputState(Utils::InputState &input_state, const std::map<Drum::Id, uint16_t> &raw_values) {
    for (const auto &[id, value] : raw_values) {
        m_pads.at(id).addToBuffer(value, m_config.debounce_delay_ms);
    }

    input_state.drum.don_left.raw = m_pads.at(Id::DON_LEFT).getMaxValueInBuffer();
    input_state.drum.ka_left.raw = m_pads.at(Id::KA_LEFT).getMaxValueInBuffer();
    input_state.drum.don_right.raw = m_pads.at(Id::DON_RIGHT).getMaxValueInBuffer();
    input_state.drum.ka_right.raw = m_pads.at(Id::KA_RIGHT).getMaxValueInBuffer();

    input_state.drum.don_left.analog = m_pads.at(Id::DON_LEFT).getAnalog(m_config.analog_gain);
    input_state.drum.ka_left.analog = m_pads.at(Id::KA_LEFT).getAnalog(m_config.analog_gain);
    input_state.drum.don_right.analog = m_pads.at(Id::DON_RIGHT).getAnalog(m_config.analog_gain);
    input_state.drum.ka_right.analog = m_pads.at(Id::KA_RIGHT).getAnalog(m_config.analog_gain);
}

void Drum::updateInputState(Utils::InputState &input_state) {
    const auto raw_values = readInputs();

    if (m_taikotune_state.isActive()) {
        updateTaikoTuneAnalysis(raw_values);
    }

    updateDigitalInputState(input_state, raw_values);
    updateAnalogInputState(input_state, raw_values);
}

void Drum::setDebounceDelay(uint16_t delay) { m_config.debounce_delay_ms = delay; }

void Drum::setTriggerThresholds(const Config::Thresholds &thresholds) { m_config.trigger_thresholds = thresholds; }

void Drum::setBigHitEnable(bool enable) { m_config.big_hit_enable = enable; }

void Drum::setBigHitThreshold(uint16_t threshold) { m_config.big_hit_threshold = threshold; }

void Drum::setSimulTap(bool enable) { m_config.enable_simultap = enable; }

// ============================================================================
// TAIKO-TUNE V2 IMPLEMENTATION (AUTOMATED MODE WITH COUNTDOWN & NOISE SAMPLING)
// ============================================================================

void Drum::startTaikoTuneAnalysis(Id pad, uint8_t pass) {
    // Backup current thresholds before starting analysis (only on Pass 1)
    if (pass == 1) {
        m_taikotune_state.backup_thresholds = m_config.trigger_thresholds;
    }
    m_taikotune_state.startAnalysis(pad, pass);
}

void Drum::updateTaikoTuneAnalysis(const std::map<Id, uint16_t> &raw_values) {
    if (!m_taikotune_state.isActive()) {
        return;
    }
    
    // Check if cancelled
    if (m_taikotune_state.cancelled) {
        return;
    }
    
    uint32_t now = to_ms_since_boot(get_absolute_time());
    
    // PHASE 1: Countdown - Sample noise levels
    if (!m_taikotune_state.countdown_complete) {
        uint32_t elapsed = now - m_taikotune_state.countdown_start;
        
        // Track maximum noise during countdown
        Id target_pad = m_taikotune_state.current_pad;
        uint16_t current_value = raw_values.at(target_pad);
        if (current_value > m_taikotune_state.max_noise_level) {
            m_taikotune_state.max_noise_level = current_value;
        }
        
        // Check if countdown finished
        if (elapsed >= TaikoTuneState::COUNTDOWN_DURATION_MS) {
            m_taikotune_state.countdown_complete = true;
        }
        return;
    }
    
    // PHASE 2: Analysis - Record hits using SMART threshold independent of user settings
    Id target_pad = m_taikotune_state.current_pad;
    uint16_t target_velocity = raw_values.at(target_pad);
    
    // SMART DETECTION: Use noise floor + safety margin, ignoring potentially wrong user settings
    // This ensures we capture real hits even if user has threshold set to 1 or 999
    uint16_t analysis_threshold = m_taikotune_state.max_noise_level + 30;
    
    // Ensure minimum threshold (in case noise sampling was too low)
    if (analysis_threshold < 40) {
        analysis_threshold = 40;
    }
    
    static uint32_t last_hit_time = 0;
    static const uint32_t HIT_COOLDOWN_MS = 100;
    
    // Detect hits using our smart threshold (NOT user's potentially broken setting)
    if (target_velocity > analysis_threshold && (now - last_hit_time) > HIT_COOLDOWN_MS) {
        m_taikotune_state.histograms[target_pad].recordHit(target_velocity);
        
        for (const auto& [pad_id, velocity] : raw_values) {
            if (pad_id != target_pad) {
                uint16_t idx = m_taikotune_state.hits_collected;
                if (idx < 60) {  // Changed to 60 for TARGET_HITS
                    m_taikotune_state.crosstalk_data[pad_id][idx] = velocity;
                }
            }
        }
        
        m_taikotune_state.hits_collected++;
        last_hit_time = now;
    }
    
    if (m_taikotune_state.isComplete()) {
        finishTaikoTuneAnalysis();
    }
}

void Drum::finishTaikoTuneAnalysis() {
    // Check if cancelled before finishing
    if (m_taikotune_state.cancelled) {
        return;
    }
    
    Id target_pad = m_taikotune_state.current_pad;
    const auto& histogram = m_taikotune_state.histograms[target_pad];
    
    TaikoTuneState::Recommendation rec;
    
    uint16_t avg_velocity = histogram.getAverage();
    uint16_t current_threshold = 0;
    
    switch (target_pad) {
        case Id::KA_LEFT: current_threshold = m_config.trigger_thresholds.ka_left; break;
        case Id::DON_LEFT: current_threshold = m_config.trigger_thresholds.don_left; break;
        case Id::DON_RIGHT: current_threshold = m_config.trigger_thresholds.don_right; break;
        case Id::KA_RIGHT: current_threshold = m_config.trigger_thresholds.ka_right; break;
    }
    
    uint16_t hits_above_threshold = 0;
    for (size_t i = 0; i < histogram.bins.size(); i++) {
        uint16_t bin_min = i * VelocityHistogram::BIN_SIZE;
        if (bin_min >= current_threshold) {
            hits_above_threshold += histogram.bins[i];
        }
    }
    
    float trigger_rate = (float)hits_above_threshold / histogram.total_hits;
    
    uint16_t max_crosstalk = 0;
    for (const auto& [other_pad, data] : m_taikotune_state.crosstalk_data) {
        if (other_pad == target_pad) continue;
        
        uint16_t avg_crosstalk = 0;
        for (uint16_t val : data) {
            avg_crosstalk += val;
        }
        avg_crosstalk /= 60;  // Changed to 60 for TARGET_HITS
        
        if (avg_crosstalk > max_crosstalk) {
            max_crosstalk = avg_crosstalk;
        }
    }

    // TWO-PASS CROSSTALK DETECTION: Compare Pass 1 vs Pass 2
    if (m_taikotune_state.current_pass == 1) {
        // Pass 1: Store crosstalk for later comparison with Pass 2
        m_taikotune_state.pass1_max_crosstalk[target_pad] = max_crosstalk;
    } else if (m_taikotune_state.current_pass == 2) {
        // Pass 2: Compare with Pass 1 and use MAXIMUM (bidirectional crosstalk)
        if (m_taikotune_state.pass1_max_crosstalk.count(target_pad) > 0) {
            uint16_t pass1_crosstalk = m_taikotune_state.pass1_max_crosstalk[target_pad];
            max_crosstalk = std::max(max_crosstalk, pass1_crosstalk);
        }
    }

    rec.crosstalk_level = max_crosstalk;

    // Calculate safe minimum threshold (noise floor + safety margin)
    uint16_t safe_minimum = m_taikotune_state.max_noise_level + 20;
    
    // Calculate new threshold (cast to uint16_t for type consistency)
    if (trigger_rate < 0.70) {
        rec.needs_adjustment = true;
        uint16_t calculated = (avg_velocity > 30) ? (avg_velocity - 30) : 0;
        rec.suggested_threshold = (calculated > safe_minimum) ? calculated : safe_minimum;
        rec.message = "Threshold too HIGH\nLower to: " + std::to_string(rec.suggested_threshold);
    } else if (trigger_rate > 0.95 || current_threshold < max_crosstalk + 20) {
        rec.needs_adjustment = true;
        uint16_t calc1 = (avg_velocity > 20) ? (avg_velocity - 20) : 0;
        uint16_t calc2 = max_crosstalk + 20;
        rec.suggested_threshold = calc1;
        if (calc2 > rec.suggested_threshold) rec.suggested_threshold = calc2;
        if (safe_minimum > rec.suggested_threshold) rec.suggested_threshold = safe_minimum;
        rec.message = "Threshold too LOW\nRaise to: " + std::to_string(rec.suggested_threshold);
    } else {
        rec.needs_adjustment = false;
        rec.suggested_threshold = current_threshold;
        rec.message = "Perfect!\nNo adjustment needed";
    }
    
    if (max_crosstalk > 30) {
        rec.message += "\n\nCross-talk: " + std::to_string(max_crosstalk);
    }
    
    m_taikotune_state.recommendations[target_pad] = rec;
    
    applyTaikoTuneRecommendation(target_pad);
    
    m_taikotune_state.current_mode = TaikoTuneState::Mode::ShowingResults;
}

void Drum::cancelTaikoTuneAnalysis() {
    // Restore backed up thresholds
    m_config.trigger_thresholds = m_taikotune_state.backup_thresholds;
    
    // Mark as cancelled and reset
    m_taikotune_state.cancel();
}

void Drum::applyTaikoTuneRecommendation(Id pad) {
    const auto& rec = m_taikotune_state.recommendations[pad];
    
    if (!rec.needs_adjustment) {
        return;
    }
    
    switch (pad) {
        case Id::KA_LEFT:
            m_config.trigger_thresholds.ka_left = rec.suggested_threshold;
            break;
        case Id::DON_LEFT:
            m_config.trigger_thresholds.don_left = rec.suggested_threshold;
            break;
        case Id::DON_RIGHT:
            m_config.trigger_thresholds.don_right = rec.suggested_threshold;
            break;
        case Id::KA_RIGHT:
            m_config.trigger_thresholds.ka_right = rec.suggested_threshold;
            break;
    }
}

} // namespace OuchiTaiko::Peripherals

// End of file Drum.cpp