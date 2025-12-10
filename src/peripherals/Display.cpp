// Beginning of file Display.cpp

#include "peripherals/Display.h"
#include "peripherals/Drum.h"

#include "hardware/gpio.h"
#include "pico/time.h"

#include "bitmaps/MenuScreens.h"

#include <list>
#include <numeric>
#include <string>
#include <cmath>
#include <sstream>
#include <cstring>

namespace OuchiTaiko::Peripherals {

// Namespace for bitmap resources (forward declarations to avoid missing bitmaps errors)
namespace bitmaps {
    extern const uint8_t IdleDonLeft[];
    extern const uint8_t IdleKaLeft[];
    extern const uint8_t IdleDonRight[];
    extern const uint8_t IdleKaRight[];
}

// ==========================================================
// File-scope Helper Functions
// ==========================================================

static void drawUpArrow(ssd1306_t *display, int x, int y) {
    ssd1306_draw_pixel(display, x + 2, y);
    ssd1306_draw_pixel(display, x + 1, y + 1);
    ssd1306_draw_pixel(display, x + 2, y + 1);
    ssd1306_draw_pixel(display, x + 3, y + 1);
    ssd1306_draw_pixel(display, x + 1, y + 2);
    ssd1306_draw_pixel(display, x + 2, y + 2);
    ssd1306_draw_pixel(display, x + 3, y + 2);
    ssd1306_draw_pixel(display, x, y + 3);
    ssd1306_draw_pixel(display, x + 1, y + 3);
    ssd1306_draw_pixel(display, x + 2, y + 3);
    ssd1306_draw_pixel(display, x + 3, y + 3);
    ssd1306_draw_pixel(display, x + 4, y + 3);
    ssd1306_draw_pixel(display, x, y + 4);
    ssd1306_draw_pixel(display, x + 1, y + 4);
    ssd1306_draw_pixel(display, x + 2, y + 4);
    ssd1306_draw_pixel(display, x + 3, y + 4);
    ssd1306_draw_pixel(display, x + 4, y + 4);
    ssd1306_draw_pixel(display, x, y + 5);
    ssd1306_draw_pixel(display, x + 1, y + 5);
    ssd1306_draw_pixel(display, x + 2, y + 5);
    ssd1306_draw_pixel(display, x + 3, y + 5);
    ssd1306_draw_pixel(display, x + 4, y + 5);
    ssd1306_draw_pixel(display, x, y + 6);
    ssd1306_draw_pixel(display, x + 1, y + 6);
    ssd1306_draw_pixel(display, x + 2, y + 6);
    ssd1306_draw_pixel(display, x + 3, y + 6);
    ssd1306_draw_pixel(display, x + 4, y + 6);
}

static void drawDownArrow(ssd1306_t *display, int x, int y) {
    ssd1306_draw_pixel(display, x, y);
    ssd1306_draw_pixel(display, x + 1, y);
    ssd1306_draw_pixel(display, x + 2, y);
    ssd1306_draw_pixel(display, x + 3, y);
    ssd1306_draw_pixel(display, x + 4, y);
    ssd1306_draw_pixel(display, x, y + 1);
    ssd1306_draw_pixel(display, x + 1, y + 1);
    ssd1306_draw_pixel(display, x + 2, y + 1);
    ssd1306_draw_pixel(display, x + 3, y + 1);
    ssd1306_draw_pixel(display, x + 4, y + 1);
    ssd1306_draw_pixel(display, x, y + 2);
    ssd1306_draw_pixel(display, x + 1, y + 2);
    ssd1306_draw_pixel(display, x + 2, y + 2);
    ssd1306_draw_pixel(display, x + 3, y + 2);
    ssd1306_draw_pixel(display, x + 4, y + 2);
    ssd1306_draw_pixel(display, x, y + 3);
    ssd1306_draw_pixel(display, x + 1, y + 3);
    ssd1306_draw_pixel(display, x + 2, y + 3);
    ssd1306_draw_pixel(display, x + 3, y + 3);
    ssd1306_draw_pixel(display, x + 4, y + 3);
    ssd1306_draw_pixel(display, x + 1, y + 4);
    ssd1306_draw_pixel(display, x + 2, y + 4);
    ssd1306_draw_pixel(display, x + 3, y + 4);
    ssd1306_draw_pixel(display, x + 1, y + 5);
    ssd1306_draw_pixel(display, x + 2, y + 5);
    ssd1306_draw_pixel(display, x + 3, y + 5);
    ssd1306_draw_pixel(display, x + 2, y + 6);
}

static void drawLeftArrow(ssd1306_t *display, int x, int y) {
    ssd1306_draw_pixel(display, x + 4, y);
    ssd1306_draw_pixel(display, x + 3, y + 1);
    ssd1306_draw_pixel(display, x + 4, y + 1);
    ssd1306_draw_pixel(display, x + 2, y + 2);
    ssd1306_draw_pixel(display, x + 3, y + 2);
    ssd1306_draw_pixel(display, x + 4, y + 2);
    ssd1306_draw_pixel(display, x + 1, y + 3);
    ssd1306_draw_pixel(display, x + 2, y + 3);
    ssd1306_draw_pixel(display, x + 3, y + 3);
    ssd1306_draw_pixel(display, x + 4, y + 3);
    ssd1306_draw_pixel(display, x + 2, y + 4);
    ssd1306_draw_pixel(display, x + 3, y + 4);
    ssd1306_draw_pixel(display, x + 4, y + 4);
    ssd1306_draw_pixel(display, x + 3, y + 5);
    ssd1306_draw_pixel(display, x + 4, y + 5);
    ssd1306_draw_pixel(display, x + 4, y + 6);
}

static void drawRightArrow(ssd1306_t *display, int x, int y) {
    ssd1306_draw_pixel(display, x, y);
    ssd1306_draw_pixel(display, x, y + 1);
    ssd1306_draw_pixel(display, x + 1, y + 1);
    ssd1306_draw_pixel(display, x, y + 2);
    ssd1306_draw_pixel(display, x + 1, y + 2);
    ssd1306_draw_pixel(display, x + 2, y + 2);
    ssd1306_draw_pixel(display, x, y + 3);
    ssd1306_draw_pixel(display, x + 1, y + 3);
    ssd1306_draw_pixel(display, x + 2, y + 3);
    ssd1306_draw_pixel(display, x + 3, y + 3);
    ssd1306_draw_pixel(display, x, y + 4);
    ssd1306_draw_pixel(display, x + 1, y + 4);
    ssd1306_draw_pixel(display, x + 2, y + 4);
    ssd1306_draw_pixel(display, x, y + 5);
    ssd1306_draw_pixel(display, x + 1, y + 5);
    ssd1306_draw_pixel(display, x, y + 6);
}

namespace {

std::string modeToString(usb_mode_t mode) {
    switch (mode) {
    case USB_MODE_SWITCH_TATACON:
        return "Switch Tatacon";
    case USB_MODE_SWITCH_HORIPAD:
        return "Switch Pro";
    case USB_MODE_DUALSHOCK3:
        return "Dualshock 3";
    case USB_MODE_PS4_TATACON:
        return "PS4 Tatacon";
    case USB_MODE_DUALSHOCK4:
        return "Dualshock 4";
    case USB_MODE_KEYBOARD_P1:
        return "Keyboard P1";
    case USB_MODE_KEYBOARD_P2:
        return "Keyboard P2";
    case USB_MODE_XBOX360:
        return "Xbox 360";
    case USB_MODE_ANDROID:
        return "Android";
    case USB_MODE_IOS:
        return "iOS";
    case USB_MODE_XBOX360_ANALOG_P1:
        return "Analog P1";
    case USB_MODE_XBOX360_ANALOG_P2:
        return "Analog P2";
    case USB_MODE_MIDI:
        return "MIDI";
    case USB_MODE_DEBUG:
        return "Debug";
    }
    return "?";
}

} // namespace

// ==========================================================
// Display Class Implementation
// ==========================================================

Display::Display(const Config &config) : 
    m_config(config),
    m_boot_splash_start(to_ms_since_boot(get_absolute_time()))
{
    m_display.external_vcc = false;
    ssd1306_init(&m_display, 128, 64, m_config.i2c_address, m_config.i2c_block);
    ssd1306_clear(&m_display);
    
    // Initialize all rings as inactive
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < MAX_RINGS_PER_DRUM; j++) {
            m_rings[i][j].active = false;
            m_rings[i][j].radius = 0;
        }
    }
    
    // Initialize particles as inactive
    for (int i = 0; i < MAX_PARTICLES; i++) {
        m_particles[i].active = false;
    }
}

