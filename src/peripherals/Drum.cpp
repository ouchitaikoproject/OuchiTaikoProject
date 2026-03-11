// Beginning of file Drum.cpp

#include "peripherals/Drum.h"

#include "hardware/adc.h"
#include "pico/time.h"
#include <mcp3204/Mcp3204Dma.h>

#include <algorithm>

namespace OuchiTaiko::Peripherals {

// ============================================================================
// InternalAdc
// ============================================================================

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
            values[idx] += adc_read();
        }
    }

    std::array<uint16_t, 4> raw_result{};
    for (size_t idx = 0; idx < raw_result.size(); ++idx) {
        raw_result[idx] = static_cast<uint16_t>(values[idx] / m_config.sample_count);
    }

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

// ============================================================================
// ExternalAdc
// ============================================================================

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

// ============================================================================
// Pad - Fixed circular buffer implementation
// ============================================================================

void Drum::Pad::setState(const bool state, const uint16_t debounce_delay) {
    if (state != m_active) {
        const uint32_t now = to_ms_since_boot(get_absolute_time());
        uint32_t time_since_change = now - m_last_change;

        if (time_since_change >= debounce_delay) {
            m_active = state;
            m_last_change = now;
        }
    }
}

void Drum::Pad::addToBuffer(uint16_t value, uint16_t debounce_delay) {
    const uint32_t now = to_ms_since_boot(get_absolute_time());
    const uint32_t expiry = now - debounce_delay;

    // Evict expired entries from the front of the circular buffer
    while (m_count > 0 && m_buffer[m_head].timestamp <= expiry) {
        m_head = (m_head + 1) % BUFFER_CAPACITY;
        m_count--;
    }

    // Write new entry at tail position (wrap around if full, overwriting oldest)
    const uint8_t tail = (m_head + m_count) % BUFFER_CAPACITY;
    m_buffer[tail] = {value, now};
    if (m_count < BUFFER_CAPACITY) {
        m_count++;
    } else {
        // Buffer full: advance head to discard oldest entry
        m_head = (m_head + 1) % BUFFER_CAPACITY;
    }
}

uint16_t Drum::Pad::getMaxValueInBuffer() const {
    if (m_count == 0) {
        return 0;
    }
    uint16_t max_val = 0;
    for (uint8_t i = 0; i < m_count; ++i) {
        uint16_t v = m_buffer[(m_head + i) % BUFFER_CAPACITY].value;
        if (v > max_val) max_val = v;
    }
    return max_val;
}

// Static helper: convert a known raw 12-bit value to a gained 16-bit analog value.
// Separated from getAnalog() so updateAnalogInputState() can reuse the already-computed
// getMaxValueInBuffer() result instead of scanning the circular buffer a second time.
uint16_t Drum::Pad::computeAnalogFromRaw(uint16_t raw, float gain) {
    // Transform 12-bit ADC value (0-4095) to 16-bit range (0-65535)
    const uint32_t value_16bit = static_cast<uint32_t>(((raw << 4) & 0xFFF0) | ((raw >> 8) & 0x000F));

    // Apply gain multiplier (compensates for missing OpAmp circuit)
    uint32_t gained_value = static_cast<uint32_t>(value_16bit * gain);

    // Clamp to 16-bit max to prevent overflow
    if (gained_value > 65535) {
        gained_value = 65535;
    }

    return static_cast<uint16_t>(gained_value);
}

uint16_t Drum::Pad::getAnalog(float gain) const {
    return computeAnalogFromRaw(getMaxValueInBuffer(), gain);
}

// ============================================================================
// RollCounter
// ============================================================================

Drum::RollCounter::RollCounter(uint32_t timeout_ms)
    : m_timeout_ms(timeout_ms), m_last_update(0), m_roll_count(0), m_previous_roll(0) {}

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

// ============================================================================
// Drum constructor
// ============================================================================

Drum::Drum(const Config &config)
    : m_config(config),
      m_pads{Pad(config.adc_channels.don_left),
             Pad(config.adc_channels.ka_left),
             Pad(config.adc_channels.don_right),
             Pad(config.adc_channels.ka_right)},
      m_roll_counter(config.roll_counter_timeout_ms)
{
    std::visit(
        [this](auto &&adc_cfg) {
            using T = std::decay_t<decltype(adc_cfg)>;

            if constexpr (std::is_same_v<T, Config::InternalAdc>) {
                m_adc = std::make_unique<InternalAdc>(adc_cfg);
            } else if constexpr (std::is_same_v<T, Config::ExternalAdc>) {
                m_adc = std::make_unique<ExternalAdc>(adc_cfg);
            } else {
                static_assert(false, "Unknown ADC type!");
            }
        },
        m_config.adc_config);
}

