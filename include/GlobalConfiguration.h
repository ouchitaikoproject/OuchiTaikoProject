#ifndef GLOBALCONFIGURATION_H_
#define GLOBALCONFIGURATION_H_

#include "peripherals/Controller.h"
#include "peripherals/Display.h"
#include "peripherals/Drum.h"
#include "peripherals/StatusLed.h"

#include "hardware/i2c.h"
#include "hardware/spi.h"

namespace OuchiTaiko::Config {

struct I2c {
    uint8_t sda_pin;
    uint8_t scl_pin;
    i2c_inst_t *block;
    uint speed_hz;
};

namespace Default {

const usb_mode_t usb_mode = USB_MODE_SWITCH_TATACON;

const I2c i2c_config = {
    .sda_pin = 6,
    .scl_pin = 7,
    .block = i2c1,
    .speed_hz = 1000000,
};

const Peripherals::Drum::Config drum_config = {
    .trigger_thresholds =
        {
            .don_left = 55,
            .ka_left = 70,
            .don_right = 55,
            .ka_right = 70,
        },
    .big_hit_enable = false,
    .big_hit_threshold = 2000,
    .debounce_delay_ms = 25,
    .roll_counter_timeout_ms = 500,
    .enable_simultap = false,
    .adc_channels =
        {
            .don_left = 1,
            .ka_left = 0,
            .don_right = 2,
            .ka_right = 3,
        },
    .adc_config =
        Peripherals::Drum::Config::InternalAdc{
            .sample_count = 8,
        },
};

const Peripherals::Controller::Config controller_config = {
    .pins =
        {
            .dpad =
                {
                    .up = 8,
                    .down = 9,
                    .left = 10,
                    .right = 11,
                },
            .buttons =
                {
                    .north = 0,
                    .east = 3,
                    .south = 1,
                    .west = 2,
                    .l = 12,
                    .r = 4,
                    .start = 5,
                    .select = 13,
                    .home = 6,
                    .share = 14,
                },
        },
    .debounce_delay_ms = 25,
    .gpio_config =
        Peripherals::Controller::Config::ExternalGpio{
            .i2c =
                {
                    .block = i2c_config.block,
                    .address = 0x20, //The WaveShare Boards use 0x27 by default, the GODIYMODULES Board uses 0x20.  If you jump A0, A1, and A2 to VCC on the GODIYMODULES, it will be 0x27   Adjust Accordingly.
                },
        },
};

const Peripherals::StatusLed::Config led_config = {
    .idle_color = {.r = 128, .g = 128, .b = 128},
    .don_left_color = {.r = 255, .g = 0, .b = 0},
    .ka_left_color = {.r = 0, .g = 0, .b = 255},
    .don_right_color = {.r = 255, .g = 255, .b = 0},
    .ka_right_color = {.r = 0, .g = 255, .b = 255},
    .led_enable_pin = 11,
    .led_pin = 12,
    .is_rgbw = false,
    .brightness = 255,
    .enable_player_color = true,
};

const Peripherals::Display::Config display_config = {
    .i2c_block = i2c_config.block,
    .i2c_address = 0x3C,
};

} // namespace Default
} // namespace OuchiTaiko::Config

#endif // GLOBALCONFIGURATION_H_