void Display::setInputState(const Utils::InputState &state) { m_input_state = state; }
void Display::setUsbMode(usb_mode_t mode) { m_usb_mode = mode; };
void Display::setPlayerId(uint8_t player_id) { m_player_id = player_id; };

void Display::setMenuState(const Utils::Menu::State &menu_state) { m_menu_state = menu_state; }

void Display::setDrumReference(Drum *drum) { m_drum = drum; }

void Display::showIdle() { m_state = State::Idle; }
void Display::showMenu() { m_state = State::Menu; }
void Display::showTaikoTuneAnalysis() { m_state = State::TaikoTuneAnalysis; }

void Display::showTaikoTuneCancelled() {
    m_taikotune_cancelled_start = to_ms_since_boot(get_absolute_time());
    m_state = State::TaikoTuneCancelled;
}

void Display::showTaikoTuneAllDrumsSplash() {
    m_taikotune_splash_start = to_ms_since_boot(get_absolute_time());
    m_state = State::TaikoTuneAllDrumsSplash;
}

void Display::showTaikoTunePassTransition() {
    m_taikotune_transition_start = to_ms_since_boot(get_absolute_time());
    m_state = State::TaikoTunePassTransition;
}

void Display::showTaikoTuneComplete() {
    m_taikotune_complete_start = to_ms_since_boot(get_absolute_time());
    m_state = State::TaikoTuneComplete;
}

void Display::setCurrentPass(uint8_t pass_number) {
    m_current_pass = pass_number;
}

void Display::showRebootCountdown() {
    m_reboot_countdown_start = to_ms_since_boot(get_absolute_time());
    m_state = State::RebootCountdown;
}

void Display::spawnParticles(int x, int y, int count) {
    for (int i = 0; i < count; i++) {
        // Find inactive particle
        for (int p = 0; p < MAX_PARTICLES; p++) {
            if (!m_particles[p].active) {
                m_particles[p].active = true;
                m_particles[p].x = (float)x;
                m_particles[p].y = (float)y;
                
                // Random angle and speed for spark effect
                float angle = (rand() % 360) * 3.14159f / 180.0f;
                float speed = 1.0f + (rand() % 20) / 10.0f; // Speed 1.0-3.0
                
                m_particles[p].vx = cos(angle) * speed;
                m_particles[p].vy = sin(angle) * speed - 0.5f; // Slight upward bias
                
                m_particles[p].life = 20 + (rand() % 20); // Life 20-40 frames
                break;
            }
        }
    }
}

void Display::updateParticles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (m_particles[i].active) {
            // Update position
            m_particles[i].x += m_particles[i].vx;
            m_particles[i].y += m_particles[i].vy;
            
            // Apply gravity
            m_particles[i].vy += 0.15f;
            
            // Decrease life
            m_particles[i].life--;
            
            // Deactivate if life runs out or out of bounds
            if (m_particles[i].life <= 0 || 
                m_particles[i].y > 64 || 
                m_particles[i].x < 0 || 
                m_particles[i].x > 128) {
                m_particles[i].active = false;
            }
        }
    }
}

void Display::drawParticles() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (m_particles[i].active) {
            int px = (int)m_particles[i].x;
            int py = (int)m_particles[i].y;
            
            if (px >= 0 && px < 128 && py >= 0 && py < 64) {
                // Draw particle as 2x2 pixel for visibility
                ssd1306_draw_pixel(&m_display, px, py);
                if (px + 1 < 128) ssd1306_draw_pixel(&m_display, px + 1, py);
                if (py + 1 < 64) ssd1306_draw_pixel(&m_display, px, py + 1);
                if (px + 1 < 128 && py + 1 < 64) ssd1306_draw_pixel(&m_display, px + 1, py + 1);
            }
        }
    }
}