// ============================================================================
// readInputs - returns stack-allocated array, zero heap allocation
// Index mapping: [0]=DON_LEFT, [1]=KA_LEFT, [2]=DON_RIGHT, [3]=KA_RIGHT
// ============================================================================

std::array<uint16_t, 4> Drum::readInputs() {
    const auto adc_values = m_adc->read();

    std::array<uint16_t, 4> result{};
    for (size_t i = 0; i < 4; ++i) {
        result[i] = adc_values[m_pads[i].getChannel()];
    }

    return result;
}

// ============================================================================
// OUCHITAIKO SPECIAL: THRESHOLD-FIRST HIT DETECTION
// DO NOT REMOVE OR REPLACE
// ============================================================================

void Drum::updateDigitalInputState(Utils::InputState &input_state, const std::array<uint16_t, 4> &raw_values) {

    // Threshold lookup by array index (matches Id enum values)
    const uint16_t thresholds[4] = {
        m_config.trigger_thresholds.don_left,   // [0] DON_LEFT
        m_config.trigger_thresholds.ka_left,    // [1] KA_LEFT
        m_config.trigger_thresholds.don_right,  // [2] DON_RIGHT
        m_config.trigger_thresholds.ka_right,   // [3] KA_RIGHT
    };

    // ============================================================================
    // OUCHITAIKO: GLOBAL DON/KA ZONE ARBITRATION — PRE-THRESHOLD
    //
    // In Taiko no Tatsujin there are exactly 4 note types:
    //   Small Don, Small Ka, Big Don (both dons), Big Ka (both kas).
    // Don and Ka are GLOBALLY mutually exclusive — no valid note ever requires
    // both zones active simultaneously. Arbitrate on RAW values BEFORE threshold
    // so crosstalk bleed from the losing zone is zeroed before it can ghost-fire.
    // Calibration becomes a secondary safety net, not the primary defence.
    // ============================================================================

    // STEP 1: Global zone arbitration on raw values.
    //         Whichever zone (Don or Ka) has the highest single sensor reading wins.
    //         The entire losing zone is zeroed before threshold filtering runs.
    uint16_t raw_max_don = (raw_values[0] > raw_values[2]) ? raw_values[0] : raw_values[2];
    uint16_t raw_max_ka  = (raw_values[1] > raw_values[3]) ? raw_values[1] : raw_values[3];

    std::array<uint16_t, 4> working = raw_values;
    if (raw_max_don >= raw_max_ka) {
        working[1] = 0;  // KA_LEFT  zeroed — Don zone wins
        working[3] = 0;  // KA_RIGHT zeroed
    } else {
        working[0] = 0;  // DON_LEFT  zeroed — Ka zone wins
        working[2] = 0;  // DON_RIGHT zeroed
    }

    // STEP 2: Apply thresholds to winning zone only
    std::array<uint16_t, 4> filtered{};
    for (size_t i = 0; i < 4; ++i) {
        filtered[i] = (working[i] > thresholds[i]) ? working[i] : 0;
    }

    // STEP 3: Twin pad logic within winning zone — suppress weaker sensor
    //         if it is less than half the stronger one (rejects single-side bleed).
    // DON pair: indices 0 (DON_LEFT) and 2 (DON_RIGHT)
    // KA pair:  indices 1 (KA_LEFT)  and 3 (KA_RIGHT)
    const auto zero_if_not_within_twin = [](std::array<uint16_t, 4> &values, size_t a, size_t b) {
        if (values[a] == 0 || values[b] == 0) return;
        if (values[a] > values[b]) {
            if (values[b] < (values[a] >> 1)) values[b] = 0;
        } else {
            if (values[a] < (values[b] >> 1)) values[a] = 0;
        }
    };

    zero_if_not_within_twin(filtered, 0, 2);  // DON_LEFT vs DON_RIGHT
    zero_if_not_within_twin(filtered, 1, 3);  // KA_LEFT  vs KA_RIGHT

    // STEP 4: Set pad states based on FILTERED values.
    // Pads in the WINNING zone use normal debounce.
    // Pads in the LOSING zone are forced inactive immediately — bypassing debounce.
    // This prevents a debounce-held active state from coexisting with the new winning zone.
    bool don_won = (raw_max_don >= raw_max_ka);
    for (size_t i = 0; i < 4; ++i) {
        bool in_don_zone = (i == 0 || i == 2);  // DON_LEFT=0, DON_RIGHT=2
        bool is_winning_zone = don_won ? in_don_zone : !in_don_zone;
        if (is_winning_zone) {
            m_pads[i].setState(filtered[i] != 0, m_config.debounce_delay_ms);
        } else {
            // Losing zone: force inactive with zero debounce — arbitration overrides
            m_pads[i].setState(false, 0);
        }
    }
    // ============================================================================
    // END OUCHITAIKO SPECIAL HIT DETECTION
    // ============================================================================

    // STEP 5: Roll counter (detect rising edges)
    bool don_left_rising  = !input_state.drum.don_left.triggered  && m_pads[idToIndex(Id::DON_LEFT)].getState();
    bool don_right_rising = !input_state.drum.don_right.triggered && m_pads[idToIndex(Id::DON_RIGHT)].getState();
    bool ka_left_rising   = !input_state.drum.ka_left.triggered   && m_pads[idToIndex(Id::KA_LEFT)].getState();
    bool ka_right_rising  = !input_state.drum.ka_right.triggered  && m_pads[idToIndex(Id::KA_RIGHT)].getState();

    if (don_left_rising || don_right_rising || ka_left_rising || ka_right_rising) {
        m_roll_counter.addHit();
    }

    // STEP 6: Write final states to input_state
    input_state.drum.don_left.triggered  = m_pads[idToIndex(Id::DON_LEFT)].getState();
    input_state.drum.ka_left.triggered   = m_pads[idToIndex(Id::KA_LEFT)].getState();
    input_state.drum.don_right.triggered = m_pads[idToIndex(Id::DON_RIGHT)].getState();
    input_state.drum.ka_right.triggered  = m_pads[idToIndex(Id::KA_RIGHT)].getState();

    input_state.drum.current_roll  = m_roll_counter.getRoll();
    input_state.drum.previous_roll = m_roll_counter.getPreviousRoll();
}

