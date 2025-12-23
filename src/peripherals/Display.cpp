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

void Display::showTantrumCountdown() {
    m_state = State::TantrumCountdown;
}

void Display::showTantrumRecording() {
    m_state = State::TantrumRecording;
}

void Display::showTantrumResults() {
    m_tantrum_results_start = to_ms_since_boot(get_absolute_time());
    m_state = State::TantrumResults;
}

void Display::showTantrumNeedsRedo() {
    m_tantrum_needsredo_start = to_ms_since_boot(get_absolute_time());
    m_state = State::TantrumNeedsRedo;
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

    // Display Streak in size 1 font - centered with 3-digit padding
    const char* streak_label = "Streak: ";
    std::string roll_number = std::to_string(m_input_state.drum.current_roll);
    
    // Pad number to 3 digits with leading spaces
    while (roll_number.length() < 3) {
        roll_number = " " + roll_number;
    }
    
    // Combine label + padded number
    std::string full_text = std::string(streak_label) + roll_number;
    
    // Center the entire string
    // Size 1 font: 6px per char
    int text_width = full_text.length() * 6;
    int centered_x = (128 - text_width) / 2;
    
    ssd1306_draw_string(&m_display, centered_x, 24, 1, full_text.c_str());

    uint32_t current_time = to_ms_since_boot(get_absolute_time());

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
    // current_time already declared above for roll rate calculation

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
        show_left_right_arrows = true;  // Changed from show_up_down_arrows
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

    // Draw title at top-left
    ssd1306_draw_string(&m_display, 0, 0, 1, descriptor_it->second.name.c_str());

    // Draw navigation arrows at top-right (only for Menu/Selection/Toggle types with multiple items)
    switch (descriptor_it->second.type) {
    case Utils::Menu::Descriptor::Type::Menu:
    case Utils::Menu::Descriptor::Type::Selection:
    case Utils::Menu::Descriptor::Type::Toggle: {
        auto selection_count = descriptor_it->second.items.size();
        
        if (selection_count > 1) {
            // Left arrow: ◄ (show if not first item)
            if (m_menu_state.selected_value > 0) {
                drawLeftArrow(&m_display, 110, 2);
            }
            
            // Right arrow: ► (show if not last item)
            if (m_menu_state.selected_value < selection_count - 1) {
                drawRightArrow(&m_display, 121, 2);
            }
        }
    } break;
    case Utils::Menu::Descriptor::Type::RebootInfo:
        break;
    case Utils::Menu::Descriptor::Type::RebootCountdown:
        showRebootCountdown();
        return;
    case Utils::Menu::Descriptor::Type::Value:
        break;
    }

    // Draw separator line below header
    ssd1306_draw_line(&m_display, 0, 10, 127, 10);

    // VALUE SLIDER SCREEN (unchanged from original)
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
        // MENU/SELECTION/TOGGLE/REBOOTINFO SCREENS
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
                    selection = "Yes (No)";
                } else {
                    selection = "(Yes) No";
                }
            } else {
                // Default Off/On for other toggles
                if (m_menu_state.selected_value == 0) {
                    selection = "(Off) On";
                } else {
                    selection = "Off (On)";
                }
            }
            break;
        case Utils::Menu::Descriptor::Type::Value:
            break;
        }
        
        // Special handling for RebootInfo pages with multiple lines (size 1 font)
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
            
            // Draw lines centered vertically and horizontally (size 1 font for multi-line info)
            int line_height = 8;  // Height per line
            int total_height = lines.size() * line_height;
            int start_y = (64 - 10 - total_height) / 2 + 10;  // Center in remaining space below header
            
            for (size_t i = 0; i < lines.size(); i++) {
                int text_x = (128 - (lines[i].length() * 6)) / 2;
                int text_y = start_y + (i * line_height);
                ssd1306_draw_string(&m_display, text_x, text_y, 1, lines[i].c_str());
            }
        } else {
            // SMART FONT SIZING: Use size 2 if text fits, fallback to size 1 if too long
            size_t newline_pos = selection.find('\n');
            if (newline_pos != std::string::npos) {
                // Two-line item
                std::string line1 = selection.substr(0, newline_pos);
                std::string line2 = selection.substr(newline_pos + 1);
                
                // Check if both lines fit in size 2 font (max 10 chars per line)
                if (line1.length() <= 10 && line2.length() <= 10) {
                    // Size 2 font: 12x16 pixels, y=20 and y=36
                    int line1_x = (128 - (line1.length() * 12)) / 2;
                    int line2_x = (128 - (line2.length() * 12)) / 2;
                    ssd1306_draw_string(&m_display, line1_x, 20, 2, line1.c_str());
                    ssd1306_draw_string(&m_display, line2_x, 36, 2, line2.c_str());
                } else {
                    // Size 1 font fallback: 6x8 pixels, y=24 and y=32
                    int line1_x = (128 - (line1.length() * 6)) / 2;
                    int line2_x = (128 - (line2.length() * 6)) / 2;
                    ssd1306_draw_string(&m_display, line1_x, 24, 1, line1.c_str());
                    ssd1306_draw_string(&m_display, line2_x, 32, 1, line2.c_str());
                }
            } else {
                // Single-line item
                // Special case: DrumBigHitArcade on current mode - show as two lines with "(Current)"
                if (m_menu_state.page == Utils::Menu::Page::DrumBigHitArcade &&
                    m_menu_state.selected_value == m_menu_state.original_value) {
                    // Draw item text at y=20 and "(Current)" at y=36
                    int text_x = (128 - (selection.length() * 12)) / 2;
                    ssd1306_draw_string(&m_display, text_x, 20, 2, selection.c_str());
                    
                    const char* current_text = "(Current)";
                    int current_x = (128 - (strlen(current_text) * 12)) / 2;
                    ssd1306_draw_string(&m_display, current_x, 36, 2, current_text);
                } else {
                    // Normal single-line item
                    if (selection.length() <= 10) {
                        // Size 2 font: 12x16 pixels, y=28
                        int text_x = (128 - (selection.length() * 12)) / 2;
                        ssd1306_draw_string(&m_display, text_x, 28, 2, selection.c_str());
                    } else {
                        // Size 1 font fallback: 6x8 pixels, y=28
                        int text_x = (128 - (selection.length() * 6)) / 2;
                        ssd1306_draw_string(&m_display, text_x, 28, 1, selection.c_str());
                    }
                }
            }
        }
    }
    
    // Draw navigation bar at bottom (if not RebootInfo/RebootCountdown)
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

