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

const usb_mode_t usb_mode = USB_MODE_DEBUG;

const I2c i2c_config = {
    // Custom RP2040-Zero PCB routes the OLED to the module pins labeled SDA/SCL.
    // On the Waveshare RP2040-Zero castellated pinout those map to GPIO8/GPIO9 on I2C0.
    .sda_pin = 8,
    .scl_pin = 9,
    .block = i2c0,
    .speed_hz = 1000000,
};

const Peripherals::Drum::Config drum_config = {
    .trigger_thresholds =
        {
            // Known-good modular-board baseline confirmed by slim calibrate and gameplay.
            .don_left = 285,
            .ka_left = 285,
            .don_right = 285,
            .ka_right = 285,
        },
    .debounce_delay_ms = 25,
    .roll_counter_timeout_ms = 500,

    // ============================================================================
    // ANALOG MODE GAIN MULTIPLIER
    // ============================================================================
    // This multiplier compensates for missing OpAmp amplification circuit.
    //
    // ** CHANGE THIS VALUE ** to adjust analog mode sensitivity:
    //   - 1.0 = No gain (direct piezo output)
    //   - 2.0-3.0 = Typical for hybrid circuits without OpAmp
    //   - 4.0-5.0 = Higher gain for very weak piezos or testing upper limits
    //   - 8.0+ = Maximum gain (may cause clipping/overflow)
    //
    // Usable range: 1.0 to 8.0
    // Recommended starting value: 4.0
    //
    // After changing this value, reflash firmware and test in-game to find
    // the optimal setting for your hardware.
    .analog_gain = 4.0,
    // ============================================================================

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
                    .up = 11,
                    .down = 10,
                    .left = 12,
                    .right = 7,
                },
            .buttons =
                {
                    .north = 3,
                    .east = 2,
                    .south = 4,
                    .west = 5,
                    .l = 13,
                    .r = 1,
                    .start = 6,
                    .select = 14,
                    .home = 0,
                    .share = 15,
                },
        },
    .debounce_delay_ms = 25,
    .gpio_config = Peripherals::Controller::Config::InternalGpio{},
};

const Peripherals::StatusLed::Config led_config = {
    .idle_color = {.r = 128, .g = 128, .b = 128},
    .don_left_color = {.r = 180, .g = 0, .b = 0},
    .ka_left_color = {.r = 0, .g = 40, .b = 180},
    .don_right_color = {.r = 180, .g = 0, .b = 0},
    .ka_right_color = {.r = 0, .g = 40, .b = 180},
    // Use the RP2040-Zero onboard WS2812. No external enable gate exists on the custom PCB.
    .led_enable_pin = UINT8_MAX,
    .led_pin = 16,
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


