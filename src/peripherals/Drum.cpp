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

        // October-2025 working profile: fast settle near idle, slower settle
        // under moderate activity. This matches the snapshot that produced the
        // cleanest in-game behavior on the user's hardware.
        if (result[idx] < 20) {
            m_baseline_values[idx] = static_cast<uint16_t>((m_baseline_values[idx] * 7u + raw_value) / 8u);
        } else if (result[idx] < 100) {
            m_baseline_values[idx] = static_cast<uint16_t>((m_baseline_values[idx] * 31u + raw_value) / 32u);
        }
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
        if ((now - m_last_change) >= debounce_delay) {
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
    static constexpr uint32_t SAME_SIDE_LOCK_MS = 10;
    std::array<bool, 4> cross_blocked{};
    std::array<bool, 4> arb_blocked{};
    std::array<bool, 4> rising_hit{};
    const uint32_t now = to_ms_since_boot(get_absolute_time());
    const uint16_t digital_debounce_ms = m_config.debounce_delay_ms;

    const auto thresholdFor = [&](Id id) -> uint16_t {
        switch (id) {
        case Id::DON_LEFT:
            return m_config.trigger_thresholds.don_left;
        case Id::KA_LEFT:
            return m_config.trigger_thresholds.ka_left;
        case Id::DON_RIGHT:
            return m_config.trigger_thresholds.don_right;
        case Id::KA_RIGHT:
            return m_config.trigger_thresholds.ka_right;
        }
        return 0;
    };

    std::array<uint16_t, 4> filtered_raw_values{};
    for (size_t i = 0; i < filtered_raw_values.size(); ++i) {
        filtered_raw_values[i] = (raw_values[i] > thresholdFor(static_cast<Id>(i))) ? raw_values[i] : static_cast<uint16_t>(0);
    }

    for (size_t side = 0; side < m_same_side_lock_until_ms.size(); ++side) {
        if (m_same_side_lock_until_ms[side] <= now) {
            m_same_side_lock_until_ms[side] = 0;
            m_same_side_winner[side] = SideWinner::None;
        }
    }

    const std::array<bool, 4> previous_state = {
        m_pads[idToIndex(Id::DON_LEFT)].getState(),
        m_pads[idToIndex(Id::KA_LEFT)].getState(),
        m_pads[idToIndex(Id::DON_RIGHT)].getState(),
        m_pads[idToIndex(Id::KA_RIGHT)].getState()
    };

    const auto zero_if_not_within_twin = [&](Id a, Id b) {
        const size_t ai = idToIndex(a);
        const size_t bi = idToIndex(b);
        if (filtered_raw_values[ai] == 0 || filtered_raw_values[bi] == 0) {
            return;
        }

        if (filtered_raw_values[ai] > filtered_raw_values[bi]) {
            if (filtered_raw_values[bi] < (filtered_raw_values[ai] >> 1)) {
                filtered_raw_values[bi] = 0;
                cross_blocked[bi] = true;
            }
        } else {
            if (filtered_raw_values[ai] < (filtered_raw_values[bi] >> 1)) {
                filtered_raw_values[ai] = 0;
                cross_blocked[ai] = true;
            }
        }
    };

    zero_if_not_within_twin(Id::DON_LEFT, Id::DON_RIGHT);
    zero_if_not_within_twin(Id::KA_LEFT, Id::KA_RIGHT);

    const auto apply_same_side_lock = [&](size_t side, size_t don_idx, size_t ka_idx) {
        if (m_same_side_lock_until_ms[side] <= now) {
            return;
        }

        if (m_same_side_winner[side] == SideWinner::Don) {
            if (filtered_raw_values[ka_idx] != 0) {
                filtered_raw_values[ka_idx] = 0;
                arb_blocked[ka_idx] = true;
            }
        } else if (m_same_side_winner[side] == SideWinner::Ka) {
            if (filtered_raw_values[don_idx] != 0) {
                filtered_raw_values[don_idx] = 0;
                arb_blocked[don_idx] = true;
            }
        }
    };

    apply_same_side_lock(0, idToIndex(Id::DON_LEFT), idToIndex(Id::KA_LEFT));
    apply_same_side_lock(1, idToIndex(Id::DON_RIGHT), idToIndex(Id::KA_RIGHT));

    for (size_t i = 0; i < m_pads.size(); ++i) {
        m_pads[i].setState(filtered_raw_values[i] != 0, digital_debounce_ms);
    }

    const bool ka_active = m_pads[idToIndex(Id::KA_LEFT)].getState() || m_pads[idToIndex(Id::KA_RIGHT)].getState();
    const bool don_active = m_pads[idToIndex(Id::DON_LEFT)].getState() || m_pads[idToIndex(Id::DON_RIGHT)].getState();

    if (ka_active && don_active) {
        const bool ka_was_first = (previous_state[idToIndex(Id::KA_LEFT)] || previous_state[idToIndex(Id::KA_RIGHT)]) &&
                                  !(previous_state[idToIndex(Id::DON_LEFT)] || previous_state[idToIndex(Id::DON_RIGHT)]);
        const bool don_was_first = (previous_state[idToIndex(Id::DON_LEFT)] || previous_state[idToIndex(Id::DON_RIGHT)]) &&
                                   !(previous_state[idToIndex(Id::KA_LEFT)] || previous_state[idToIndex(Id::KA_RIGHT)]);

        if (ka_was_first) {
            m_pads[idToIndex(Id::DON_LEFT)].setState(false, digital_debounce_ms);
            m_pads[idToIndex(Id::DON_RIGHT)].setState(false, digital_debounce_ms);
            arb_blocked[idToIndex(Id::DON_LEFT)] = filtered_raw_values[idToIndex(Id::DON_LEFT)] != 0;
            arb_blocked[idToIndex(Id::DON_RIGHT)] = filtered_raw_values[idToIndex(Id::DON_RIGHT)] != 0;
        } else if (don_was_first) {
            m_pads[idToIndex(Id::KA_LEFT)].setState(false, digital_debounce_ms);
            m_pads[idToIndex(Id::KA_RIGHT)].setState(false, digital_debounce_ms);
            arb_blocked[idToIndex(Id::KA_LEFT)] = filtered_raw_values[idToIndex(Id::KA_LEFT)] != 0;
            arb_blocked[idToIndex(Id::KA_RIGHT)] = filtered_raw_values[idToIndex(Id::KA_RIGHT)] != 0;
        } else {
            m_pads[idToIndex(Id::DON_LEFT)].setState(false, digital_debounce_ms);
            m_pads[idToIndex(Id::DON_RIGHT)].setState(false, digital_debounce_ms);
            arb_blocked[idToIndex(Id::DON_LEFT)] = filtered_raw_values[idToIndex(Id::DON_LEFT)] != 0;
            arb_blocked[idToIndex(Id::DON_RIGHT)] = filtered_raw_values[idToIndex(Id::DON_RIGHT)] != 0;
        }
    }

    input_state.drum.don_left.triggered = m_pads[idToIndex(Id::DON_LEFT)].getState();
    input_state.drum.ka_left.triggered = m_pads[idToIndex(Id::KA_LEFT)].getState();
    input_state.drum.don_right.triggered = m_pads[idToIndex(Id::DON_RIGHT)].getState();
    input_state.drum.ka_right.triggered = m_pads[idToIndex(Id::KA_RIGHT)].getState();

    const std::array<bool, 4> current_state = {
        input_state.drum.don_left.triggered,
        input_state.drum.ka_left.triggered,
        input_state.drum.don_right.triggered,
        input_state.drum.ka_right.triggered
    };

    bool any_rising = false;
    for (size_t i = 0; i < rising_hit.size(); ++i) {
        rising_hit[i] = current_state[i] && !previous_state[i];
        if (rising_hit[i]) {
            m_last_rise_ms[i] = now;
        }
        any_rising = any_rising || rising_hit[i];
    }

    const auto latch_side_winner = [&](size_t side, size_t don_idx, size_t ka_idx) {
        if (rising_hit[don_idx] && !rising_hit[ka_idx]) {
            m_same_side_winner[side] = SideWinner::Don;
            m_same_side_lock_until_ms[side] = now + SAME_SIDE_LOCK_MS;
        } else if (rising_hit[ka_idx] && !rising_hit[don_idx]) {
            m_same_side_winner[side] = SideWinner::Ka;
            m_same_side_lock_until_ms[side] = now + SAME_SIDE_LOCK_MS;
        } else if (rising_hit[don_idx] && rising_hit[ka_idx]) {
            if (filtered_raw_values[ka_idx] > filtered_raw_values[don_idx]) {
                m_same_side_winner[side] = SideWinner::Ka;
            } else {
                m_same_side_winner[side] = SideWinner::Don;
            }
            m_same_side_lock_until_ms[side] = now + SAME_SIDE_LOCK_MS;
        }
    };

    latch_side_winner(0, idToIndex(Id::DON_LEFT), idToIndex(Id::KA_LEFT));
    latch_side_winner(1, idToIndex(Id::DON_RIGHT), idToIndex(Id::KA_RIGHT));

    if (any_rising) {
        m_roll_counter.addHit();
    }

    input_state.drum.current_roll = m_roll_counter.getRoll();
    input_state.drum.previous_roll = m_roll_counter.getPreviousRoll();

    // Latch debug masks for a short window so host-side polling does not miss
    // one-tick edges under high-rate firmware loops.
    static constexpr uint32_t DEBUG_MASK_LATCH_MS = 18;
    for (size_t i = 0; i < 4; ++i) {
        if (rising_hit[i]) {
            m_dbg_hit_until_ms[i] = now + DEBUG_MASK_LATCH_MS;
        }
        if (cross_blocked[i]) {
            m_dbg_cross_until_ms[i] = now + DEBUG_MASK_LATCH_MS;
        }
        if (arb_blocked[i]) {
            m_dbg_arb_until_ms[i] = now + DEBUG_MASK_LATCH_MS;
        }
    }

    const auto maskFromLatches = [&](const std::array<uint32_t, 4> &until) -> uint8_t {
        return ((until[idToIndex(Id::DON_LEFT)] > now) ? (1u << 0) : 0u) |
               ((until[idToIndex(Id::KA_LEFT)] > now) ? (1u << 1) : 0u) |
               ((until[idToIndex(Id::DON_RIGHT)] > now) ? (1u << 2) : 0u) |
               ((until[idToIndex(Id::KA_RIGHT)] > now) ? (1u << 3) : 0u);
    };

    input_state.drum.debug_event.hit_mask = maskFromLatches(m_dbg_hit_until_ms);
    input_state.drum.debug_event.cross_block_mask = maskFromLatches(m_dbg_cross_until_ms);
    input_state.drum.debug_event.arb_block_mask = maskFromLatches(m_dbg_arb_until_ms);
    input_state.drum.debug_event.held_high_mask = 0;
    input_state.drum.debug_event.timestamp_ms = now;
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

    const bool is_analog_mode = (usb_mode == USB_MODE_XBOX360_ANALOG_P1) ||
                                (usb_mode == USB_MODE_XBOX360_ANALOG_P2);

    // Always keep buffered/raw analog view for UI + calibration diagnostics.
    updateAnalogInputState(input_state, raw_values);

    if (!is_analog_mode) {
        // Low-latency gameplay path:
        // Use fresh ADC-subtracted samples for digital hit decisions so
        // buffered peak hold does not delay release/rearm during fast rolls.
        updateDigitalInputState(input_state, raw_values);
    }

    if (m_guided_cal_state.isActive()) {
        updateGuidedCalibration(input_state.drum, raw_values);
    }
}