void Display::drawBootSplash() {
    uint32_t elapsed = to_ms_since_boot(get_absolute_time()) - m_boot_splash_start;
    
    // Smooth, satisfying timing
    const uint32_t BLANK_DURATION = 100;
    const uint32_t SLIDE_DURATION = 700;
    const uint32_t BOUNCE_DURATION = 1200;
    const uint32_t SLIDE_END = BLANK_DURATION + SLIDE_DURATION;
    const uint32_t BOUNCE_END = SLIDE_END + BOUNCE_DURATION;
    
    // Drop to the very bottom of the screen
    const int INITIAL_OFFSET = -30;  // Start well above screen
    const int DROP_TO_BOTTOM = 11;   // Final position when touching bottom
    
    int slide_offset = INITIAL_OFFSET;
    bool should_spawn_particles = false;
    static bool particles_spawned = false;
    static uint32_t last_boot_time = 0;
    
    // Reset particle spawn flag on new boot
    if (m_boot_splash_start != last_boot_time) {
        particles_spawned = false;
        last_boot_time = m_boot_splash_start;
    }
    
    if (elapsed < BLANK_DURATION) {
        return;
    } else if (elapsed < SLIDE_END) {
        uint32_t slide_elapsed = elapsed - BLANK_DURATION;
        float progress = (float)slide_elapsed / (float)SLIDE_DURATION;
        
        // Accelerating drop (gravity-like)
        float eased = progress * progress;
        
        // Drop from above to bottom of screen
        slide_offset = INITIAL_OFFSET + (int)(eased * (DROP_TO_BOTTOM - INITIAL_OFFSET));
    } else if (elapsed < BOUNCE_END) {
        uint32_t bounce_elapsed = elapsed - SLIDE_END;
        float bounce_progress = (float)bounce_elapsed / (float)BOUNCE_DURATION;
        
        // Smooth bounces with natural decay - bounces UP from bottom
        float frequency = 3.5f * 2.0f * 3.14159f;  // 3.5 smooth bounces
        float decay = exp(-3.5f * bounce_progress);  // Gentler decay
        
        // Start at bottom (DROP_TO_BOTTOM=11) and bounce upward toward 0
        float bounce_height = sin(frequency * bounce_progress) * 11.0f * decay;
        
        // Interpolate from DROP_TO_BOTTOM to 0 as bounces settle
        float settle = DROP_TO_BOTTOM * (1.0f - bounce_progress);
        
        slide_offset = settle - (int)bounce_height;
        
        // Spawn particles on first impact
        if (!particles_spawned && bounce_elapsed < 50) {
            should_spawn_particles = true;
            particles_spawned = true;
        }
    } else {
        slide_offset = 0;  // Rest position at center
    }
    
    // Spawn particles at actual bottom of screen where text impacts
    if (should_spawn_particles) {
        // Calculate where the bottom of "Project" text is when it hits bottom
        int text_bottom_y = 46 + DROP_TO_BOTTOM + 7;  // y-position + drop + text height
        
        // Center-Spawn, High-Density, High-Velocity Explosion
        const int START_X = 44; 
        const int END_X = 84;   
        
        for (int x = START_X; x < END_X; x++) { 
            // Spawn 3 particles per pixel for a massive, dense burst
            for (int i = 0; i < 3; i++) {
                // Find inactive particle
                for (int p = 0; p < MAX_PARTICLES; p++) {
                    if (!m_particles[p].active) {
                        m_particles[p].active = true;
                        m_particles[p].x = (float)x;
                        m_particles[p].y = (float)text_bottom_y;
                        
                        // Angle: Focused upward spread
                        float angle = -3.14159f / 2.0f + ((rand() % 100) / 100.0f - 0.5f) * 2.0f;
                        
                        // Speed: Higher maximum speed for particles to go higher (3.0-7.9)
                        float speed = 3.0f + (rand() % 50) / 10.0f; 
                        
                        m_particles[p].vx = cos(angle) * speed;
                        m_particles[p].vy = sin(angle) * speed;
                        
                        // Decay: Longer life (40-59 frames)
                        m_particles[p].life = 40 + (rand() % 20); 
                        break;
                    }
                }
            }
        }
    }
    
    // Update and draw particles
    updateParticles();
    drawParticles();
    
    // Draw text
    const char* line1 = "KillerQ's";
    int line1_width = strlen(line1) * 6;
    ssd1306_draw_string(&m_display, (128 - line1_width) / 2, 10 + slide_offset, 1, line1);
    
    const char* line2 = "OuchiTaiko";
    int line2_width = strlen(line2) * 12;
    ssd1306_draw_string(&m_display, (128 - line2_width) / 2, 24 + slide_offset, 2, line2);
    
    const char* line3 = "Project";
    int line3_width = strlen(line3) * 6;
    ssd1306_draw_string(&m_display, (128 - line3_width) / 2, 46 + slide_offset, 1, line3);
}