void Drum::updateAnalogInputState(Utils::InputState &input_state, const std::array<uint16_t, 4> &raw_values) {
    for (size_t i = 0; i < 4; ++i) {
        m_pads[i].addToBuffer(raw_values[i], m_config.debounce_delay_ms);
    }

    // Scan each pad's circular buffer exactly once, then compute both raw and analog from that result.
    // Previously getMaxValueInBuffer() and getAnalog() each scanned the buffer separately (2x per pad = 8 scans).
    const auto applyPad = [&](Utils::InputState::Drum::Pad &out, Id id) {
        const uint16_t raw = m_pads[idToIndex(id)].getMaxValueInBuffer();
        out.raw    = raw;
        out.analog = m_pads[idToIndex(id)].computeAnalogFromRaw(raw, m_config.analog_gain);
    };

    applyPad(input_state.drum.don_left,  Id::DON_LEFT);
    applyPad(input_state.drum.ka_left,   Id::KA_LEFT);
    applyPad(input_state.drum.don_right, Id::DON_RIGHT);
    applyPad(input_state.drum.ka_right,  Id::KA_RIGHT);
}

void Drum::updateInputState(Utils::InputState &input_state, usb_mode_t usb_mode) {
    const auto raw_values = readInputs();

    if (m_tantrum_state.isActive()) {
        updateTaikoTantrum(raw_values);
    }

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

void Drum::updateTaikoTantrum(const std::array<uint16_t, 4> &raw_values) {
    if (!m_tantrum_state.isActive()) {
        return;
    }

    uint32_t now = to_ms_since_boot(get_absolute_time());

    if (m_tantrum_state.current_mode == TantrumState::Mode::Instructions) {
        return;  // Wait for external trigger via startCountdown()
    }

    if (m_tantrum_state.current_mode == TantrumState::Mode::Countdown) {
        uint32_t elapsed = now - m_tantrum_state.countdown_start;
        if (elapsed >= TantrumState::COUNTDOWN_DURATION_MS) {
            m_tantrum_state.startCalibration();
        }
        return;
    }

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

void Drum::processCalibrationData(const std::array<uint16_t, 4> &raw_values) {
    uint32_t now = to_ms_since_boot(get_absolute_time());

    // Find which sensor has the strongest signal
    uint8_t strongest_idx = 0;
    uint16_t max_value = 0;
    for (uint8_t i = 0; i < 4; ++i) {
        if (raw_values[i] > max_value) {
            max_value = raw_values[i];
            strongest_idx = i;
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

    m_tantrum_state.last_hit_time = now;
    m_tantrum_state.total_hits_detected++;

    // Track maximum value for the strongest sensor (intentional hit)
    if (max_value > m_tantrum_state.max_hit_value[strongest_idx]) {
        m_tantrum_state.max_hit_value[strongest_idx] = max_value;
    }

    // Track maximum crosstalk for all OTHER sensors
    for (uint8_t i = 0; i < 4; ++i) {
        if (i != strongest_idx && raw_values[i] > m_tantrum_state.max_crosstalk_to[i]) {
            m_tantrum_state.max_crosstalk_to[i] = raw_values[i];
        }
    }
}

void Drum::finishTaikoTantrum() {
    m_tantrum_state.current_mode = TantrumState::Mode::ShowingResults;

    // VALIDATION: Check that user hit hard enough on all sensors
    for (uint8_t i = 0; i < 4; ++i) {
        if (m_tantrum_state.max_hit_value[i] < TantrumState::MIN_ACCEPTABLE_MAX) {
            m_tantrum_state.current_mode = TantrumState::Mode::NeedsRedo;
            m_tantrum_state.needs_redo = true;
            m_tantrum_state.redo_reason = "Not all pads hit\nhard enough (min 300)";  // string literal — no allocation
            return;
        }
    }

    // CALCULATE THRESHOLDS: Use HIGHER of crosstalk-based OR hit-strength-based threshold
    for (uint8_t i = 0; i < 4; ++i) {
        uint16_t max_crosstalk = m_tantrum_state.max_crosstalk_to[i];
        uint16_t max_hit       = m_tantrum_state.max_hit_value[i];
        
        uint16_t crosstalk_threshold   = max_crosstalk + TantrumState::SAFETY_MARGIN;
        uint16_t hit_strength_threshold = (max_hit * 30) / 100;  // 30% of max hit (matches web tool)

        uint16_t final_threshold =
            (crosstalk_threshold > hit_strength_threshold) ? crosstalk_threshold : hit_strength_threshold;

        // SAFETY CAP: Never recommend a threshold above 50% of max hit strength.
        // If crosstalk is so bad the math would exceed this, the pad would become
        // unresponsive. Cap it and force the high-crosstalk warning instead.
        uint16_t max_safe_threshold = max_hit >> 1;  // 50% of hit strength
        if (final_threshold > max_safe_threshold) {
            final_threshold = max_safe_threshold;
            m_tantrum_state.high_crosstalk_warning = true;
        }

        m_tantrum_state.recommended_thresholds[i] = final_threshold;

        // CROSSTALK WARNING: if crosstalk > 50% of hit strength
        if (max_crosstalk > (max_hit >> 1)) {
            m_tantrum_state.high_crosstalk_warning = true;
        }
    }
}

void Drum::cancelTaikoTantrum() {
    m_tantrum_state.reset();
}

void Drum::applyTantrumRecommendations() {
    if (!m_tantrum_state.hasRecommendations()) {
        return;
    }

    // Index mapping: [0]=DON_LEFT, [1]=KA_LEFT, [2]=DON_RIGHT, [3]=KA_RIGHT
    m_config.trigger_thresholds.don_left  = m_tantrum_state.recommended_thresholds[idToIndex(Id::DON_LEFT)];
    m_config.trigger_thresholds.ka_left   = m_tantrum_state.recommended_thresholds[idToIndex(Id::KA_LEFT)];
    m_config.trigger_thresholds.don_right = m_tantrum_state.recommended_thresholds[idToIndex(Id::DON_RIGHT)];
    m_config.trigger_thresholds.ka_right  = m_tantrum_state.recommended_thresholds[idToIndex(Id::KA_RIGHT)];
}

} // namespace OuchiTaiko::Peripherals

// End of file Drum.cpp