void Drum::setDebounceDelay(uint16_t delay) { m_config.debounce_delay_ms = delay; }

void Drum::setTriggerThresholds(const Config::Thresholds &thresholds) { m_config.trigger_thresholds = thresholds; }

// ============================================================================
// GUIDED CALIBRATION WIZARD
// ============================================================================

void Drum::startGuidedCalibration() {
    m_guided_cal_state.startWelcome();
    m_guided_cal_state.recommended_thresholds[idToIndex(Id::DON_LEFT)] = m_config.trigger_thresholds.don_left;
    m_guided_cal_state.recommended_thresholds[idToIndex(Id::KA_LEFT)] = m_config.trigger_thresholds.ka_left;
    m_guided_cal_state.recommended_thresholds[idToIndex(Id::DON_RIGHT)] = m_config.trigger_thresholds.don_right;
    m_guided_cal_state.recommended_thresholds[idToIndex(Id::KA_RIGHT)] = m_config.trigger_thresholds.ka_right;
}

void Drum::advanceCalibWizard() {
    auto& s = m_guided_cal_state;
    switch (s.current_mode) {
        case GuidedCalState::Mode::Welcome:
            s.startInstructions();
            break;
        case GuidedCalState::Mode::Instructions:
            s.current_pad = 0;
            s.retry_count_for_pad = 0;
            s.startPadNormal();
            break;
        case GuidedCalState::Mode::PadHardPrompt:
            s.startPadHard();
            break;
        case GuidedCalState::Mode::BleedDetected:
            s.retry_count_for_pad++;
            s.startPadNormal();
            break;
        case GuidedCalState::Mode::PadNormal:
        case GuidedCalState::Mode::PadHard:
        case GuidedCalState::Mode::Saving:
        case GuidedCalState::Mode::Complete:
        case GuidedCalState::Mode::Cancelled:
            break;
        case GuidedCalState::Mode::Review:
            s.startSaving();
            break;
        case GuidedCalState::Mode::CancelConfirm:
            s.startCancelled();
            break;
        default:
            break;
    }
}

