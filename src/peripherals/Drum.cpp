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

bool Drum::Pad::hasSharpVelocityRise(uint16_t current_value) const {
    // Need at least 2 samples to calculate rise rate
    if (m_buffer.size() < 2) {
        return false;  // Not enough data, assume sharp (allow hit)
    }

    // Get the most recent samples to calculate dV/dt
    const BufferEntry& latest = m_buffer.back();
    const BufferEntry& previous = m_buffer[m_buffer.size() - 2];

    uint32_t time_delta = latest.timestamp - previous.timestamp;

    // Avoid division by zero
    if (time_delta == 0) {
        return true;  // Same timestamp, assume sharp
    }

    int32_t value_delta = static_cast<int32_t>(current_value) - static_cast<int32_t>(previous.value);

    // Calculate rise rate: dV/dt (value change per millisecond)
    // Real hits have sharp spikes: typically > 50 units/ms
    // Bounce/resonance has slow rise: typically < 20 units/ms
    float rise_rate = static_cast<float>(value_delta) / static_cast<float>(time_delta);

    // Thresholds for sharp rise detection:
    // - EXTREME mode: 30 units/ms (very aggressive, allows fast rolls)
    // - COMPETITIVE mode: 20 units/ms (balanced)
    // - STANDARD mode: Not used (pure time-based)
    constexpr float SHARP_RISE_THRESHOLD = 20.0f;

    return rise_rate >= SHARP_RISE_THRESHOLD;
}

