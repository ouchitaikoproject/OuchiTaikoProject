#include "peripherals/StatusLed.h"

#include "hardware/gpio.h"
#include "pio_ws2812/ws2812.h"

#include <algorithm>

namespace OuchiTaiko::Peripherals {

StatusLed::StatusLed(const Config &config) : m_config(config) {
    if (m_config.led_enable_pin != UINT8_MAX) {
        gpio_init(m_config.led_enable_pin);
        gpio_set_dir(m_config.led_enable_pin, (bool)GPIO_OUT);
        gpio_put(m_config.led_enable_pin, true);
    }

    ws2812_init(pio0, config.led_pin, m_config.is_rgbw);
}

void StatusLed::setBrightness(const uint8_t brightness) { m_config.brightness = brightness; }
void StatusLed::setEnablePlayerColor(const bool do_enable) { m_config.enable_player_color = do_enable; }

void StatusLed::setInputState(const Utils::InputState &input_state) { m_input_state = input_state; }
void StatusLed::setPlayerColor(const Config::Color &color) { m_player_color = color; }

void StatusLed::update() {
    constexpr float IDLE_DIM_FACTOR = 0.25f;
    constexpr float HIT_DIM_FACTOR = 0.50f;

    Config::Color mixed = {};
    bool triggered = false;

    const auto add_color = [](Config::Color &base, const Config::Color &add) {
        base.r = std::max(base.r, add.r);
        base.g = std::max(base.g, add.g);
        base.b = std::max(base.b, add.b);
    };

    if (m_input_state.drum.don_left.triggered) {
        add_color(mixed, m_config.don_left_color);
        triggered = true;
    }
    if (m_input_state.drum.ka_left.triggered) {
        add_color(mixed, m_config.ka_left_color);
        triggered = true;
    }
    if (m_input_state.drum.don_right.triggered) {
        add_color(mixed, m_config.don_right_color);
        triggered = true;
    }
    if (m_input_state.drum.ka_right.triggered) {
        add_color(mixed, m_config.ka_right_color);
        triggered = true;
    }

    const auto &dpad = m_input_state.controller.dpad;
    const auto &buttons = m_input_state.controller.buttons;
    const bool nav_triggered = dpad.up || dpad.down || dpad.left || dpad.right || buttons.north || buttons.east ||
                               buttons.south || buttons.west || buttons.l || buttons.r || buttons.start ||
                               buttons.select || buttons.home || buttons.share;
    if (nav_triggered) {
        add_color(mixed, {.r = 255, .g = 245, .b = 20});
        triggered = true;
    }

    const float brightness_factor = ((float)m_config.brightness / (float)UINT8_MAX) *
                                    (triggered ? HIT_DIM_FACTOR : IDLE_DIM_FACTOR);

    const auto pack_color = [&](const Config::Color &color) {
        // RP2040-Zero onboard WS2812 is effectively GRB for this driver path.
        return ws2812_rgb_to_gamma_corrected_u32pixel(static_cast<uint8_t>((float)color.g * brightness_factor),
                                                      static_cast<uint8_t>((float)color.r * brightness_factor),
                                                      static_cast<uint8_t>((float)color.b * brightness_factor));
    };

    if (triggered) {
        ws2812_put_pixel(pio0, pack_color(mixed));
    } else {
        const auto idle_color =
            m_config.enable_player_color ? m_player_color.value_or(m_config.idle_color) : m_config.idle_color;

        ws2812_put_pixel(pio0, pack_color(idle_color));
    }
}

} // namespace OuchiTaiko::Peripherals