void Drum::cancelGuidedCalibration() {
    auto& s = m_guided_cal_state;
    if (!s.isActive()) {
        return;
    }
    if (s.current_mode == GuidedCalState::Mode::CancelConfirm) {
        s.current_mode = s.mode_before_cancel;
        return;
    }
    if (s.current_mode == GuidedCalState::Mode::Cancelled ||
        s.current_mode == GuidedCalState::Mode::Complete ||
        s.current_mode == GuidedCalState::Mode::Saving) {
        s.reset();
        return;
    }
    s.startCancelConfirm();
}

void Drum::updateGuidedCalibration(const Utils::InputState::Drum &drum_state, const std::array<uint16_t, 4> &raw_values) {
    auto& s = m_guided_cal_state;
    if (!s.isActive()) return;

    const uint32_t now = to_ms_since_boot(get_absolute_time());
    const uint8_t padIdx = s.currentPadIndex();

    const auto beginBleedWatch = [&](uint8_t target_idx) {
        s.bleed_watch_active = true;
        s.bleed_watch_until = now + GuidedCalState::BLEED_WATCH_MS;
        s.bleed_target_index = target_idx;
        for (uint8_t i = 0; i < 4; ++i) {
            s.bleed_watch_peaks[i] = (i == target_idx) ? 0 : raw_values[i];
        }
    };

    const auto raiseOffenders = [&](uint8_t offender_mask) {
        s.last_offender_mask = offender_mask;
        s.bleed_watch_active = false;
        s.bleed_watch_until = 0;
        s.watch_success_action = GuidedCalState::WatchSuccessAction::None;
        for (uint8_t i = 0; i < 4; ++i) {
            s.last_threshold_before_raise[i] = s.recommended_thresholds[i];
            if ((offender_mask & (1u << i)) != 0) {
                uint16_t raised = static_cast<uint16_t>(s.recommended_thresholds[i] + GuidedCalState::THRESHOLD_STEP);
                if (raised > GuidedCalState::THRESHOLD_MAX) {
                    raised = GuidedCalState::THRESHOLD_MAX;
                }
                s.recommended_thresholds[i] = raised;
            }
        }
        s.startBleedDetected();
    };

    const auto applyBleedWatch = [&]() -> bool {
        if (!s.bleed_watch_active) {
            return false;
        }

        uint8_t offender_mask = 0;
        for (uint8_t i = 0; i < 4; ++i) {
            if (i == s.bleed_target_index) {
                continue;
            }
            if (raw_values[i] > s.bleed_watch_peaks[i]) {
                s.bleed_watch_peaks[i] = raw_values[i];
            }
            const uint16_t threshold = s.recommended_thresholds[i];
            const uint16_t bleed_floor =
                (threshold > GuidedCalState::BLEED_PRETRIGGER_MARGIN)
                    ? static_cast<uint16_t>(threshold - GuidedCalState::BLEED_PRETRIGGER_MARGIN)
                    : 0;
            if (s.bleed_watch_peaks[i] >= bleed_floor) {
                offender_mask |= static_cast<uint8_t>(1u << i);
            }
        }

        if (offender_mask != 0) {
            raiseOffenders(offender_mask);
            return true;
        }

        if (now <= s.bleed_watch_until) {
            return false;
        }

        s.bleed_watch_active = false;
        s.bleed_watch_until = 0;
        if (s.watch_success_action == GuidedCalState::WatchSuccessAction::AdvanceToHardPrompt) {
            s.watch_success_action = GuidedCalState::WatchSuccessAction::None;
            s.startPadHardPrompt();
        } else if (s.watch_success_action == GuidedCalState::WatchSuccessAction::FinishPad) {
            s.watch_success_action = GuidedCalState::WatchSuccessAction::None;
            _finishCurrentPadTest();
        }
        return false;
    };

    applyBleedWatch();

    const uint8_t hitEdgeMask = static_cast<uint8_t>(drum_state.debug_event.hit_mask & static_cast<uint8_t>(~s.prev_hit_mask));
    const uint8_t crossEdgeMask = static_cast<uint8_t>(drum_state.debug_event.cross_block_mask & static_cast<uint8_t>(~s.prev_cross_mask));
    const uint8_t arbEdgeMask = static_cast<uint8_t>(drum_state.debug_event.arb_block_mask & static_cast<uint8_t>(~s.prev_arb_mask));

    uint8_t nearBleedMask = 0;
    for (uint8_t i = 0; i < 4; ++i) {
        if (i == padIdx) {
            continue;
        }
        const uint16_t threshold = s.recommended_thresholds[i];
        const uint16_t bleed_floor =
            (threshold > GuidedCalState::BLEED_PRETRIGGER_MARGIN)
                ? static_cast<uint16_t>(threshold - GuidedCalState::BLEED_PRETRIGGER_MARGIN)
                : 0;
        if (raw_values[i] >= bleed_floor) {
            nearBleedMask |= static_cast<uint8_t>(1u << i);
        }
    }

    s.prev_hit_mask = drum_state.debug_event.hit_mask;
    s.prev_cross_mask = drum_state.debug_event.cross_block_mask;
    s.prev_arb_mask = drum_state.debug_event.arb_block_mask;
    s.prev_held_mask = drum_state.debug_event.held_high_mask;

    switch (s.current_mode) {
    case GuidedCalState::Mode::Welcome:
    case GuidedCalState::Mode::Instructions:
    case GuidedCalState::Mode::BleedDetected:
    case GuidedCalState::Mode::Review:
    case GuidedCalState::Mode::CancelConfirm:
        break;

    case GuidedCalState::Mode::PadNormal:
    case GuidedCalState::Mode::PadHardPrompt:
    case GuidedCalState::Mode::PadHard: {
        const bool targetHit = (hitEdgeMask & (1u << padIdx)) != 0;
        const uint8_t extrasMask = static_cast<uint8_t>(crossEdgeMask | arbEdgeMask | nearBleedMask);

        if (!targetHit) {
            break;
        }

        if (extrasMask != 0) {
            raiseOffenders(extrasMask);
            break;
        }

        const bool block_for_startup =
            (s.current_mode == GuidedCalState::Mode::PadNormal) && ((now - s.phase_start) < 250);
        if (block_for_startup || (now - s.last_target_hit_time) < GuidedCalState::HIT_COOLDOWN_MS) {
            break;
        }

        s.last_target_hit_time = now;
        beginBleedWatch(padIdx);

        if (s.current_mode == GuidedCalState::Mode::PadNormal) {
            if (s.normal_hits_done < GuidedCalState::REQUIRED_NORMAL_HITS) {
                s.normal_hits_done++;
            }
            if (s.normal_hits_done >= GuidedCalState::REQUIRED_NORMAL_HITS) {
                s.watch_success_action = GuidedCalState::WatchSuccessAction::AdvanceToHardPrompt;
            }
        } else {
            s.hard_hit_done = true;
            s.watch_success_action = GuidedCalState::WatchSuccessAction::FinishPad;
        }
        break;
    }

    case GuidedCalState::Mode::Saving:
        if ((now - s.phase_start) >= GuidedCalState::SAVING_DISPLAY_MS) {
            s.startComplete();
        }
        break;

    case GuidedCalState::Mode::Complete:
        if ((now - s.phase_start) >= GuidedCalState::COMPLETE_DISPLAY_MS) {
            s.current_mode = GuidedCalState::Mode::Inactive;
        }
        break;

    case GuidedCalState::Mode::Cancelled:
        if ((now - s.phase_start) >= GuidedCalState::CANCELLED_DISPLAY_MS) {
            s.current_mode = GuidedCalState::Mode::Inactive;
        }
        break;

    default:
        break;
    }
}