// ============================================================================
// TAIKO TANTRUM CALIBRATION SCREENS
// ============================================================================

void Display::drawTantrumCountdownScreen() {
    if (!m_drum) return;

    const auto& tantrum_state = m_drum->getTantrumState();

    // Auto-transition to recording when countdown expires
    if (tantrum_state.current_mode == Drum::TantrumState::Mode::Recording) {
        m_state = State::TantrumRecording;
        return;
    }

    // Title
    const char* header = "Tantrum Cal";
    int header_x = (128 - strlen(header) * 12) / 2;
    ssd1306_draw_string(&m_display, header_x, 4, 2, header);

    ssd1306_draw_line(&m_display, 0, 22, 127, 22);

    // Countdown number (large)
    uint32_t seconds = tantrum_state.getSecondsRemaining();
    std::string countdown_str = std::to_string(seconds);
    int countdown_x = (128 - countdown_str.length() * 12) / 2;
    ssd1306_draw_string(&m_display, countdown_x, 28, 2, countdown_str.c_str());

    // Warning message
    const char* warning = "DON'T HIT YET";
    int warning_x = (128 - strlen(warning) * 6) / 2;
    ssd1306_draw_string(&m_display, warning_x, 48, 1, warning);
}

void Display::drawTantrumRecordingScreen() {
    if (!m_drum) return;

    const auto& tantrum_state = m_drum->getTantrumState();

    // Auto-transition when recording finishes
    if (tantrum_state.current_mode == Drum::TantrumState::Mode::ShowingResults) {
        showTantrumResults();
        return;
    }
    if (tantrum_state.current_mode == Drum::TantrumState::Mode::NeedsRedo) {
        showTantrumNeedsRedo();
        return;
    }

    // Title
    const char* header = "Recording...";
    int header_x = (128 - strlen(header) * 6) / 2;
    ssd1306_draw_string(&m_display, header_x, 0, 1, header.c_str());

    ssd1306_draw_line(&m_display, 0, 10, 127, 10);

    // Instructions (size 1 font, centered)
    const char* line1 = "Hit all pads as";
    int line1_x = (128 - strlen(line1) * 6) / 2;
    ssd1306_draw_string(&m_display, line1_x, 14, 1, line1);

    const char* line2 = "hard as in-game, as";
    int line2_x = (128 - strlen(line2) * 6) / 2;
    ssd1306_draw_string(&m_display, line2_x, 22, 1, line2);

    const char* line3 = "fast as possible,";
    int line3_x = (128 - strlen(line3) * 6) / 2;
    ssd1306_draw_string(&m_display, line3_x, 30, 1, line3);

    const char* line4 = "random patterns";
    int line4_x = (128 - strlen(line4) * 6) / 2;
    ssd1306_draw_string(&m_display, line4_x, 38, 1, line4);

    // Progress bar
    const int BAR_X = 10;
    const int BAR_Y = 48;
    const int BAR_WIDTH = 88;
    const int BAR_HEIGHT = 8;

    ssd1306_draw_line(&m_display, BAR_X, BAR_Y, BAR_X + BAR_WIDTH, BAR_Y);
    ssd1306_draw_line(&m_display, BAR_X, BAR_Y + BAR_HEIGHT, BAR_X + BAR_WIDTH, BAR_Y + BAR_HEIGHT);
    ssd1306_draw_line(&m_display, BAR_X, BAR_Y, BAR_X, BAR_Y + BAR_HEIGHT);
    ssd1306_draw_line(&m_display, BAR_X + BAR_WIDTH, BAR_Y, BAR_X + BAR_WIDTH, BAR_Y + BAR_HEIGHT);

    float progress = tantrum_state.getProgress();
    int fill_width = (int)(progress * (BAR_WIDTH - 4));

    if (fill_width > 0) {
        for (int y = BAR_Y + 2; y < BAR_Y + BAR_HEIGHT - 1; y++) {
            for (int x = BAR_X + 2; x < BAR_X + 2 + fill_width; x++) {
                ssd1306_draw_pixel(&m_display, x, y);
            }
        }
    }

    // Time remaining
    uint32_t seconds = tantrum_state.getSecondsRemaining();
    std::string time_str = std::to_string(seconds) + "s";
    ssd1306_draw_string(&m_display, BAR_X + BAR_WIDTH + 4, BAR_Y, 1, time_str.c_str());
}