void Display::drawIdleScreen() {
    std::string mode_string = modeToString(m_usb_mode);
    mode_string += " Mode";
    ssd1306_draw_string(&m_display, 0, 0, 1, mode_string.c_str());
    ssd1306_draw_line(&m_display, 0, 10, 128, 10);

    // Option 2: Left-align "Streak:" label, right-align number
    const char* streak_label = "Streak: ";
    ssd1306_draw_string(&m_display, 2, 16, 1, streak_label);

    // Right-align the number at a fixed position (reserve space for 3 digits)
    std::string roll_number = std::to_string(m_input_state.drum.current_roll);
    int number_width = roll_number.length() * 6;
    int number_x = 62 - number_width;  // Right-align at x=62
    ssd1306_draw_string(&m_display, number_x, 16, 1, roll_number.c_str());

    // Calculate and display roll rate (rolls/second)
    static uint32_t last_roll_count = 0;
    static uint32_t last_roll_time = 0;
    static uint16_t displayed_roll_rate = 0;  // Smoothed roll rate for display

    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    // Update roll rate when roll count changes
    if (m_input_state.drum.current_roll > last_roll_count && m_input_state.drum.current_roll > 1) {
        uint32_t time_delta = current_time - last_roll_time;
        if (time_delta > 0) {
            // Calculate rolls per second: (1000ms / time_per_hit)
            // Each roll increment represents one hit, so rate = 1000 / time_delta
            uint16_t instant_rate = 1000 / time_delta;
            // Smooth the displayed value to reduce jitter
            displayed_roll_rate = (displayed_roll_rate * 3 + instant_rate) / 4;
        }
        last_roll_time = current_time;
    }
    last_roll_count = m_input_state.drum.current_roll;

    // Reset displayed rate if no hits for 1 second
    if (current_time - last_roll_time > 1000) {
        displayed_roll_rate = 0;
    }

    // Display roll rate below streak
    const char* rate_label = "Rate: ";
    ssd1306_draw_string(&m_display, 2, 24, 1, rate_label);
    std::string rate_string = std::to_string(displayed_roll_rate) + "/s";
    int rate_width = rate_string.length() * 6;
    int rate_x = 62 - rate_width;
    ssd1306_draw_string(&m_display, rate_x, 24, 1, rate_string.c_str());

    // Detect hits (rising edge)
    bool ka_left_hit = m_input_state.drum.ka_left.triggered && !m_last_ka_left;
    bool don_left_hit = m_input_state.drum.don_left.triggered && !m_last_don_left;
    bool don_right_hit = m_input_state.drum.don_right.triggered && !m_last_don_right;
    bool ka_right_hit = m_input_state.drum.ka_right.triggered && !m_last_ka_right;
    
    // Activate bursts on hits
    if (ka_left_hit) activateRing(0);
    if (don_left_hit) activateRing(1);
    if (don_right_hit) activateRing(2);
    if (ka_right_hit) activateRing(3);
    
    // Update last state
    m_last_ka_left = m_input_state.drum.ka_left.triggered;
    m_last_don_left = m_input_state.drum.don_left.triggered;
    m_last_don_right = m_input_state.drum.don_right.triggered;
    m_last_ka_right = m_input_state.drum.ka_right.triggered;

    // Update and draw burst animations
    updateRings();
    drawRings();
    
    // Draw static center dots for each drum (3x3 centered)
    for (int i = 0; i < 4; i++) {
        uint8_t x = DRUM_CENTERS[i][0];
        uint8_t y = DRUM_CENTERS[i][1];
        // Draw a 3x3 pixel dot (centered on x, y)
        ssd1306_draw_pixel(&m_display, x - 1, y - 1);
        ssd1306_draw_pixel(&m_display, x, y - 1);
        ssd1306_draw_pixel(&m_display, x + 1, y - 1);
        ssd1306_draw_pixel(&m_display, x - 1, y);
        ssd1306_draw_pixel(&m_display, x, y);
        ssd1306_draw_pixel(&m_display, x + 1, y);
        ssd1306_draw_pixel(&m_display, x - 1, y + 1);
        ssd1306_draw_pixel(&m_display, x, y + 1);
        ssd1306_draw_pixel(&m_display, x + 1, y + 1);
    }

    // Button tester - show first pressed button in top-right
    // BUT disable during SELECT/START holds to avoid confusion
    const char* button_label = nullptr;
    
    // Check if we're in a hold state (SELECT or START held for significant time)
    static uint32_t select_hold_time = 0;
    static uint32_t start_hold_time = 0;
    static bool select_was_pressed = false;
    static bool start_was_pressed = false;
    
    bool select_currently_pressed = m_input_state.controller.buttons.select;
    bool start_currently_pressed = m_input_state.controller.buttons.start;
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    
    // Track hold times
    if (select_currently_pressed && !select_was_pressed) {
        select_hold_time = current_time;
    } else if (!select_currently_pressed) {
        select_hold_time = 0;
    }
    
    if (start_currently_pressed && !start_was_pressed) {
        start_hold_time = current_time;
    } else if (!start_currently_pressed) {
        start_hold_time = 0;
    }
    
    select_was_pressed = select_currently_pressed;
    start_was_pressed = start_currently_pressed;
    
    // Disable button tester if SELECT or START held for >300ms
    bool in_hold_state = (select_hold_time != 0 && (current_time - select_hold_time) > 300) ||
                         (start_hold_time != 0 && (current_time - start_hold_time) > 300);
    
    if (!in_hold_state) {
        // Check buttons in priority order (first pressed wins)
        if (m_input_state.controller.dpad.up) button_label = "Up";
        else if (m_input_state.controller.dpad.down) button_label = "Down";
        else if (m_input_state.controller.dpad.left) button_label = "Left";
        else if (m_input_state.controller.dpad.right) button_label = "Right";
        else if (m_input_state.controller.buttons.north) button_label = "North";
        else if (m_input_state.controller.buttons.south) button_label = "South";
        else if (m_input_state.controller.buttons.east) button_label = "East";
        else if (m_input_state.controller.buttons.west) button_label = "West";
        else if (m_input_state.controller.buttons.l) button_label = "L";
        else if (m_input_state.controller.buttons.r) button_label = "R";
        else if (m_input_state.controller.buttons.start) button_label = "Start";
        else if (m_input_state.controller.buttons.select) button_label = "Select";
        else if (m_input_state.controller.buttons.home) button_label = "Home";
        else if (m_input_state.controller.buttons.share) button_label = "Share";
    }
    
    if (button_label) {
        // Draw in top-right corner, right-aligned with 1px margin
        int label_width = strlen(button_label) * 6;
        ssd1306_draw_string(&m_display, 128 - label_width - 1, 16, 1, button_label);
    }

    ssd1306_draw_line(&m_display, 0, 54, 128, 54);
    ssd1306_draw_string(&m_display, 0, 56, 1, "Hold SELECT for Menu");
}

void Display::drawNavigationBar(const Utils::Menu::Descriptor &descriptor) {
    ssd1306_draw_line(&m_display, 0, 54, 127, 54);
    
    std::string left_label = "Back";
    std::string right_label = "OK";
    bool show_left_right_arrows = false;
    bool show_up_down_arrows = false;
    
    // Hide OK button if on DrumBigHitArcade and already on current mode
    bool hide_ok = (m_menu_state.page == Utils::Menu::Page::DrumBigHitArcade &&
                    m_menu_state.selected_value == m_menu_state.original_value);
    
    switch (descriptor.type) {
    case Utils::Menu::Descriptor::Type::Menu:
    case Utils::Menu::Descriptor::Type::Selection:
    case Utils::Menu::Descriptor::Type::Toggle:
        show_left_right_arrows = true;
        break;
    case Utils::Menu::Descriptor::Type::Value:
        show_up_down_arrows = true;
        right_label = "Save";
        break;
    case Utils::Menu::Descriptor::Type::RebootInfo:
    case Utils::Menu::Descriptor::Type::RebootCountdown:
        right_label = "Reboot";
        left_label = "";
        break;
    }
    
    std::string nav_text = "";
    if (!left_label.empty()) {
        nav_text += "B:" + left_label;
        if (!hide_ok) {
            nav_text += " ";
        }
    }
    if (!hide_ok) {
        nav_text += "A:" + right_label;
    }
    
    int text_width = nav_text.length() * 6;
    int arrows_width = 0;
    int nav_label_width = 18;
    
    if (show_left_right_arrows) {
        arrows_width = 12;
    } else if (show_up_down_arrows) {
        arrows_width = 13;
    }
    
    int extra_spacing = show_left_right_arrows || show_up_down_arrows ? 18 : 0;
    int total_width = arrows_width + nav_label_width + extra_spacing + text_width;
    
    int start_x = (128 - total_width) / 2;
    
    if (show_left_right_arrows) {
        drawLeftArrow(&m_display, start_x, 56);
        drawRightArrow(&m_display, start_x + 7, 56);
        ssd1306_draw_string(&m_display, start_x + 15, 56, 1, "Nav");
    } else if (show_up_down_arrows) {
        drawUpArrow(&m_display, start_x, 56);
        drawDownArrow(&m_display, start_x + 8, 56);
        ssd1306_draw_string(&m_display, start_x + 16, 56, 1, "Adj");
    }
    
    int text_x = start_x + arrows_width + nav_label_width + extra_spacing;
    ssd1306_draw_string(&m_display, text_x, 56, 1, nav_text.c_str());
}