void Drum::_finishCurrentPadTest() {
    _advanceToNextPad();
}

void Drum::_advanceToNextPad() {
    auto& s = m_guided_cal_state;
    s.current_pad++;
    if (s.current_pad >= 4) {
        s.startReview();
    } else {
        s.retry_count_for_pad = 0;
        s.startPadNormal();
    }
}

void Drum::applyGuidedCalRecommendations() {
    if (!m_guided_cal_state.hasRecommendations()) return;
    m_config.trigger_thresholds.don_left  = m_guided_cal_state.recommended_thresholds[idToIndex(Id::DON_LEFT)];
    m_config.trigger_thresholds.ka_left   = m_guided_cal_state.recommended_thresholds[idToIndex(Id::KA_LEFT)];
    m_config.trigger_thresholds.don_right = m_guided_cal_state.recommended_thresholds[idToIndex(Id::DON_RIGHT)];
    m_config.trigger_thresholds.ka_right  = m_guided_cal_state.recommended_thresholds[idToIndex(Id::KA_RIGHT)];

    m_last_guided_cal_report_version++;
    snprintf(
        m_last_guided_cal_report,
        sizeof(m_last_guided_cal_report),
        "VER=%lu;"
        "NR=%u,%u,%u,%u;"
        "MH=%u,%u,%u,%u;"
        "XT=%u,%u,%u,%u;"
        "TH=%u,%u,%u,%u;"
        "WARN=%u",
        static_cast<unsigned long>(m_last_guided_cal_report_version),
        0u, 0u, 0u, 0u,
        0u, 0u, 0u, 0u,
        0u, 0u, 0u, 0u,
        static_cast<unsigned>(m_guided_cal_state.recommended_thresholds[0]),
        static_cast<unsigned>(m_guided_cal_state.recommended_thresholds[1]),
        static_cast<unsigned>(m_guided_cal_state.recommended_thresholds[2]),
        static_cast<unsigned>(m_guided_cal_state.recommended_thresholds[3]),
        0u);
}
} // namespace OuchiTaiko::Peripherals

// End of file Drum.cpp





