// Beginning of file Drum.cpp

#include "peripherals/Drum.h"

#include "hardware/adc.h"
#include "pico/time.h"
#include <mcp3204/Mcp3204Dma.h>

#include <algorithm>
#include <cstdio>

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
//
// ORDER IS CRITICAL: Thresholds MUST run before zone arbitration.
// Running arbitration on raw values causes idle ADC noise to randomly zero
// entire zones every frame, killing valid hits via forced setState(false,0).
// Threshold-first means quiet frames are never touched at all.
// ============================================================================

void Drum::updateDigitalInputState(Utils::InputState &input_state, const std::array<uint16_t, 4> &raw_values) {

    // Threshold lookup by array index (matches Id enum values)
    const uint16_t thresholds[4] = {
        m_config.trigger_thresholds.don_left,   // [0] DON_LEFT
        m_config.trigger_thresholds.ka_left,    // [1] KA_LEFT
        m_config.trigger_thresholds.don_right,  // [2] DON_RIGHT
        m_config.trigger_thresholds.ka_right,   // [3] KA_RIGHT
    };

    // STEP 1: Apply thresholds FIRST.
    // Anything below threshold is zeroed. Quiet frames produce all-zero filtered[],
    // and nothing downstream runs -- no arbitration, no pad state changes, no debounce churn.
    std::array<uint16_t, 4> filtered{};
    for (size_t i = 0; i < 4; ++i) {
        filtered[i] = (raw_values[i] > thresholds[i]) ? raw_values[i] : 0;
    }

    // STEP 2: Twin pad suppression on filtered values.
    // If one side of a Don or Ka pair is less than half the other, suppress the weaker side.
    // This rejects single-sensor bleed while allowing genuine Big Don / Big Ka hits through.
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

    // STEP 3: Global Don/Ka zone arbitration on FILTERED values only.
    // In Taiko no Tatsujin, Don and Ka are mutually exclusive -- no valid note ever
    // requires both zones simultaneously. If both zones cleared threshold, the weaker
    // one is crosstalk; suppress it.
    // Because this runs on post-threshold values, it only fires when there is a real
    // competing signal -- never on idle noise.
    uint16_t max_don = (filtered[0] > filtered[2]) ? filtered[0] : filtered[2];
    uint16_t max_ka  = (filtered[1] > filtered[3]) ? filtered[1] : filtered[3];

    if (max_don > 0 && max_ka > 0) {
        // Both zones have a real above-threshold signal -- suppress the weaker zone entirely
        if (max_don >= max_ka) {
            filtered[1] = 0;  // KA_LEFT  zeroed -- Don zone wins
            filtered[3] = 0;  // KA_RIGHT zeroed
        } else {
            filtered[0] = 0;  // DON_LEFT  zeroed -- Ka zone wins
            filtered[2] = 0;  // DON_RIGHT zeroed
        }
    }

    // STEP 4: Set pad states with normal debounce.
    // All pads use the same debounce path -- no forced setState(false,0) bypass needed
    // because arbitration now only runs when real signals are present.
    for (size_t i = 0; i < 4; ++i) {
        m_pads[i].setState(filtered[i] != 0, m_config.debounce_delay_ms);
    }
    // ============================================================================
    // END OUCHITAIKO SPECIAL HIT DETECTION
    // ============================================================================

    // STEP 5: Roll counter -- detect rising edges
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
// GUIDED CALIBRATION WIZARD - Per-pad, per-step (replaces old random-hit Tantrum)
//
// Pad order: DON_LEFT(0), DON_RIGHT(2), KA_LEFT(1), KA_RIGHT(3)
// Per pad: Step 0=Normal x3, Step 1=Hard x3, Step 2=Roll 3s
// Algorithm matches web tool exactly (median, 45/55% caps, crosstalk margins)
// ============================================================================

void Drum::startTaikoTantrum() {
    m_tantrum_state.startWelcome();
}

// Called by Main.cpp A-button handler to advance through wizard steps.
// PadNormal, PadHard, and PadRoll are NOT skippable -- they must complete
// naturally (3 hits detected / roll timer expires). A is only honoured on
// screens where the user needs to explicitly confirm before continuing.
void Drum::advanceCalibWizard() {
    auto& s = m_tantrum_state;
    switch (s.current_mode) {
        case TantrumState::Mode::Welcome:
            s.startPadNormal();   // Begin Step 0 for first pad
            break;
        case TantrumState::Mode::PadNormal:
        case TantrumState::Mode::PadHard:
        case TantrumState::Mode::PadRoll:
        case TantrumState::Mode::PhaseTransition:
        case TantrumState::Mode::PadDone:
        case TantrumState::Mode::Overview:
            // These phases are hit/time-driven -- A button does nothing here.
            // They auto-advance once the required hits or roll timer completes.
            break;
        case TantrumState::Mode::Error:
            // Redo current pad from Normal phase
            s.startPadNormal();
            break;
        default:
            break;
    }
}

void Drum::cancelTaikoTantrum() {
    m_tantrum_state.reset();
}

void Drum::updateTaikoTantrum(const std::array<uint16_t, 4> &raw_values) {
    auto& s = m_tantrum_state;
    if (!s.isActive()) return;

    uint32_t now     = to_ms_since_boot(get_absolute_time());
    uint8_t  padIdx  = s.currentPadIndex();

    switch (s.current_mode) {

    case TantrumState::Mode::Welcome:
    case TantrumState::Mode::Error:
        // Waiting for user input (A/B) -- handled in advanceCalibWizard / cancelTaikoTantrum
        break;

    case TantrumState::Mode::PadDone: {
        if ((now - s.phase_start) >= TantrumState::PAD_DONE_DISPLAY_MS) {
            _advanceToNextPad();
        }
        break;
    }

    case TantrumState::Mode::Overview: {
        if ((now - s.phase_start) >= TantrumState::OVERVIEW_DISPLAY_MS) {
            s.startSaving();
        }
        break;
    }

    case TantrumState::Mode::PadNormal:
    case TantrumState::Mode::PadHard: {
        // Auto-advance to next step once we have 3 hits on the target pad
        if (s.hit_count >= 3) {
            if (s.current_mode == TantrumState::Mode::PadNormal) {
                s.normal_ref[padIdx] = s.medianOfHits();
                s.startPhaseTransition("Strong hits", TantrumState::Mode::PadHard);
            } else {
                s.max_hit[padIdx] = s.medianOfHits();
                // Validate before roll
                if (s.max_hit[padIdx] < TantrumState::MIN_ACCEPTABLE_MAX) {
                    s.error_msg = "Hit too soft!";
                    s.current_mode = TantrumState::Mode::Error;
                } else if (s.max_hit[padIdx] <= (s.normal_ref[padIdx] + TantrumState::MIN_STRONG_DELTA)) {
                    s.error_msg = "Need harder strong hits";
                    s.current_mode = TantrumState::Mode::Error;
                } else {
                    s.startPhaseTransition("Rapid hits", TantrumState::Mode::PadRoll);
                }
            }
            break;
        }
        // Detect hits on target pad
        uint16_t target_val = raw_values[padIdx];
        if (target_val < TantrumState::MIN_HIT_STRENGTH) break;
        // Entry grace: ignore all hits for 500ms after phase starts (ADC settle after cancel/restart)
        if ((now - s.phase_start) < 500) break;
        if ((now - s.last_hit_time) < TantrumState::HIT_COOLDOWN_MS) break;
        // Must be above threshold on target, clearly stronger than neighbours
        // (simple dominance check: target > max of the other 3)
        uint16_t max_other = 0;
        for (uint8_t i = 0; i < 4; ++i) {
            if (i != padIdx && raw_values[i] > max_other) max_other = raw_values[i];
        }
        if (target_val < max_other) break;  // Another pad is stronger - not this pad
        // Record hit
        s.last_hit_time = now;
        if (s.hit_count < TantrumState::MAX_HITS) {
            s.hit_peaks[s.hit_count++] = target_val;
        }
        break;
    }

    case TantrumState::Mode::PadRoll: {
        if (!s.roll_started && (now - s.phase_start) >= TantrumState::ROLL_START_TIMEOUT_MS) {
            s.error_msg = "Start rapid hits";
            s.current_mode = TantrumState::Mode::Error;
            break;
        }
        // Start timer on first hit
        if (!s.roll_started && raw_values[padIdx] > TantrumState::MIN_HIT_STRENGTH) {
            s.roll_started = true;
            s.roll_start   = now;
        }
        // Record crosstalk into all OTHER pads during this pad's roll
        if (s.roll_started) {
            for (uint8_t i = 0; i < 4; ++i) {
                if (i == padIdx) continue;
                if (raw_values[i] > s.max_crosstalk[i]) {
                    s.max_crosstalk[i] = raw_values[i];
                }
            }
            // Auto-advance when roll duration expires
            if ((now - s.roll_start) >= TantrumState::ROLL_DURATION_MS) {
                _finishCurrentPadRoll();
            }
        }
        break;
    }

    case TantrumState::Mode::PhaseTransition: {
        // Auto-advance after 2s -- no user input accepted during this window
        if ((now - s.phase_start) >= TantrumState::PHASE_TRANSITION_DELAY_MS) {
            if (s.transition_next_mode == TantrumState::Mode::PadHard) {
                s.startPadHard();
            } else {
                s.startPadRoll();
            }
        }
        break;
    }

    case TantrumState::Mode::Saving: {
        if ((now - s.phase_start) >= TantrumState::SAVING_DISPLAY_MS) {
            s.startComplete();
        }
        break;
    }

    case TantrumState::Mode::Complete: {
        if ((now - s.phase_start) >= TantrumState::COMPLETE_DISPLAY_MS) {
            s.current_mode = TantrumState::Mode::Inactive;
        }
        break;
    }

    default:
        break;
    }
}

// Internal: finish roll for current pad (crosstalk accumulation complete for this pad's roll).
// NOTE: threshold computation is intentionally deferred to _advanceToNextPad() / startOverview()
// so that max_crosstalk[] is fully populated across ALL pads before any threshold is calculated.
void Drum::_finishCurrentPadRoll() {
    auto& s = m_tantrum_state;
    uint8_t padIdx = s.currentPadIndex();
    s.pad_done[padIdx] = true;
    s.startPadDone();
}

// Internal: after PadDone screen, move to next pad or Overview
void Drum::_advanceToNextPad() {
    auto& s = m_tantrum_state;
    s.current_pad++;
    if (s.current_pad >= 4) {
        // All pads done -- now safe to compute all thresholds since max_crosstalk[] is fully populated
        s.computeAllThresholds();
        s.startOverview();
    } else {
        s.startPadNormal();
    }
}

void Drum::applyTantrumRecommendations() {
    if (!m_tantrum_state.hasRecommendations()) return;
    m_config.trigger_thresholds.don_left  = m_tantrum_state.recommended_thresholds[idToIndex(Id::DON_LEFT)];
    m_config.trigger_thresholds.ka_left   = m_tantrum_state.recommended_thresholds[idToIndex(Id::KA_LEFT)];
    m_config.trigger_thresholds.don_right = m_tantrum_state.recommended_thresholds[idToIndex(Id::DON_RIGHT)];
    m_config.trigger_thresholds.ka_right  = m_tantrum_state.recommended_thresholds[idToIndex(Id::KA_RIGHT)];

    m_last_tantrum_report_version++;
    snprintf(
        m_last_tantrum_report,
        sizeof(m_last_tantrum_report),
        "VER=%lu;"
        "NR=%u,%u,%u,%u;"
        "MH=%u,%u,%u,%u;"
        "XT=%u,%u,%u,%u;"
        "TH=%u,%u,%u,%u;"
        "WARN=%u",
        static_cast<unsigned long>(m_last_tantrum_report_version),
        static_cast<unsigned>(m_tantrum_state.normal_ref[0]),
        static_cast<unsigned>(m_tantrum_state.normal_ref[1]),
        static_cast<unsigned>(m_tantrum_state.normal_ref[2]),
        static_cast<unsigned>(m_tantrum_state.normal_ref[3]),
        static_cast<unsigned>(m_tantrum_state.max_hit[0]),
        static_cast<unsigned>(m_tantrum_state.max_hit[1]),
        static_cast<unsigned>(m_tantrum_state.max_hit[2]),
        static_cast<unsigned>(m_tantrum_state.max_hit[3]),
        static_cast<unsigned>(m_tantrum_state.max_crosstalk[0]),
        static_cast<unsigned>(m_tantrum_state.max_crosstalk[1]),
        static_cast<unsigned>(m_tantrum_state.max_crosstalk[2]),
        static_cast<unsigned>(m_tantrum_state.max_crosstalk[3]),
        static_cast<unsigned>(m_tantrum_state.recommended_thresholds[0]),
        static_cast<unsigned>(m_tantrum_state.recommended_thresholds[1]),
        static_cast<unsigned>(m_tantrum_state.recommended_thresholds[2]),
        static_cast<unsigned>(m_tantrum_state.recommended_thresholds[3]),
        m_tantrum_state.high_crosstalk_warning ? 1u : 0u);
}
} // namespace OuchiTaiko::Peripherals

// End of file Drum.cpp