void Display::drawTantrumResultsScreen() {
    if (!m_drum) return;

    const auto& tantrum_state = m_drum->getTantrumState();

    // Auto-transition after 5 seconds
    uint32_t elapsed = to_ms_since_boot(get_absolute_time()) - m_tantrum_results_start;
    if (elapsed >= TANTRUM_RESULTS_DISPLAY_MS) {
        m_state = State::Menu;
        return;
    }

    // Title
    const char* header = "Complete!";
    int header_x = (128 - strlen(header) * 12) / 2;
    ssd1306_draw_string(&m_display, header_x, 4, 2, header);

    ssd1306_draw_line(&m_display, 0, 22, 127, 22);

    // Show calculated thresholds (size 1 font)
    const auto& thresholds = tantrum_state.recommended_thresholds;

    std::string line1 = "DL:" + std::to_string(thresholds.at(Drum::Id::DON_LEFT)) +
                        " DR:" + std::to_string(thresholds.at(Drum::Id::DON_RIGHT));
    int line1_x = (128 - line1.length() * 6) / 2;
    ssd1306_draw_string(&m_display, line1_x, 26, 1, line1.c_str());

    std::string line2 = "KL:" + std::to_string(thresholds.at(Drum::Id::KA_LEFT)) +
                        " KR:" + std::to_string(thresholds.at(Drum::Id::KA_RIGHT));
    int line2_x = (128 - line2.length() * 6) / 2;
    ssd1306_draw_string(&m_display, line2_x, 36, 1, line2.c_str());

    // Crosstalk warning if needed
    if (tantrum_state.high_crosstalk_warning) {
        const char* warning1 = "High crosstalk!";
        int warning1_x = (128 - strlen(warning1) * 6) / 2;
        ssd1306_draw_string(&m_display, warning1_x, 46, 1, warning1);

        const char* warning2 = "Check pad mounting";
        int warning2_x = (128 - strlen(warning2) * 6) / 2;
        ssd1306_draw_string(&m_display, warning2_x, 54, 1, warning2);
    } else {
        const char* success = "Thresholds applied!";
        int success_x = (128 - strlen(success) * 6) / 2;
        ssd1306_draw_string(&m_display, success_x, 50, 1, success);
    }
}