void Display::drawMenuScreen() {
    auto descriptor_it = Utils::Menu::descriptors.find(m_menu_state.page);
    if (descriptor_it == Utils::Menu::descriptors.end()) {
        return;
    }

    ssd1306_draw_line(&m_display, 0, 10, 127, 10);

    ssd1306_draw_string(&m_display, 0, 0, 1, descriptor_it->second.name.c_str());

    switch (descriptor_it->second.type) {
    case Utils::Menu::Descriptor::Type::Menu:
    case Utils::Menu::Descriptor::Type::Selection:
    case Utils::Menu::Descriptor::Type::Toggle: {
        auto selection_count = descriptor_it->second.items.size();
        auto current_page = m_menu_state.selected_value + 1;
        
        std::string page_indicator = "[" + std::to_string(current_page) + "/" + std::to_string(selection_count) + "]";
        int indicator_width = page_indicator.length() * 6;
        ssd1306_draw_string(&m_display, 128 - indicator_width, 0, 1, page_indicator.c_str());
    } break;
    case Utils::Menu::Descriptor::Type::RebootInfo:
        // Just show a static info page, no countdown
        break;
    case Utils::Menu::Descriptor::Type::RebootCountdown:
        // Trigger countdown display for this type only
        showRebootCountdown();
        return;
    case Utils::Menu::Descriptor::Type::Value:
        break;
    }

    if (descriptor_it->second.type == Utils::Menu::Descriptor::Type::Value) {
        const int bar_x = 2;
        const int bar_height = 8;
        const int bar_width = 124;
        
        const int group_start_y = 17;
        
        std::string value_str = "(" + std::to_string(m_menu_state.selected_value) + ")";
        ssd1306_draw_string(&m_display, (128 - (value_str.length() * 6)) / 2, group_start_y, 1, value_str.c_str());
        
        uint16_t max_val = descriptor_it->second.max_value;
        std::string min_str = "0";
        std::string max_str = std::to_string(max_val);
        ssd1306_draw_string(&m_display, bar_x, group_start_y + 12, 1, min_str.c_str());
        int max_str_width = max_str.length() * 6;
        ssd1306_draw_string(&m_display, 128 - max_str_width, group_start_y + 12, 1, max_str.c_str());
        
        const int bar_y = group_start_y + 22;
        
        ssd1306_draw_line(&m_display, bar_x, bar_y, bar_x + bar_width, bar_y);
        ssd1306_draw_line(&m_display, bar_x, bar_y + bar_height, bar_x + bar_width, bar_y + bar_height);
        ssd1306_draw_line(&m_display, bar_x, bar_y, bar_x, bar_y + bar_height);
        ssd1306_draw_line(&m_display, bar_x + bar_width, bar_y, bar_x + bar_width, bar_y + bar_height);
        
        if (max_val > 0) {
            int fill_width = (m_menu_state.selected_value * (bar_width - 4)) / max_val;
            
            if (fill_width > 0) {
                for (int y = bar_y + 2; y < bar_y + bar_height - 2; y++) {
                    for (int x = bar_x + 2; x < bar_x + 2 + fill_width; x++) {
                        ssd1306_draw_pixel(&m_display, x, y);
                    }
                }
            }
			}
        
    } else {
        std::string selection;
        switch (descriptor_it->second.type) {
        case Utils::Menu::Descriptor::Type::Menu:
        case Utils::Menu::Descriptor::Type::Selection:
        case Utils::Menu::Descriptor::Type::RebootInfo:
            selection = descriptor_it->second.items.at(m_menu_state.selected_value).first;
            break;
        case Utils::Menu::Descriptor::Type::RebootCountdown:
            selection = descriptor_it->second.items.at(m_menu_state.selected_value).first;
            break;
        case Utils::Menu::Descriptor::Type::Toggle:
            // Special display for Bootsel, Reset, and ResetThresholds pages (confirmation dialogs)
            if (m_menu_state.page == Utils::Menu::Page::Bootsel || 
                m_menu_state.page == Utils::Menu::Page::Reset ||
                m_menu_state.page == Utils::Menu::Page::DrumTriggerThresholdsReset) {
                if (m_menu_state.selected_value == 0) {
                    selection = "Yes [No]";
                } else {
                    selection = "[Yes] No";
                }
            } else {
                // Default Off/On for other toggles
                if (m_menu_state.selected_value == 0) {
                    selection = "[Off] On";
                } else {
                    selection = "Off [On]";
                }
            }
            break;
        case Utils::Menu::Descriptor::Type::Value:
            break;
        }
        
        // Special handling for RebootInfo pages with multiple lines
        if (descriptor_it->second.type == Utils::Menu::Descriptor::Type::RebootInfo) {
            std::vector<std::string> lines;
            size_t start = 0;
            size_t newline_pos;
            
            // Split text into lines
            while ((newline_pos = selection.find('\n', start)) != std::string::npos) {
                lines.push_back(selection.substr(start, newline_pos - start));
                start = newline_pos + 1;
            }
            // Add the last line (or only line if no newlines)
            lines.push_back(selection.substr(start));
            
            // Draw lines centered vertically and horizontally
            int line_height = 8;  // Height per line
            int total_height = lines.size() * line_height;
            int start_y = (64 - 10 - total_height) / 2 + 10;  // Center in remaining space below header
            
            for (size_t i = 0; i < lines.size(); i++) {
                int text_x = (128 - (lines[i].length() * 6)) / 2;
                int text_y = start_y + (i * line_height);
                ssd1306_draw_string(&m_display, text_x, text_y, 1, lines[i].c_str());
            }
        } else {
            // Original two-line handling for other menu types
            size_t newline_pos = selection.find('\n');
            if (newline_pos != std::string::npos) {
                std::string line1 = selection.substr(0, newline_pos);
                std::string line2 = selection.substr(newline_pos + 1);
                
                ssd1306_draw_string(&m_display, (128 - (line1.length() * 6)) / 2, 24, 1, line1.c_str());
                ssd1306_draw_string(&m_display, (128 - (line2.length() * 6)) / 2, 32, 1, line2.c_str());
            } else {
                int text_x = (128 - (selection.length() * 6)) / 2;
                ssd1306_draw_string(&m_display, text_x, 28, 1, selection.c_str());
            }
            
            // Show "(Current Mode)" for DrumBigHitArcade if this is the current setting
            if (m_menu_state.page == Utils::Menu::Page::DrumBigHitArcade &&
                m_menu_state.selected_value == m_menu_state.original_value) {
                const char* current_text = "(Current Mode)";
                int current_x = (128 - (strlen(current_text) * 6)) / 2;
                ssd1306_draw_string(&m_display, current_x, 40, 1, current_text);
            }
        }
        
        if (descriptor_it->second.type == Utils::Menu::Descriptor::Type::Menu ||
            descriptor_it->second.type == Utils::Menu::Descriptor::Type::Selection) {
            
            auto selection_count = descriptor_it->second.items.size();
            
            if (selection_count > 1) {
                if (m_menu_state.selected_value > 0) {
                    drawLeftArrow(&m_display, 2, 28);
                }
                
                if (m_menu_state.selected_value < selection_count - 1) {
                    drawRightArrow(&m_display, 121, 28);
                }
            }
        }
    }
    
    if (descriptor_it->second.type != Utils::Menu::Descriptor::Type::RebootInfo &&
        descriptor_it->second.type != Utils::Menu::Descriptor::Type::RebootCountdown) {
        drawNavigationBar(descriptor_it->second);
    }
}

