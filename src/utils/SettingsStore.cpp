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

struct __attribute((packed, aligned(1))) LegacyStorecacheV1 {
    uint8_t in_use;
    uint8_t version;
    usb_mode_t usb_mode;
    OuchiTaiko::Peripherals::Drum::Config::Thresholds trigger_thresholds;
    uint8_t led_brightness;
    bool led_enable_player_color;
    uint16_t debounce_delay;
    std::array<uint8_t, FLASH_PAGE_SIZE - sizeof(uint8_t) - sizeof(uint8_t) - sizeof(usb_mode_t) -
                            sizeof(OuchiTaiko::Peripherals::Drum::Config::Thresholds) - sizeof(uint8_t) -
                            sizeof(bool) - sizeof(uint16_t)>
        _padding;
};
static_assert(sizeof(LegacyStorecacheV1) == FLASH_PAGE_SIZE);

bool is_old_waveshare_threshold_baseline(const OuchiTaiko::Peripherals::Drum::Config::Thresholds &thresholds) {
    return thresholds.don_left == 60 && thresholds.ka_left == 150 &&
           thresholds.don_right == 80 && thresholds.ka_right == 180;
}

} // namespace

SettingsStore::SettingsStore()
    : m_store_cache({.in_use = m_magic_byte,
                     .version = m_store_version,  // Increment this when settings format changes
                     .usb_mode = Config::Default::usb_mode,
                     .trigger_thresholds = Config::Default::drum_config.trigger_thresholds,
                     .led_brightness = Config::Default::led_config.brightness,
                     .led_enable_player_color = Config::Default::led_config.enable_player_color,
                     .debounce_delay = Config::Default::drum_config.debounce_delay_ms,
                     .guided_cal_report_version = 0,
                     .guided_cal_session_id = 0,
                     .guided_cal_uptime_ms = 0,
                     .guided_cal_usb_mode = Config::Default::usb_mode,
                     .guided_cal_report = {},
                     ._padding = {}}) {
    uint32_t current_page = m_flash_offset + m_flash_size - m_store_size;
    bool found_valid = false;
    uint8_t found_version = 0;
    for (size_t i = 0; i < m_store_pages; ++i) {
        if (read_byte(current_page) == m_magic_byte) {
            uint8_t stored_version = read_byte(current_page + 1);
            if (stored_version == m_store_version || stored_version == m_store_version_legacy_v1) {
                found_valid = true;
                found_version = stored_version;
                break;
            }
        }
        current_page -= m_store_size;
    }

    if (found_valid) {
        if (found_version == m_store_version) {
            m_store_cache = *(reinterpret_cast<Storecache *>(XIP_BASE + current_page));
            m_dirty = false;
        } else {
            // Migrate v1 settings into current v2 schema.
            const auto legacy = *(reinterpret_cast<LegacyStorecacheV1 *>(XIP_BASE + current_page));
            m_store_cache.in_use = m_magic_byte;
            m_store_cache.version = m_store_version;
            m_store_cache.usb_mode = legacy.usb_mode;
            m_store_cache.trigger_thresholds = legacy.trigger_thresholds;
            m_store_cache.led_brightness = legacy.led_brightness;
            m_store_cache.led_enable_player_color = legacy.led_enable_player_color;
            m_store_cache.debounce_delay = legacy.debounce_delay;
            m_store_cache.guided_cal_report_version = 0;
            m_store_cache.guided_cal_session_id = 0;
            m_store_cache.guided_cal_uptime_ms = 0;
            m_store_cache.guided_cal_usb_mode = legacy.usb_mode;
            m_store_cache.guided_cal_report.fill('\0');
            m_dirty = true;
        }
    }

    // One-time safety migration: early Waveshare builds shipped with a low
    // threshold baseline. Preserve user-tuned values, but move untouched old
    // defaults to the current played-in baseline.
    if (is_old_waveshare_threshold_baseline(m_store_cache.trigger_thresholds)) {
        m_store_cache.trigger_thresholds = Config::Default::drum_config.trigger_thresholds;
        m_dirty = true;
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

void SettingsStore::setLastGuidedCalReport(uint32_t version, const char* report, usb_mode_t mode, uint32_t uptime_ms) {
    if (report == nullptr) {
        report = "";
    }

    constexpr size_t max_len = sizeof(m_store_cache.guided_cal_report) - 1;
    char normalized[sizeof(m_store_cache.guided_cal_report)]{};
    std::strncpy(normalized, report, max_len);
    normalized[max_len] = '\0';

    if (m_store_cache.guided_cal_report_version != version ||
        std::strncmp(m_store_cache.guided_cal_report.data(), normalized, sizeof(m_store_cache.guided_cal_report)) != 0) {
        m_store_cache.guided_cal_report_version = version;
        m_store_cache.guided_cal_session_id++;
        m_store_cache.guided_cal_uptime_ms = uptime_ms;
        m_store_cache.guided_cal_usb_mode = mode;
        std::memcpy(m_store_cache.guided_cal_report.data(), normalized, sizeof(m_store_cache.guided_cal_report));
        m_store_cache.guided_cal_report.back() = '\0';
        m_dirty = true;
    }
}

uint32_t SettingsStore::getLastGuidedCalReportVersion() const { return m_store_cache.guided_cal_report_version; }
uint32_t SettingsStore::getLastGuidedCalSessionId() const { return m_store_cache.guided_cal_session_id; }
uint32_t SettingsStore::getLastGuidedCalUptimeMs() const { return m_store_cache.guided_cal_uptime_ms; }
usb_mode_t SettingsStore::getLastGuidedCalUsbMode() const { return m_store_cache.guided_cal_usb_mode; }

const char* SettingsStore::getLastGuidedCalReport() const { return m_store_cache.guided_cal_report.data(); }

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
    // Reset cache to factory defaults BEFORE erasing flash
    m_store_cache = Storecache{
        .in_use = m_magic_byte,
        .version = m_store_version,
        .usb_mode = Config::Default::usb_mode,
        .trigger_thresholds = Config::Default::drum_config.trigger_thresholds,
        .led_brightness = Config::Default::led_config.brightness,
        .led_enable_player_color = Config::Default::led_config.enable_player_color,
        .debounce_delay = Config::Default::drum_config.debounce_delay_ms,
        .guided_cal_report_version = 0,
        .guided_cal_session_id = 0,
        .guided_cal_uptime_ms = 0,
        .guided_cal_usb_mode = Config::Default::usb_mode,
        .guided_cal_report = {},
        ._padding = {}
    };
    
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


