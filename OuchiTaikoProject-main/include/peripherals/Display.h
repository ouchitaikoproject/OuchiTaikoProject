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
        TaikoTuneAnalysis,
        TaikoTuneResults,
        TaikoTuneCancelled,
        TaikoTuneAllDrumsSplash,
        TaikoTunePassTransition,
        TaikoTuneComplete,
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

    // Taiko-Tune: Reference to drum for accessing analysis state
    Drum *m_drum{nullptr};

    // Timer for auto-transitioning screens
    uint32_t m_taikotune_results_start{0};
    static constexpr uint32_t RESULTS_DISPLAY_MS = 3000;
    
    uint32_t m_taikotune_cancelled_start{0};
    static constexpr uint32_t CANCELLED_DISPLAY_MS = 2000;
    
    uint32_t m_taikotune_splash_start{0};
    static constexpr uint32_t SPLASH_DISPLAY_MS = 3000;
    
    uint32_t m_taikotune_transition_start{0};
    static constexpr uint32_t TRANSITION_DISPLAY_MS = 3000;

    uint32_t m_taikotune_complete_start{0};
    static constexpr uint32_t COMPLETE_DISPLAY_MS = 5000;

    uint8_t m_current_pass{1};

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
        {16, 39},  // Ka-Left
        {47, 39},  // Don-Left
        {78, 39},  // Don-Right
        {109, 39}  // Ka-Right
    };

    // Display rendering functions
    void drawBootSplash();
    void drawIdleScreen();
    void drawMenuScreen();
    void drawNavigationBar(const Utils::Menu::Descriptor &descriptor);
    
    // Taiko-Tune display functions
    void drawTaikoTuneAnalysisScreen();
    void drawTaikoTuneResultsScreen();
    void drawTaikoTuneCancelledScreen();
    void drawTaikoTuneAllDrumsSplashScreen();
    void drawTaikoTunePassTransitionScreen();
    void drawTaikoTuneCompleteScreen();
    
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
    
    // Taiko-Tune integration
    void setDrumReference(Drum *drum);
    void setCurrentPass(uint8_t pass_number);

    void showIdle();
    void showMenu();
    void showTaikoTuneAnalysis();
    void showTaikoTuneCancelled();
    void showTaikoTuneAllDrumsSplash();
    void showTaikoTunePassTransition();
    void showTaikoTuneComplete();
    void showRebootCountdown();

    void update();
};

} // namespace OuchiTaiko::Peripherals

#endif // PERIPHERALS_DISPLAY_H_

//End of Display.h