void Display::drawRebootCountdown() {
    uint32_t elapsed = to_ms_since_boot(get_absolute_time()) - m_reboot_countdown_start;
    
    // Check if this is bootloader countdown or normal reboot countdown
    bool is_bootloader = (m_menu_state.page == Utils::Menu::Page::BootselMsg);
    
    if (is_bootloader) {
        // Bootloader mode: Show countdown WITH instructions
        uint8_t countdown = 5;
        if (elapsed < REBOOT_COUNTDOWN_MS) {
            countdown = 5 - (elapsed / 1000);
        } else {
            countdown = 0;
        }
        
        // Title
        const char* line1 = "Entering Bootloader";
        int line1_x = (128 - strlen(line1) * 6) / 2;
        ssd1306_draw_string(&m_display, line1_x, 4, 1, line1);
        
        // Countdown
        std::string countdown_text = "Starting in " + std::to_string(countdown) + "...";
        int line2_x = (128 - countdown_text.length() * 6) / 2;
        ssd1306_draw_string(&m_display, line2_x, 16, 1, countdown_text.c_str());
        
        // Instructions (always visible)
        ssd1306_draw_line(&m_display, 10, 26, 117, 26);
        
        const char* line3 = "Then Drag .uf2 File";
        int line3_x = (128 - strlen(line3) * 6) / 2;
        ssd1306_draw_string(&m_display, line3_x, 32, 1, line3);
        
        const char* line4 = "If No Auto-Reboot:";
        int line4_x = (128 - strlen(line4) * 6) / 2;
        ssd1306_draw_string(&m_display, line4_x, 42, 1, line4);
        
        const char* line5 = "Unplug & Replug USB";
        int line5_x = (128 - strlen(line5) * 6) / 2;
        ssd1306_draw_string(&m_display, line5_x, 52, 1, line5);
        
    } else {
        // Normal reboot countdown screen
        uint8_t countdown = 5;
        if (elapsed >= REBOOT_COUNTDOWN_MS) {
            countdown = 0;
        } else {
            countdown = 5 - (elapsed / 1000);
        }
        
        const char* line1 = "Applying Changes...";
        int line1_x = (128 - strlen(line1) * 6) / 2;
        ssd1306_draw_string(&m_display, line1_x, 8, 1, line1);
        
        std::string countdown_text = "Reboot in " + std::to_string(countdown) + " seconds";
        int line2_x = (128 - countdown_text.length() * 6) / 2;
        ssd1306_draw_string(&m_display, line2_x, 24, 1, countdown_text.c_str());
        
        const char* line3 = "If No Reboot, Unplug";
        int line3_x = (128 - strlen(line3) * 6) / 2;
        ssd1306_draw_string(&m_display, line3_x, 40, 1, line3);
        
        const char* line4 = "& Replug USB to Apply";
        int line4_x = (128 - strlen(line4) * 6) / 2;
        ssd1306_draw_string(&m_display, line4_x, 52, 1, line4);
    }
}

void Display::drawTaikoTuneAnalysisScreen() {
    if (!m_drum) return;
    
    const auto& tt_state = m_drum->getTaikoTuneState();
    
    if (tt_state.current_mode == Drum::TaikoTuneState::Mode::ShowingResults) {
        m_state = State::TaikoTuneResults;
        m_taikotune_results_start = to_ms_since_boot(get_absolute_time());
        return;
    }
    
    // Show pass indicator
    std::string header;
    if (m_current_pass == 1) {
        header = "Pass 1 of 2 (This Way -->)";
    } else {
        header = "Pass 2 of 2 (Opposite <--)";
    }
    int header_x = (128 - header.length() * 6) / 2;
    ssd1306_draw_string(&m_display, header_x, 0, 1, header.c_str());
    
    ssd1306_draw_line(&m_display, 0, 10, 127, 10);
    
    std::string pad_name;
    switch (tt_state.current_pad) {
        case Drum::Id::KA_LEFT: pad_name = "Ka-Left"; break;
        case Drum::Id::DON_LEFT: pad_name = "Don-Left"; break;
        case Drum::Id::DON_RIGHT: pad_name = "Don-Right"; break;
        case Drum::Id::KA_RIGHT: pad_name = "Ka-Right"; break;
    }
    
    if (!tt_state.countdown_complete) {
        std::string analyzing = "Analyzing " + pad_name;
        int analyzing_x = (128 - analyzing.length() * 6) / 2;
        ssd1306_draw_string(&m_display, analyzing_x, 14, 1, analyzing.c_str());
        
        uint8_t countdown = tt_state.getCountdownRemaining();
        std::string countdown_str = std::to_string(countdown);
        int countdown_x = (128 - countdown_str.length() * 12) / 2;
        ssd1306_draw_string(&m_display, countdown_x, 28, 2, countdown_str.c_str());
        
        std::string noise_msg = "Noise: " + std::to_string(tt_state.max_noise_level);
        int noise_x = (128 - noise_msg.length() * 6) / 2;
        ssd1306_draw_string(&m_display, noise_x, 46, 1, noise_msg.c_str());
        
        const char* bottom_msg = "Don't Touch Drums Yet";
        int bottom_x = (128 - strlen(bottom_msg) * 6) / 2;
        ssd1306_draw_string(&m_display, bottom_x, 56, 1, bottom_msg);
        
    } else {
        std::string analyzing = "Analyzing " + pad_name;
        int analyzing_x = (128 - analyzing.length() * 6) / 2;
        ssd1306_draw_string(&m_display, analyzing_x, 14, 1, analyzing.c_str());
        
        const char* instruction = "Hit Naturally to 100%";
        int instruction_x = (128 - strlen(instruction) * 6) / 2;
        ssd1306_draw_string(&m_display, instruction_x, 26, 1, instruction);
        
        const int BAR_X = 10;
        const int BAR_Y = 36;
        const int BAR_WIDTH = 88;
        const int BAR_HEIGHT = 10;
        
        ssd1306_draw_line(&m_display, BAR_X, BAR_Y, BAR_X + BAR_WIDTH, BAR_Y);
        ssd1306_draw_line(&m_display, BAR_X, BAR_Y + BAR_HEIGHT, BAR_X + BAR_WIDTH, BAR_Y + BAR_HEIGHT);
        ssd1306_draw_line(&m_display, BAR_X, BAR_Y, BAR_X, BAR_Y + BAR_HEIGHT);
        ssd1306_draw_line(&m_display, BAR_X + BAR_WIDTH, BAR_Y, BAR_X + BAR_WIDTH, BAR_Y + BAR_HEIGHT);
        
        uint16_t percentage = (tt_state.hits_collected * 100) / Drum::TaikoTuneState::TARGET_HITS;
        if (percentage > 100) percentage = 100;
        
        int fill_width = (percentage * (BAR_WIDTH - 4)) / 100;
        if (fill_width > 0) {
            for (int y = BAR_Y + 2; y < BAR_Y + BAR_HEIGHT - 1; y++) {
                for (int x = BAR_X + 2; x < BAR_X + 2 + fill_width; x++) {
                    ssd1306_draw_pixel(&m_display, x, y);
                }
            }
        }
        
        std::string percent_str = std::to_string(percentage) + "%";
        ssd1306_draw_string(&m_display, BAR_X + BAR_WIDTH + 6, BAR_Y + 1, 1, percent_str.c_str());
        
        const char* bottom_msg = "Press B to Cancel";
        int bottom_x = (128 - strlen(bottom_msg) * 6) / 2;
        ssd1306_draw_string(&m_display, bottom_x, 56, 1, bottom_msg);
    }
}

