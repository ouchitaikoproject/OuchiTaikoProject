#ifndef UTILS_SETTINGSSTORE_H_
#define UTILS_SETTINGSSTORE_H_

#include "peripherals/Drum.h"
#include "usb/device_driver.h"

#include "hardware/flash.h"

#include <array>

namespace OuchiTaiko::Utils {

class SettingsStore {
  private:
    const static uint32_t m_flash_size = FLASH_SECTOR_SIZE;
    const static uint32_t m_flash_offset = PICO_FLASH_SIZE_BYTES - m_flash_size;
    const static uint32_t m_store_size = FLASH_PAGE_SIZE;
    const static uint32_t m_store_pages = m_flash_size / m_store_size;
    const static uint8_t m_magic_byte = 0x39;
    const static uint8_t m_store_version = 2;
    const static uint8_t m_store_version_legacy_v1 = 1;

    struct __attribute((packed, aligned(1))) Storecache {
        uint8_t in_use;
        uint8_t version;  // Settings version to detect incompatible changes
        usb_mode_t usb_mode;
        Peripherals::Drum::Config::Thresholds trigger_thresholds;
        uint8_t led_brightness;
        bool led_enable_player_color;
        uint16_t hit_hold_ms;
        uint32_t guided_cal_report_version;
        uint32_t guided_cal_session_id;
        uint32_t guided_cal_uptime_ms;
        usb_mode_t guided_cal_usb_mode;
        std::array<char, 192> guided_cal_report;

        std::array<uint8_t, m_store_size - sizeof(uint8_t) - sizeof(uint8_t) - sizeof(usb_mode_t) -
                                sizeof(Peripherals::Drum::Config::Thresholds) - sizeof(uint8_t) - sizeof(bool) -
                                sizeof(uint16_t) - sizeof(uint32_t) - sizeof(uint32_t) - sizeof(uint32_t) -
                                sizeof(usb_mode_t) - sizeof(std::array<char, 192>)>
            _padding;
    };
    static_assert(sizeof(Storecache) == m_store_size);

    enum class RebootType : uint8_t {
        None,
        Normal,
        Bootsel,
    };

    Storecache m_store_cache;
    bool m_dirty{true};
    RebootType m_scheduled_reboot{RebootType::None};

    Storecache read();

  public:
    SettingsStore();

    void setUsbMode(usb_mode_t mode);
    [[nodiscard]] usb_mode_t getUsbMode() const;

    void setTriggerThresholds(const Peripherals::Drum::Config::Thresholds &thresholds);
    [[nodiscard]] Peripherals::Drum::Config::Thresholds getTriggerThresholds() const;

    void setLedBrightness(uint8_t brightness);
    [[nodiscard]] uint8_t getLedBrightness() const;

    void setLedEnablePlayerColor(bool do_enable);
    [[nodiscard]] bool getLedEnablePlayerColor() const;

    void setHitHoldMs(uint16_t delay);
    [[nodiscard]] uint16_t getHitHoldMs() const;

    void setLastGuidedCalReport(uint32_t version, const char* report, usb_mode_t mode, uint32_t uptime_ms);
    [[nodiscard]] uint32_t getLastGuidedCalReportVersion() const;
    [[nodiscard]] uint32_t getLastGuidedCalSessionId() const;
    [[nodiscard]] uint32_t getLastGuidedCalUptimeMs() const;
    [[nodiscard]] usb_mode_t getLastGuidedCalUsbMode() const;
    [[nodiscard]] const char* getLastGuidedCalReport() const;

    void scheduleReboot(bool bootsel = false);

    void store();
    void reset();
};
} // namespace OuchiTaiko::Utils

#endif // UTILS_SETTINGSSTORE_H_

