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
        
        // ============================================================================
        // OUCHITAIKO SPECIAL: ADAPTIVE BASELINE TRACKING
        // DO NOT REMOVE - This continuously adjusts for sensor drift and temperature changes
        // ============================================================================
        if (result[idx] < 20) {
            m_baseline_values[idx] = (m_baseline_values[idx] * 7 + raw_value) / 8;
        } else if (result[idx] < 100) {
            m_baseline_values[idx] = (m_baseline_values[idx] * 31 + raw_value) / 32;
        }
        // ============================================================================
        // END OUCHITAIKO SPECIAL ADAPTIVE BASELINE
        // ============================================================================
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
        uint32_t time_since_change = now - m_last_change;

        // Simple time-based debounce (original DonCon2040 behavior)
        if (time_since_change >= debounce_delay) {
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
    // Baseline tracking removed - ADC layer already handles baseline subtraction
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
    // ADC layer already handles baseline subtraction, no need to subtract again

    // Bit-shift FIRST to get full 16-bit range
    uint32_t value_16bit = raw_to_uint16(max_value);

    // Apply gain multiplier (compensates for missing OpAmp circuit)
    uint32_t gained_value = static_cast<uint32_t>(value_16bit * gain);

    // Clamp to 16-bit max to prevent overflow
    if (gained_value > 65535) {
        gained_value = 65535;
    }

    return static_cast<uint16_t>(gained_value);
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
    
    // ============================================================================
    // OUCHITAIKO SPECIAL: THRESHOLD-FIRST HIT DETECTION ALGORITHM
    // DO NOT REMOVE OR REPLACE - This is the proven working approach from 45 days ago!
    // Key principle: Filter by thresholds FIRST, then do all comparisons on filtered values
    // This eliminates crosstalk before it participates in decision-making
    // ============================================================================
    
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

    // STEP 1: Apply thresholds FIRST - create filtered map
    for (const auto &entry : raw_values) {
        uint16_t threshold = get_threshold(entry.first);
        filtered_raw_values[entry.first] = (entry.second > threshold) ? entry.second : 0;
    }

    // STEP 2: Twin pad zero logic (if one is much weaker, zero it out)
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

    // STEP 3: Don-vs-Ka crosstalk suppression on FILTERED values
    uint16_t max_don = std::max(filtered_raw_values[Id::DON_LEFT], filtered_raw_values[Id::DON_RIGHT]);
    uint16_t max_ka = std::max(filtered_raw_values[Id::KA_LEFT], filtered_raw_values[Id::KA_RIGHT]);
    
    if (max_don > 0 && max_ka > 0) {
        // Both types triggered - suppress the weaker one
        if (max_don > max_ka) {
            // Don wins - suppress Ka
            filtered_raw_values[Id::KA_LEFT] = 0;
            filtered_raw_values[Id::KA_RIGHT] = 0;
        } else {
            // Ka wins - suppress Don
            filtered_raw_values[Id::DON_LEFT] = 0;
            filtered_raw_values[Id::DON_RIGHT] = 0;
        }
    }

    // STEP 4: Set pad states based on FILTERED values
    for (const auto &entry : filtered_raw_values) {
        m_pads.at(entry.first).setState(entry.second != 0, m_config.debounce_delay_ms);
    }
    // ============================================================================
    // END OUCHITAIKO SPECIAL HIT DETECTION
    // ============================================================================

    // STEP 5: Roll counter (detect rising edges)
    bool don_left_rising = !input_state.drum.don_left.triggered && m_pads.at(Id::DON_LEFT).getState();
    bool don_right_rising = !input_state.drum.don_right.triggered && m_pads.at(Id::DON_RIGHT).getState();
    bool ka_left_rising = !input_state.drum.ka_left.triggered && m_pads.at(Id::KA_LEFT).getState();
    bool ka_right_rising = !input_state.drum.ka_right.triggered && m_pads.at(Id::KA_RIGHT).getState();

    if (don_left_rising || don_right_rising || ka_left_rising || ka_right_rising) {
        m_roll_counter.addHit();
    }

    // STEP 6: Write final states to input_state
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

void Drum::updateInputState(Utils::InputState &input_state, usb_mode_t usb_mode) {
    const auto raw_values = readInputs();

    if (m_tantrum_state.isActive()) {
        updateTaikoTantrum(raw_values);
    }

    // In Xbox 360 Analog modes, skip digital threshold checking entirely
    // This prevents .triggered from being set, forcing the game to read only analog values
    const bool is_analog_mode = (usb_mode == USB_MODE_XBOX360_ANALOG_P1) ||
                                (usb_mode == USB_MODE_XBOX360_ANALOG_P2);

    if (!is_analog_mode) {
        updateDigitalInputState(input_state, raw_values);
    }

    updateAnalogInputState(input_state, raw_values);
}

void Drum::setDebounceDelay(uint16_t delay) { m_config.debounce_delay_ms = delay; }

void Drum::setTriggerThresholds(const Config::Thresholds &thresholds) { m_config.trigger_thresholds = thresholds; }

// ============================================================================
// AUTO CALIBRATE - Single 20-second calibration phase
// ============================================================================

void Drum::startTaikoTantrum() {
    m_tantrum_state.startInstructions();
}

void Drum::startTantrumCountdown() {
    m_tantrum_state.startCountdown();
}

void Drum::updateTaikoTantrum(const std::map<Id, uint16_t> &raw_values) {
    if (!m_tantrum_state.isActive()) {
        return;
    }

    uint32_t now = to_ms_since_boot(get_absolute_time());

    // WAITING FOR USER: Instructions screen - user must press A button to start
    // (This is handled externally by checking getTantrumState().current_mode and calling startCountdown)
    if (m_tantrum_state.current_mode == TantrumState::Mode::Instructions) {
        // Do nothing - wait for external trigger via startCountdown()
        return;
    }

    // PHASE 1: COUNTDOWN (3 seconds)
    if (m_tantrum_state.current_mode == TantrumState::Mode::Countdown) {
        uint32_t elapsed = now - m_tantrum_state.countdown_start;
        if (elapsed >= TantrumState::COUNTDOWN_DURATION_MS) {
            m_tantrum_state.startCalibration();
        }
        return;
    }

    // PHASE 2: CALIBRATION (20 seconds) - Mix rolls & standard hits
    if (m_tantrum_state.current_mode == TantrumState::Mode::Calibrating) {
        uint32_t elapsed = now - m_tantrum_state.calibration_start;
        if (elapsed >= TantrumState::CALIBRATION_DURATION_MS) {
            finishTaikoTantrum();
            return;
        }
        processCalibrationData(raw_values);
        return;
    }
}

void Drum::processCalibrationData(const std::map<Id, uint16_t> &raw_values) {
    uint32_t now = to_ms_since_boot(get_absolute_time());

    // Find which sensor has the strongest signal
    Id strongest_sensor = Id::DON_LEFT;
    uint16_t max_value = 0;
    for (const auto& [sensor_id, value] : raw_values) {
        if (value > max_value) {
            max_value = value;
            strongest_sensor = sensor_id;
        }
    }

    // Only process if strongest sensor is above minimum hit strength
    if (max_value < TantrumState::MIN_HIT_STRENGTH) {
        return;
    }

    // Hit cooldown to prevent noise spikes from being counted as separate hits
    if ((now - m_tantrum_state.last_hit_time) < TantrumState::HIT_COOLDOWN_MS) {
        return;
    }

    // Record this as a hit
    m_tantrum_state.last_hit_time = now;
    m_tantrum_state.total_hits_detected++;

    // Track maximum value for the strongest sensor (intentional hit)
    if (max_value > m_tantrum_state.max_hit_value[strongest_sensor]) {
        m_tantrum_state.max_hit_value[strongest_sensor] = max_value;
    }

    // Track maximum crosstalk for all OTHER sensors (unintentional activation)
    for (const auto& [sensor_id, value] : raw_values) {
        if (sensor_id != strongest_sensor) {
            if (value > m_tantrum_state.max_crosstalk_to[sensor_id]) {
                m_tantrum_state.max_crosstalk_to[sensor_id] = value;
            }
        }
    }
}

void Drum::finishTaikoTantrum() {
    m_tantrum_state.current_mode = TantrumState::Mode::ShowingResults;

    // VALIDATION: Check that user hit hard enough on all sensors
    bool all_sensors_hit = true;
    for (auto id : {Id::DON_LEFT, Id::DON_RIGHT, Id::KA_LEFT, Id::KA_RIGHT}) {
        if (m_tantrum_state.max_hit_value[id] < TantrumState::MIN_ACCEPTABLE_MAX) {
            all_sensors_hit = false;
            break;
        }
    }

    if (!all_sensors_hit) {
        m_tantrum_state.current_mode = TantrumState::Mode::NeedsRedo;
        m_tantrum_state.needs_redo = true;
        m_tantrum_state.redo_reason = "Not all pads hit\nhard enough (min 300)";
        return;
    }

    // CALCULATE THRESHOLDS: Use HIGHER of crosstalk-based OR hit-strength-based threshold
    // This prevents thresholds from being too low when mechanical isolation is excellent
    for (auto id : {Id::DON_LEFT, Id::DON_RIGHT, Id::KA_LEFT, Id::KA_RIGHT}) {
        uint16_t max_crosstalk = m_tantrum_state.max_crosstalk_to[id];
        uint16_t max_hit = m_tantrum_state.max_hit_value[id];
        
        // Crosstalk-based threshold: crosstalk + safety margin
        uint16_t crosstalk_threshold = max_crosstalk + TantrumState::SAFETY_MARGIN;
        
        // Hit-strength-based threshold: 35% of maximum hit strength
        // Using integer math: (max_hit * 35) / 100
        uint16_t hit_strength_threshold = (max_hit * 35) / 100;
        
        // Use whichever is HIGHER to ensure good sensitivity while preventing crosstalk
        uint16_t final_threshold = (crosstalk_threshold > hit_strength_threshold) 
                                   ? crosstalk_threshold 
                                   : hit_strength_threshold;
        
        m_tantrum_state.recommended_thresholds[id] = final_threshold;

        // CROSSTALK WARNING: Check if crosstalk is >50% of hit strength
        // Use integer math: crosstalk > (max_hit / 2) instead of (crosstalk / max_hit > 0.5)
        if (max_crosstalk > (max_hit >> 1)) {  // >> 1 is divide by 2
            m_tantrum_state.high_crosstalk_warning = true;
        }
    }
}

void Drum::cancelTaikoTantrum() {
    m_tantrum_state.reset();
}

void Drum::applyTantrumRecommendations() {
    if (m_tantrum_state.recommended_thresholds.empty()) {
        return;
    }

    m_config.trigger_thresholds.don_left = m_tantrum_state.recommended_thresholds[Id::DON_LEFT];
    m_config.trigger_thresholds.don_right = m_tantrum_state.recommended_thresholds[Id::DON_RIGHT];
    m_config.trigger_thresholds.ka_left = m_tantrum_state.recommended_thresholds[Id::KA_LEFT];
    m_config.trigger_thresholds.ka_right = m_tantrum_state.recommended_thresholds[Id::KA_RIGHT];
}

} // namespace OuchiTaiko::Peripherals

// End of file Drum.cpp