void Drum::Pad::setState(const bool state, const uint16_t debounce_delay, PerformanceProfile profile) {
    if (state != m_active) {
        const uint32_t now = to_ms_since_boot(get_absolute_time());
        uint32_t time_since_change = now - m_last_change;

        // STANDARD mode: Pure time-based debounce (original behavior)
        if (profile == PerformanceProfile::STANDARD) {
            if (time_since_change >= debounce_delay) {
                m_active = state;
                m_last_change = now;
            }
            return;
        }

        // COMPETITIVE & EXTREME modes: Velocity-aware smart triggering
        // Get effective debounce time based on profile
        uint16_t min_debounce_time;
        if (profile == PerformanceProfile::EXTREME) {
            min_debounce_time = 8;  // 120 hits/sec capable
        } else {  // COMPETITIVE
            min_debounce_time = 12;  // ~83 hits/sec capable
        }

        // Allow state change if EITHER:
        // 1. Full debounce time has elapsed (safe fallback), OR
        // 2. Minimum time has passed AND velocity rise is sharp (real hit detected)
        bool safe_time_elapsed = time_since_change >= debounce_delay;
        bool min_time_and_sharp = (time_since_change >= min_debounce_time) &&
                                  (state && hasSharpVelocityRise(getMaxValueInBuffer()));

        if (safe_time_elapsed || min_time_and_sharp) {
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

    // Bit-shift FIRST to get full 16-bit range
    uint32_t value_16bit = raw_to_uint16(baseline_subtracted);

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

    const auto is_over_threshold = [&](Id target) {
        return (raw_values.at(target) > get_threshold(target));
    };

    const auto resolve_twin_pads = [&](Id left, Id right) {
        // If neither pad is over threshold, set both to false
        if (!is_over_threshold(left) && !is_over_threshold(right)) {
            m_pads.at(left).setState(false, m_config.debounce_delay_ms, m_config.performance_profile);
            m_pads.at(right).setState(false, m_config.debounce_delay_ms, m_config.performance_profile);
            return;
        }

        // Trigger twin pad if within 50% of hit strength to allow
        // simultaneous hits while still rejecting unintended double hits.
        if (raw_values.at(left) > raw_values.at(right)) {
            m_pads.at(left).setState(true, m_config.debounce_delay_ms, m_config.performance_profile);

            if (raw_values.at(right) > (raw_values.at(left) >> 1)) {
                m_pads.at(right).setState(true, m_config.debounce_delay_ms, m_config.performance_profile);
            } else {
                m_pads.at(right).setState(false, m_config.debounce_delay_ms, m_config.performance_profile);
            }
        } else {
            m_pads.at(right).setState(true, m_config.debounce_delay_ms, m_config.performance_profile);

            if (raw_values.at(left) > (raw_values.at(right) >> 1)) {
                m_pads.at(left).setState(true, m_config.debounce_delay_ms, m_config.performance_profile);
            } else {
                m_pads.at(left).setState(false, m_config.debounce_delay_ms, m_config.performance_profile);
            }
        }
    };

    // DON vs KA CROSSTALK SUPPRESSION (from original DonCon2040)
    // Compare RAW values BEFORE thresholds to determine which type is stronger
    // This prevents crosstalk from ever being registered, regardless of threshold settings
    if (std::max(raw_values.at(Id::DON_LEFT), raw_values.at(Id::DON_RIGHT)) >
        std::max(raw_values.at(Id::KA_LEFT), raw_values.at(Id::KA_RIGHT))) {

        // DON is stronger - process don pads, suppress ka pads
        resolve_twin_pads(Id::DON_LEFT, Id::DON_RIGHT);

        m_pads.at(Id::KA_LEFT).setState(false, m_config.debounce_delay_ms, m_config.performance_profile);
        m_pads.at(Id::KA_RIGHT).setState(false, m_config.debounce_delay_ms, m_config.performance_profile);
    } else {
        // KA is stronger - process ka pads, suppress don pads
        resolve_twin_pads(Id::KA_LEFT, Id::KA_RIGHT);

        m_pads.at(Id::DON_LEFT).setState(false, m_config.debounce_delay_ms, m_config.performance_profile);
        m_pads.at(Id::DON_RIGHT).setState(false, m_config.debounce_delay_ms, m_config.performance_profile);
    }

    if (m_config.big_hit_enable) {
        if (raw_values.at(Id::DON_LEFT) > m_config.big_hit_threshold) {
            m_pads.at(Id::DON_RIGHT).setState(true, m_config.debounce_delay_ms, m_config.performance_profile);
        }
        if (raw_values.at(Id::DON_RIGHT) > m_config.big_hit_threshold) {
            m_pads.at(Id::DON_LEFT).setState(true, m_config.debounce_delay_ms, m_config.performance_profile);
        }
        if (raw_values.at(Id::KA_LEFT) > m_config.big_hit_threshold) {
            m_pads.at(Id::KA_RIGHT).setState(true, m_config.debounce_delay_ms, m_config.performance_profile);
        }
        if (raw_values.at(Id::KA_RIGHT) > m_config.big_hit_threshold) {
            m_pads.at(Id::KA_LEFT).setState(true, m_config.debounce_delay_ms, m_config.performance_profile);
        }
    }

    bool don_left_rising = !input_state.drum.don_left.triggered && m_pads.at(Id::DON_LEFT).getState();
    bool don_right_rising = !input_state.drum.don_right.triggered && m_pads.at(Id::DON_RIGHT).getState();
    bool ka_left_rising = !input_state.drum.ka_left.triggered && m_pads.at(Id::KA_LEFT).getState();
    bool ka_right_rising = !input_state.drum.ka_right.triggered && m_pads.at(Id::KA_RIGHT).getState();

    if (don_left_rising || don_right_rising || ka_left_rising || ka_right_rising) {
        m_roll_counter.addHit();
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

void Drum::setBigHitEnable(bool enable) { m_config.big_hit_enable = enable; }

void Drum::setBigHitThreshold(uint16_t threshold) { m_config.big_hit_threshold = threshold; }

void Drum::setSimulTap(bool enable) { m_config.enable_simultap = enable; }

void Drum::setPerformanceProfile(PerformanceProfile profile) { m_config.performance_profile = profile; }

// ============================================================================
// TAIKO TANTRUM CALIBRATION - Stress test for worst-case crosstalk
// ============================================================================

void Drum::startTaikoTantrum() {
    m_tantrum_state.startCountdown();
}

void Drum::updateTaikoTantrum(const std::map<Id, uint16_t> &raw_values) {
    if (!m_tantrum_state.isActive()) {
        return;
    }

    uint32_t now = to_ms_since_boot(get_absolute_time());

    // COUNTDOWN PHASE: Transition to recording when countdown expires
    if (m_tantrum_state.current_mode == TantrumState::Mode::Countdown) {
        uint32_t elapsed = now - m_tantrum_state.countdown_start;
        if (elapsed >= TantrumState::COUNTDOWN_DURATION_MS) {
            m_tantrum_state.startRecording();
        }
        return;
    }

    // RECORDING PHASE: Track maximum hits and crosstalk
    if (m_tantrum_state.current_mode == TantrumState::Mode::Recording) {
        uint32_t elapsed = now - m_tantrum_state.recording_start;

        // Check if recording finished
        if (elapsed >= TantrumState::RECORDING_DURATION_MS) {
            finishTaikoTantrum();
            return;
        }

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
        static uint32_t last_hit_time = 0;
        if ((now - last_hit_time) < TantrumState::HIT_COOLDOWN_MS) {
            return;
        }

        // Record this as a hit
        last_hit_time = now;
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

    // CALCULATE THRESHOLDS: Maximum crosstalk + safety margin
    for (auto id : {Id::DON_LEFT, Id::DON_RIGHT, Id::KA_LEFT, Id::KA_RIGHT}) {
        uint16_t max_crosstalk = m_tantrum_state.max_crosstalk_to[id];
        uint16_t safe_threshold = max_crosstalk + TantrumState::SAFETY_MARGIN;

        m_tantrum_state.recommended_thresholds[id] = safe_threshold;

        // CROSSTALK WARNING: Check if crosstalk is >50% of hit strength
        uint16_t max_hit = m_tantrum_state.max_hit_value[id];
        float crosstalk_ratio = static_cast<float>(max_crosstalk) / static_cast<float>(max_hit);
        if (crosstalk_ratio > TantrumState::MAX_CROSSTALK_RATIO) {
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