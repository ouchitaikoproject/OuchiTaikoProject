//Beginning of Display.h

#ifndef PERIPHERALS_DISPLAY_H_
#define PERIPHERALS_DISPLAY_H_

#include "usb/device_driver.h"
#include "utils/InputState.h"
#include "utils/Menu.h"

#include "hardware/i2c.h"
#include <ssd1306/ssd1306.h>

#include <cstdint>
#include <memory>
#include <string>

namespace OuchiTaiko::Peripherals {

// Forward declaration for Drum
class Drum;

class Display {
  public:
    struct Config {
        i2c_inst_t *i2c_block;
        uint8_t i2c_address;
    };

    struct BringupTestState {
        bool active{false};
        uint8_t current_index{0};
    };

  private:
    enum class State : uint8_t {
        BootSplash,
        Idle,
        Menu,
        // Guided calibration wizard screens
        GuidedCalWelcome,
        GuidedCalPadTest,  // Normal hits + hard hits (same layout, label changes)
        GuidedCalOverview,
        GuidedCalSaving,
        GuidedCalComplete,
        GuidedCalError,
        BringupTest,
        RebootCountdown
    };

    Config m_config;
    State m_state{State::BootSplash};

    Utils::InputState m_input_state;
    usb_mode_t m_usb_mode{USB_MODE_DEBUG};
    uint8_t m_player_id{0};

    Utils::Menu::State m_menu_state{};

    ssd1306_t m_display{};
    uint32_t m_next_frame_time{0};

    // Boot splash timing
    uint32_t m_boot_splash_start{0};
    static constexpr uint32_t BOOT_SPLASH_DURATION_MS = 6000;  // 3s INSERT COIN + 1.5s scroll + 1.5s hold

    // Reboot countdown timing
    uint32_t m_reboot_countdown_start{0};
    static constexpr uint32_t REBOOT_COUNTDOWN_MS = 5000;

    // Previous state variables for rising edge detection
    bool m_last_ka_left = false;
    bool m_last_don_left = false;
    bool m_last_don_right = false;
    bool m_last_ka_right = false;

    // Guided calibration: Reference to drum for accessing calibration state
    Drum *m_drum{nullptr};

    // Fresh thresholds from SettingsStore
    Peripherals::Drum::Config::Thresholds m_current_thresholds{};
    BringupTestState m_bringup_test_state{};

    // Burst circle animation
    struct Ring {
        uint8_t radius;
        bool active;
    };
    static const uint8_t MAX_RINGS_PER_DRUM = 2;
    Ring m_rings[4][MAX_RINGS_PER_DRUM];

    // Particle system for splash screen impact effects
    struct Particle {
        float x, y;
        float vx, vy;
        uint8_t life;
        bool active;
    };
    static const uint8_t MAX_PARTICLES = 40;
    Particle m_particles[MAX_PARTICLES];

    // Static drum outline positions (x, y) - positioned below button text
    // Button text at y=16 (8px tall) ends at y=24, leave 3px gap, rings start at y=27
    // Max animation radius is 17px, so center at y=38 keeps animation below y=55 (footer at y=54)
    static constexpr uint8_t DRUM_CENTERS[4][2] = {
        {16, 38},  // Ka-Left
        {47, 38},  // Don-Left
        {78, 38},  // Don-Right
        {109, 38}  // Ka-Right
    };
    
    // Static outline radius for drum zones (always visible)
    static constexpr uint8_t DRUM_OUTLINE_RADIUS = 14;

    // Display rendering functions
    void drawBootSplash();
    void drawIdleScreen();
    void drawMenuScreen();
    void drawNavigationBar(const Utils::Menu::Descriptor &descriptor);

    // Guided calibration wizard draw functions
    void drawGuidedCalWelcomeScreen();
    void drawGuidedCalPadTestScreen();
    void drawGuidedCalOverviewScreen();
    void drawGuidedCalSavingScreen();
    void drawGuidedCalCompleteScreen();
    void drawGuidedCalErrorScreen();
    void drawBringupTestScreen();

    // Reboot countdown display function
    void drawRebootCountdown();

    // Burst animation functions
    void activateRing(uint8_t drum_index);
    void updateRings();
    void drawRings();
    void drawCircleRing(uint8_t x_center, uint8_t y_center, uint8_t radius);
    void drawCircleRingBlack(uint8_t x_center, uint8_t y_center, uint8_t radius);

    // Particle system functions for splash screen
    void spawnParticles(int x, int y, int count);
    void updateParticles();
    void drawParticles();

  public:
    Display(const Config &config);

    void setInputState(const Utils::InputState &state);
    void setUsbMode(usb_mode_t mode);
    void setPlayerId(uint8_t player_id);

    void setMenuState(const Utils::Menu::State &menu_state);

    // Drum reference + thresholds
    void setDrumReference(Drum *drum);
    void setCurrentThresholds(const Peripherals::Drum::Config::Thresholds &thresholds);
    void setBringupTestState(const BringupTestState &state);

    void showIdle();
    void showMenu();
    // Guided calibration wizard show functions
    void showGuidedCalWelcome();
    void showGuidedCalPadTest();
    void showGuidedCalOverview();
    void showGuidedCalSaving();
    void showGuidedCalComplete();
    void showGuidedCalError();
    void showBringupTest();
    void showRebootCountdown();

    void update();
};

} // namespace OuchiTaiko::Peripherals

#endif // PERIPHERALS_DISPLAY_H_