void Display::drawTaikoTuneResultsScreen() {
    if (!m_drum) return;

    const auto& tt_state = m_drum->getTaikoTuneState();

    // CRITICAL FIX: Check if Drum state changed (All 4 Drums mode started next drum)
    // If Drum is no longer in ShowingResults, exit immediately and let update() show correct screen
    if (tt_state.current_mode != Drum::TaikoTuneState::Mode::ShowingResults) {
        m_state = State::TaikoTuneAnalysis;  // Switch to analysis screen
        return;
    }

    const auto& rec = tt_state.recommendations.at(tt_state.current_pad);

    uint32_t elapsed = to_ms_since_boot(get_absolute_time()) - m_taikotune_results_start;
    if (elapsed >= RESULTS_DISPLAY_MS) {
        m_state = State::Menu;
        return;
    }
    
    const char* header = "Complete!";
    int header_x = (128 - strlen(header) * 12) / 2;
    ssd1306_draw_string(&m_display, header_x, 8, 2, header);
    
    std::string pad_name;
    switch (tt_state.current_pad) {
        case Drum::Id::KA_LEFT: pad_name = "Ka-Left"; break;
        case Drum::Id::DON_LEFT: pad_name = "Don-Left"; break;
        case Drum::Id::DON_RIGHT: pad_name = "Don-Right"; break;
        case Drum::Id::KA_RIGHT: pad_name = "Ka-Right"; break;
    }
    
    std::string msg1 = pad_name + " threshold";
    int msg1_x = (128 - msg1.length() * 6) / 2;
    ssd1306_draw_string(&m_display, msg1_x, 30, 1, msg1.c_str());
    
    std::string msg2 = "adjusted to: " + std::to_string(rec.suggested_threshold);
    int msg2_x = (128 - msg2.length() * 6) / 2;
    ssd1306_draw_string(&m_display, msg2_x, 40, 1, msg2.c_str());

    // Context-aware bottom message
    const char* bottom = (m_current_pass >= 1) ? "Next drum starting..." : "Returning to menu...";
    int bottom_x = (128 - strlen(bottom) * 6) / 2;
    ssd1306_draw_string(&m_display, bottom_x, 56, 1, bottom);
}

void Display::drawTaikoTuneAllDrumsSplashScreen() {
    // Auto-transition after 3 seconds
    uint32_t elapsed = to_ms_since_boot(get_absolute_time()) - m_taikotune_splash_start;
    if (elapsed >= SPLASH_DISPLAY_MS) {
        m_state = State::TaikoTuneAnalysis;
        return;
    }
    
    // Title
    const char* line1 = "All 4 Drums Mode";
    int line1_x = (128 - strlen(line1) * 6) / 2;
    ssd1306_draw_string(&m_display, line1_x, 4, 1, line1);
    
    ssd1306_draw_line(&m_display, 10, 14, 117, 14);
    
    // Pass info
    const char* line2 = "PASS 1: This Way -->";
    int line2_x = (128 - strlen(line2) * 6) / 2;
    ssd1306_draw_string(&m_display, line2_x, 20, 1, line2);
    
    const char* line3 = "PASS 2: Opposite <--";
    int line3_x = (128 - strlen(line3) * 6) / 2;
    ssd1306_draw_string(&m_display, line3_x, 30, 1, line3);
    
    ssd1306_draw_line(&m_display, 10, 40, 117, 40);
    
    // Stats
    const char* line4 = "The Screens Will Tell";
    int line4_x = (128 - strlen(line4) * 6) / 2;
    ssd1306_draw_string(&m_display, line4_x, 46, 1, line4);
    
    const char* line5 = "Which Drums To Hit";
    int line5_x = (128 - strlen(line5) * 6) / 2;
    ssd1306_draw_string(&m_display, line5_x, 56, 1, line5);
}

void Display::drawTaikoTuneCancelledScreen() {
    uint32_t elapsed = to_ms_since_boot(get_absolute_time()) - m_taikotune_cancelled_start;
    if (elapsed >= CANCELLED_DISPLAY_MS) {
        m_state = State::Menu;
        return;
    }
    
    // Title
    const char* header = "Cancelled";
    int header_x = (128 - strlen(header) * 12) / 2;
    ssd1306_draw_string(&m_display, header_x, 16, 2, header);
    
    // Message
    const char* msg = "Analysis stopped";
    int msg_x = (128 - strlen(msg) * 6) / 2;
    ssd1306_draw_string(&m_display, msg_x, 36, 1, msg);
    
    const char* msg2 = "Thresholds restored";
    int msg2_x = (128 - strlen(msg2) * 6) / 2;
    ssd1306_draw_string(&m_display, msg2_x, 46, 1, msg2);
}

