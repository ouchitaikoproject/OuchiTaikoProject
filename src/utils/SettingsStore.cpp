#include "utils/SettingsStore.h"

#include "GlobalConfiguration.h"

#include "hardware/watchdog.h"
#include "hardware/resets.h"
#include "hardware/structs/usb.h"
#include "pico/bootrom.h"
#include "pico/multicore.h"
#include "tusb.h"

#include "usb/device/hid/ps4_auth.h"

#include <cstring>
#include <cstddef>  // For offsetof

namespace OuchiTaiko::Utils {

namespace {

uint8_t read_byte(uint32_t offset) {
    return *(reinterpret_cast<uint8_t *>(XIP_BASE + offset));
}

} // namespace

SettingsStore::SettingsStore()
    : m_store_cache({.in_use = m_magic_byte,
                     .version = 1,  // Increment this when settings format changes
                     .usb_mode = Config::Default::usb_mode,
                     .trigger_thresholds = Config::Default::drum_config.trigger_thresholds,
                     .led_brightness = Config::Default::led_config.brightness,
                     .led_enable_player_color = Config::Default::led_config.enable_player_color,
                     .debounce_delay = Config::Default::drum_config.debounce_delay_ms,
                     .big_hit_enable = Config::Default::drum_config.big_hit_enable,
                     .big_hit_threshold = Config::Default::drum_config.big_hit_threshold,
                     .enable_simultap = Config::Default::drum_config.enable_simultap,
                     .performance_profile = Config::Default::drum_config.performance_profile,
                     ._padding = {}}) {
    uint32_t current_page = m_flash_offset + m_flash_size - m_store_size;
    bool found_valid = false;
    for (size_t i = 0; i < m_store_pages; ++i) {
        if (read_byte(current_page) == m_magic_byte) {
            // Check version compatibility
            uint8_t stored_version = read_byte(current_page + 1);
            if (stored_version == 1) {  // Only load if version matches
                found_valid = true;
                break;
            }
            // If version mismatch, treat as invalid and use defaults
        }
        current_page -= m_store_size;
    }

    if (found_valid) {
        m_store_cache = *(reinterpret_cast<Storecache *>(XIP_BASE + current_page));
        m_dirty = false;
    }
}

void SettingsStore::setUsbMode(const usb_mode_t mode) {
    if (mode != m_store_cache.usb_mode) {
        m_store_cache.usb_mode = mode;
        m_dirty = true;
    }
}

usb_mode_t SettingsStore::getUsbMode() const { return m_store_cache.usb_mode; }

void SettingsStore::setTriggerThresholds(const Peripherals::Drum::Config::Thresholds &thresholds) {
    if (m_store_cache.trigger_thresholds.don_left != thresholds.don_left ||
        m_store_cache.trigger_thresholds.don_right != thresholds.don_right ||
        m_store_cache.trigger_thresholds.ka_left != thresholds.ka_left ||
        m_store_cache.trigger_thresholds.ka_right != thresholds.ka_right) {

        m_store_cache.trigger_thresholds = thresholds;
        m_dirty = true;
    }
}
Peripherals::Drum::Config::Thresholds SettingsStore::getTriggerThresholds() const {
    return m_store_cache.trigger_thresholds;
}

void SettingsStore::setBigHitEnable(const bool enable) {
    if (m_store_cache.big_hit_enable != enable) {
        m_store_cache.big_hit_enable = enable;
        m_dirty = true;
    }
}
bool SettingsStore::getBigHitEnable() const {
    return m_store_cache.big_hit_enable;
}

void SettingsStore::setBigHitThreshold(const uint16_t threshold) {
    if (m_store_cache.big_hit_threshold != threshold) {
        m_store_cache.big_hit_threshold = threshold;
        m_dirty = true;
    }
}
uint16_t SettingsStore::getBigHitThreshold() const {
    return m_store_cache.big_hit_threshold;
}

void SettingsStore::setLedBrightness(const uint8_t brightness) {
    if (m_store_cache.led_brightness != brightness) {
        m_store_cache.led_brightness = brightness;
        m_dirty = true;
    }
}
uint8_t SettingsStore::getLedBrightness() const { return m_store_cache.led_brightness; }

void SettingsStore::setLedEnablePlayerColor(const bool do_enable) {
    if (m_store_cache.led_enable_player_color != do_enable) {
        m_store_cache.led_enable_player_color = do_enable;
        m_dirty = true;
    }
}
bool SettingsStore::getLedEnablePlayerColor() const { return m_store_cache.led_enable_player_color; }

void SettingsStore::setDebounceDelay(const uint16_t delay) {
    if (m_store_cache.debounce_delay != delay) {
        m_store_cache.debounce_delay = delay;
        m_dirty = true;
    }
}
uint16_t SettingsStore::getDebounceDelay() const { return m_store_cache.debounce_delay; }

void SettingsStore::setSimulTap(const bool enable) {
    if (m_store_cache.enable_simultap != enable) {
        m_store_cache.enable_simultap = enable;
        m_dirty = true;
    }
}
bool SettingsStore::getSimulTap() const { return m_store_cache.enable_simultap; }

void SettingsStore::setPerformanceProfile(Peripherals::Drum::PerformanceProfile profile) {
    if (m_store_cache.performance_profile != profile) {
        m_store_cache.performance_profile = profile;
        m_dirty = true;
    }
}

Peripherals::Drum::PerformanceProfile SettingsStore::getPerformanceProfile() const {
    return m_store_cache.performance_profile;
}

void SettingsStore::store() {
    bool force_write = (m_scheduled_reboot != RebootType::None);
    
    if (m_dirty || force_write) {
        multicore_lockout_start_blocking();
        const uint32_t interrupts = save_and_disable_interrupts();

        uint32_t current_page = m_flash_offset;
        bool do_erase = true;
        for (size_t i = 0; i < m_store_pages; ++i) {
            if (read_byte(current_page) == 0xFF) {
                do_erase = false;
                break;
            }
            current_page += m_store_size;
        }

        if (do_erase) {
            flash_range_erase(m_flash_offset, m_flash_size);
            current_page = m_flash_offset;
        }

        flash_range_program(current_page, reinterpret_cast<uint8_t *>(&m_store_cache), sizeof(m_store_cache));

        m_dirty = false;

        restore_interrupts_from_disabled(interrupts);
        multicore_lockout_end_blocking();
        
        // CRITICAL: 100ms delay after flash write to ensure it completes
        // Without this, settings may not persist through reboot
        sleep_ms(100);
    }

    switch (m_scheduled_reboot) {
    case RebootType::Normal: {
        // NEW: Wait for countdown to complete (5000ms total)
        // This gives the user time to read the countdown message
        sleep_ms(5000);
        
        // Comprehensive USB shutdown sequence
        ps4_auth_deinit();
        sleep_ms(10);
        
        // Disconnect from host
        tud_disconnect();
        sleep_ms(500);  // Increased delay for Windows to recognize disconnect
        
        // Deinitialize TinyUSB
        tud_deinit(0);
        sleep_ms(100);
        
        // Disable USB interrupt
        irq_set_enabled(USBCTRL_IRQ, false);
        sleep_ms(50);
        
        // Reset USB hardware
        reset_block(RESETS_RESET_USBCTRL_BITS);
        unreset_block_wait(RESETS_RESET_USBCTRL_BITS);
        sleep_ms(100);
        
        // Final cleanup
        ps4_auth_deinit();
        sleep_ms(50);
        
        // Use watchdog to trigger reset - give it enough time
        watchdog_enable(100, false);  // 100ms should be plenty
        while(1) {
            tight_loop_contents();
        }
        break;
    }
    case RebootType::Bootsel:
        // NEW: Wait for countdown to complete (5000ms) before entering bootloader
        // This gives USB hub time to stabilize and shows countdown to user
        sleep_ms(5000);
        
        // Bootloader mode entry sequence
        ps4_auth_deinit();
        sleep_ms(50);
        tud_disconnect();
        sleep_ms(250);
        ps4_auth_deinit();
        tud_deinit(0);
        reset_usb_boot(0, PICO_STDIO_USB_RESET_BOOTSEL_INTERFACE_DISABLE_MASK);
        break;
    case RebootType::None:
        // No reboot scheduled - do nothing
        break;
    }
}

void SettingsStore::reset() {
    multicore_lockout_start_blocking();
    const uint32_t interrupts = save_and_disable_interrupts();

    flash_range_erase(m_flash_offset, m_flash_size);

    restore_interrupts(interrupts);
    multicore_lockout_end_blocking();

    m_dirty = false;

    scheduleReboot();
    // DON'T call store() immediately - let the menu show the countdown first
    // store() will be called by Main.cpp when menu exits
}

void SettingsStore::scheduleReboot(const bool bootsel) {
    if (m_scheduled_reboot != RebootType::Bootsel) {
        m_scheduled_reboot = (bootsel ? RebootType::Bootsel : RebootType::Normal);
    }
}

} // namespace OuchiTaiko::Utils