void Display::drawTantrumNeedsRedoScreen() {
    if (!m_drum) return;

    const auto& tantrum_state = m_drum->getTantrumState();

    // Auto-transition after 5 seconds
    uint32_t elapsed = to_ms_since_boot(get_absolute_time()) - m_tantrum_needsredo_start;
    if (elapsed >= TANTRUM_NEEDSREDO_DISPLAY_MS) {
        m_state = State::Menu;
        return;
    }

    // Title
    const char* header = "Redo Needed";
    int header_x = (128 - strlen(header) * 12) / 2;
    ssd1306_draw_string(&m_display, header_x, 8, 2, header);

    ssd1306_draw_line(&m_display, 0, 26, 127, 26);

    // Show error message
    std::string error_msg = tantrum_state.redo_reason;

    // Split error message by newlines
    size_t newline_pos = error_msg.find('\n');
    if (newline_pos != std::string::npos) {
        std::string line1 = error_msg.substr(0, newline_pos);
        std::string line2 = error_msg.substr(newline_pos + 1);

        int line1_x = (128 - line1.length() * 6) / 2;
        ssd1306_draw_string(&m_display, line1_x, 32, 1, line1.c_str());

        int line2_x = (128 - line2.length() * 6) / 2;
        ssd1306_draw_string(&m_display, line2_x, 42, 1, line2.c_str());
    } else {
        int msg_x = (128 - error_msg.length() * 6) / 2;
        ssd1306_draw_string(&m_display, msg_x, 36, 1, error_msg.c_str());
    }

    const char* retry = "Try again";
    int retry_x = (128 - strlen(retry) * 6) / 2;
    ssd1306_draw_string(&m_display, retry_x, 54, 1, retry);
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
            // Set radius to 10 (smaller to fit under Streak label)
            m_rings[drum_index][i].radius = 10; 
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
    case State::TantrumCountdown:
        drawTantrumCountdownScreen();
        break;
    case State::TantrumRecording:
        drawTantrumRecordingScreen();
        break;
    case State::TantrumResults:
        drawTantrumResultsScreen();
        break;
    case State::TantrumNeedsRedo:
        drawTantrumNeedsRedoScreen();
        break;
    case State::RebootCountdown:
        drawRebootCountdown();
        break;
    }

    ssd1306_show(&m_display);
}

} // namespace OuchiTaiko::Peripherals

// End of file Display.cpp