void Display::drawTaikoTunePassTransitionScreen() {
    // Auto-transition after 3 seconds
    uint32_t elapsed = to_ms_since_boot(get_absolute_time()) - m_taikotune_transition_start;
    if (elapsed >= TRANSITION_DISPLAY_MS) {
        m_state = State::TaikoTuneAnalysis;
        return;
    }
    
    // Title
    const char* line1 = "Pass 1 Complete!";
    int line1_x = (128 - strlen(line1) * 12) / 2;
    ssd1306_draw_string(&m_display, line1_x, 8, 2, line1);
    
    ssd1306_draw_line(&m_display, 10, 28, 117, 28);
    
    // Instructions
    const char* line2 = "Starting Pass 2";
    int line2_x = (128 - strlen(line2) * 6) / 2;
    ssd1306_draw_string(&m_display, line2_x, 34, 1, line2);
    
    const char* line3 = "(Opposite Direction)";
    int line3_x = (128 - strlen(line3) * 6) / 2;
    ssd1306_draw_string(&m_display, line3_x, 44, 1, line3);
    
    const char* line4 = "Don't Touch Drums Yet";
    int line4_x = (128 - strlen(line4) * 6) / 2;
    ssd1306_draw_string(&m_display, line4_x, 56, 1, line4);
}

void Display::drawTaikoTuneCompleteScreen() {
    // Display doesn't auto-transition - Main.cpp handles timeout and exit

    // Title
    const char* line1 = "All Drums";
    int line1_x = (128 - strlen(line1) * 12) / 2;
    ssd1306_draw_string(&m_display, line1_x, 4, 2, line1);

    const char* line2 = "Complete!";
    int line2_x = (128 - strlen(line2) * 12) / 2;
    ssd1306_draw_string(&m_display, line2_x, 20, 2, line2);

    ssd1306_draw_line(&m_display, 10, 38, 117, 38);

    // Success message
    const char* line3 = "All Thresholds Saved";
    int line3_x = (128 - strlen(line3) * 6) / 2;
    ssd1306_draw_string(&m_display, line3_x, 44, 1, line3);

    const char* line4 = "Returning to menu...";
    int line4_x = (128 - strlen(line4) * 6) / 2;
    ssd1306_draw_string(&m_display, line4_x, 56, 1, line4);
}

// ==========================================================
// CUSTOM BURST ANIMATION FUNCTIONS (Rings)
// ==========================================================

void Display::activateRing(uint8_t drum_index) {
    if (drum_index >= 4) return;
    
    // Find an inactive ring slot for this drum
    for (int i = 0; i < MAX_RINGS_PER_DRUM; i++) {
        if (!m_rings[drum_index][i].active) {
            m_rings[drum_index][i].active = true;
            // Set radius to 12 (a few pixels larger than original)
            m_rings[drum_index][i].radius = 12; 
            break;
        }
    }
}

void Display::updateRings() {
    for (int drum = 0; drum < 4; drum++) {
        for (int ring = 0; ring < MAX_RINGS_PER_DRUM; ring++) {
            if (m_rings[drum][ring].active) {
                // SHRINK rapidly (Decay by 2 per frame for fast disappearance)
                if (m_rings[drum][ring].radius > 0) {
                    m_rings[drum][ring].radius -= 2;
                } else {
                    m_rings[drum][ring].active = false;
                }
            }
        }
    }
}

void Display::drawRings() {
    for (int drum = 0; drum < 4; drum++) {
        uint8_t x = DRUM_CENTERS[drum][0];
        uint8_t y = DRUM_CENTERS[drum][1];
        
        for (int ring = 0; ring < MAX_RINGS_PER_DRUM; ring++) {
            if (m_rings[drum][ring].active) {
                drawCircleRing(x, y, m_rings[drum][ring].radius);
            }
        }
    }
}

// Draws a circle outline (ring) using a simplified midpoint algorithm
void Display::drawCircleRing(uint8_t x_center, uint8_t y_center, uint8_t radius) {
    if (radius <= 0) return;

    int x = radius;
    int y = 0;
    int err = 0;

    auto draw_pixel_safe = [&](int px, int py) {
        int draw_x = x_center + px;
        int draw_y = y_center + py;
        // Bounds check
        if (draw_x >= 0 && draw_x < 128 && draw_y >= 0 && draw_y < 64) {
            ssd1306_draw_pixel(&m_display, draw_x, draw_y);
        }
    };

    while (x >= y) {
        // Draw the 8 octants
        draw_pixel_safe(x, y);
        draw_pixel_safe(y, x);
        draw_pixel_safe(-y, x);
        draw_pixel_safe(-x, y);
        draw_pixel_safe(-x, -y);
        draw_pixel_safe(-y, -x);
        draw_pixel_safe(y, -x);
        draw_pixel_safe(x, -y);

        if (err <= 0) {
            y += 1;
            err += 2 * y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}

void Display::update() {
    static const uint32_t interval_ms = 16;  // 60 FPS

    if (to_ms_since_boot(get_absolute_time()) - m_next_frame_time < interval_ms) {
        return;
    }
    m_next_frame_time += interval_ms;

    if (m_state == State::BootSplash) {
        uint32_t elapsed = to_ms_since_boot(get_absolute_time()) - m_boot_splash_start;
        if (elapsed >= BOOT_SPLASH_DURATION_MS) {
            m_state = State::Idle;
        }
    }
    
    ssd1306_clear(&m_display);

    switch (m_state) {
    case State::BootSplash:
        drawBootSplash();
        break;
    case State::Idle:
        drawIdleScreen();
        break;
    case State::Menu:
        drawMenuScreen();
        break;
    case State::TaikoTuneAnalysis:
        drawTaikoTuneAnalysisScreen();
        break;
    case State::TaikoTuneResults:
        drawTaikoTuneResultsScreen();
        break;
    case State::TaikoTuneAllDrumsSplash:
        drawTaikoTuneAllDrumsSplashScreen();
        break;
    case State::TaikoTuneCancelled:
        drawTaikoTuneCancelledScreen();
        break;
    case State::TaikoTunePassTransition:
        drawTaikoTunePassTransitionScreen();
        break;
    case State::TaikoTuneComplete:
        drawTaikoTuneCompleteScreen();
        break;
    case State::RebootCountdown:
        drawRebootCountdown();
        break;
    }

    ssd1306_show(&m_display);
}

} // namespace OuchiTaiko::Peripherals

// End of file Display.cpp