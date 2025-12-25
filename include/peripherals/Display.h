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

  private:
    enum class State : uint8_t {
        BootSplash,
        Idle,
        Menu,
        TantrumCountdown,
        TantrumRecording,
        TantrumResults,
        TantrumNeedsRedo,
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
    static constexpr uint32_t BOOT_SPLASH_DURATION_MS = 3000;

    // Reboot countdown timing
    uint32_t m_reboot_countdown_start{0};
    static constexpr uint32_t REBOOT_COUNTDOWN_MS = 5000;

    // Previous state variables for rising edge detection
    bool m_last_ka_left = false;
    bool m_last_don_left = false;
    bool m_last_don_right = false;
    bool m_last_ka_right = false;

    // Taiko Tantrum: Reference to drum for accessing calibration state
    Drum *m_drum{nullptr};

    // ADD THIS LINE - Fresh thresholds from SettingsStore
    Peripherals::Drum::Config::Thresholds m_current_thresholds{};

    // Timer for auto-transitioning screens
    uint32_t m_tantrum_results_start{0};
    static constexpr uint32_t TANTRUM_RESULTS_DISPLAY_MS = 5000;

    uint32_t m_tantrum_needsredo_start{0};
    static constexpr uint32_t TANTRUM_NEEDSREDO_DISPLAY_MS = 5000;

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

    // Static dot positions for each drum (x, y)
    static constexpr uint8_t DRUM_CENTERS[4][2] = {
        {16, 43},  // Ka-Left (moved down 4px)
        {47, 43},  // Don-Left
        {78, 43},  // Don-Right
        {109, 43}  // Ka-Right
    };

    // Display rendering functions
    void drawBootSplash();
    void drawIdleScreen();
    void drawMenuScreen();
    void drawNavigationBar(const Utils::Menu::Descriptor &descriptor);
    
    // Taiko Tantrum display functions
    void drawTantrumCountdownScreen();
    void drawTantrumRecordingScreen();
    void drawTantrumResultsScreen();
    void drawTantrumNeedsRedoScreen();
    
    // Reboot countdown display function
    void drawRebootCountdown();

    // Burst animation functions
    void activateRing(uint8_t drum_index);
    void updateRings();
    void drawRings();
    void drawCircleRing(uint8_t x_center, uint8_t y_center, uint8_t radius); 

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

    // Taiko Tantrum integration
    void setDrumReference(Drum *drum);
    void setCurrentThresholds(const Peripherals::Drum::Config::Thresholds &thresholds);
    
    void showIdle();
    void showMenu();
    void showTantrumCountdown();
    void showTantrumRecording();
    void showTantrumResults();
    void showTantrumNeedsRedo();
    void showRebootCountdown();

    void update();
};

} // namespace OuchiTaiko::Peripherals

#endif // PERIPHERALS_DISPLAY_H_

//End of Display.h