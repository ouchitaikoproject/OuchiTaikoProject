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

void Display::setCurrentThresholds(const Peripherals::Drum::Config::Thresholds &thresholds) {
    m_current_thresholds = thresholds;
}

void Display::showIdle() { m_state = State::Idle; }
void Display::showMenu() { m_state = State::Menu; }

void Display::showTantrumWelcome()    { m_state = State::TantrumWelcome; }
void Display::showTantrumPadHitting() { m_state = State::TantrumPadHitting; }
void Display::showTantrumPhaseTransition() { m_state = State::TantrumPhaseTransition; }
void Display::showTantrumPadRoll()    { m_state = State::TantrumPadRoll; }
void Display::showTantrumPadDone()    { m_state = State::TantrumPadDone; }
void Display::showTantrumOverview()   { m_state = State::TantrumOverview; }
void Display::showTantrumSaving()     { m_state = State::TantrumSaving; }
void Display::showTantrumComplete()   { m_state = State::TantrumComplete; }
void Display::showTantrumError()      { m_state = State::TantrumError; }

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

// ============================================
// NEW BOOT SPLASH - Classic Arcade Text Scroll
// ============================================

void Display::drawBootSplash() {
    uint32_t elapsed = to_ms_since_boot(get_absolute_time()) - m_boot_splash_start;
    
    // Animation timing
    const uint32_t PHASE1_INSERT_COIN = 3000;   // Show "INSERT COIN" blinking (3x longer)
    const uint32_t PHASE2_SCROLL = 1500;         // Scroll text in
    const uint32_t PHASE3_HOLD = 2500;           // Hold final position (1.5s longer)
    
    const uint32_t PHASE1_END = PHASE1_INSERT_COIN;
    const uint32_t PHASE2_END = PHASE1_END + PHASE2_SCROLL;
    const uint32_t PHASE3_END = PHASE2_END + PHASE3_HOLD;
    
    // ============================================
    // PHASE 1: "INSERT COIN" BLINKING - BIG STOCK FONT
    // ============================================
    if (elapsed < PHASE1_END) {
        // Blink every 250ms
        if ((elapsed / 250) % 2 == 0) {
            const char* text = "INSERT COIN";
            int text_width = strlen(text) * 6 * 2;
            ssd1306_draw_string(&m_display, (128 - text_width) / 2, 28, 2, text);
        }
    }
    
    // ============================================
    // PHASE 2: SCROLL TEXT IN FROM RIGHT
    // ============================================
    else if (elapsed < PHASE2_END) {
        uint32_t scroll_time = elapsed - PHASE1_END;
        float progress = (float)scroll_time / (float)PHASE2_SCROLL;
        
        // Smooth easing
        float eased = progress * progress * (3.0f - 2.0f * progress); // Smoothstep
        
        // Line 1: "KillerQ's" - scrolls from right
        const char* line1 = "KillerQ's";
        int line1_width = strlen(line1) * 6;  // Size 1 font
        int line1_start_x = 128;  // Start off-screen right
        int line1_end_x = (128 - line1_width) / 2;  // Center
        int line1_x = line1_start_x - (int)(eased * (line1_start_x - line1_end_x));
        ssd1306_draw_string(&m_display, line1_x, 12, 1, line1);
        
        // Line 2: "OuchiTaiko" - BIG font, scrolls from right (slight delay)
        float progress2 = progress > 0.2f ? (progress - 0.2f) / 0.8f : 0.0f;
        float eased2 = progress2 * progress2 * (3.0f - 2.0f * progress2);
        
        const char* line2 = "OuchiTaiko";
        int line2_width = strlen(line2) * 12;  // Size 2 font
        int line2_start_x = 128;
        int line2_end_x = (128 - line2_width) / 2;
        int line2_x = line2_start_x - (int)(eased2 * (line2_start_x - line2_end_x));
        ssd1306_draw_string(&m_display, line2_x, 26, 2, line2);
        
        // Line 3: "Project" - scrolls from right (more delay)
        float progress3 = progress > 0.4f ? (progress - 0.4f) / 0.6f : 0.0f;
        float eased3 = progress3 * progress3 * (3.0f - 2.0f * progress3);
        
        const char* line3 = "Project";
        int line3_width = strlen(line3) * 6;
        int line3_start_x = 128;
        int line3_end_x = (128 - line3_width) / 2;
        int line3_x = line3_start_x - (int)(eased3 * (line3_start_x - line3_end_x));
        ssd1306_draw_string(&m_display, line3_x, 48, 1, line3);
    }
    
    // ============================================
    // PHASE 3: HOLD FINAL POSITION
    // ============================================
    else if (elapsed < PHASE3_END) {
        // Line 1: "KillerQ's"
        const char* line1 = "KillerQ's";
        int line1_width = strlen(line1) * 6;
        ssd1306_draw_string(&m_display, (128 - line1_width) / 2, 12, 1, line1);
        
        // Line 2: "OuchiTaiko" - BIG
        const char* line2 = "OuchiTaiko";
        int line2_width = strlen(line2) * 12;
        ssd1306_draw_string(&m_display, (128 - line2_width) / 2, 26, 2, line2);
        
        // Line 3: "Project"
        const char* line3 = "Project";
        int line3_width = strlen(line3) * 6;
        ssd1306_draw_string(&m_display, (128 - line3_width) / 2, 48, 1, line3);
    }
    // After PHASE3_END, continue showing the same screen until transition
}

void Display::drawIdleScreen() {
    std::string mode_string = modeToString(m_usb_mode);
    mode_string += " Mode";
    ssd1306_draw_string(&m_display, 0, 0, 1, mode_string.c_str());
    ssd1306_draw_line(&m_display, 0, 10, 128, 10);

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

    // Draw small center dots for each drum (3x3 centered)
    for (int i = 0; i < 4; i++) {
        uint8_t x = DRUM_CENTERS[i][0];
        uint8_t y = DRUM_CENTERS[i][1];
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
    
    // THEN draw expanding WHITE rings on top
    updateRings();
    drawRings();

    // Button tester - show first pressed button in top-right
    const char* button_label = nullptr;
    
    static uint32_t select_hold_time = 0;
    static uint32_t start_hold_time = 0;
    static bool select_was_pressed = false;
    static bool start_was_pressed = false;
    
    bool select_currently_pressed = m_input_state.controller.buttons.select;
    bool start_currently_pressed = m_input_state.controller.buttons.start;

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
    
    bool in_hold_state = (select_hold_time != 0 && (current_time - select_hold_time) > 300) ||
                         (start_hold_time != 0 && (current_time - start_hold_time) > 300);
    
    if (!in_hold_state) {
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
        int label_width = strlen(button_label) * 6;
        int centered_x = (128 - label_width) / 2;
        ssd1306_draw_string(&m_display, centered_x, 16, 1, button_label);
    }

    ssd1306_draw_line(&m_display, 0, 54, 128, 54);
    ssd1306_draw_string(&m_display, 0, 56, 1, "Hold SELECT for Menu");
}

void Display::drawNavigationBar(const Utils::Menu::Descriptor &descriptor) {
    ssd1306_draw_line(&m_display, 0, 54, 127, 54);
    
    std::string left_label = "Cancel";
    std::string right_label = "OK";
    bool show_left_right_arrows = false;
    bool show_up_down_arrows = false;
    
    bool hide_ok = false;
    
    switch (descriptor.type) {
    case Utils::Menu::Descriptor::Type::Menu:
    case Utils::Menu::Descriptor::Type::Selection:
    case Utils::Menu::Descriptor::Type::Toggle:
        show_left_right_arrows = true;
        break;
    case Utils::Menu::Descriptor::Type::Value:
        show_left_right_arrows = true;
        right_label = "Save";
        break;
    case Utils::Menu::Descriptor::Type::UnifiedThresholds:
        show_left_right_arrows = true;
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

   // Draw title at top-left (except for UnifiedThresholds which has custom header)
    if (descriptor_it->second.type != Utils::Menu::Descriptor::Type::UnifiedThresholds) {
        ssd1306_draw_string(&m_display, 0, 0, 1, descriptor_it->second.name.c_str());
    }

    // Draw navigation arrows at top-right
    switch (descriptor_it->second.type) {
    case Utils::Menu::Descriptor::Type::Menu:
    case Utils::Menu::Descriptor::Type::Selection:
    case Utils::Menu::Descriptor::Type::Toggle: {
        auto selection_count = descriptor_it->second.items.size();
        
        if (selection_count > 1) {
            if (m_menu_state.selected_value > 0) {
                drawLeftArrow(&m_display, 110, 2);
            }
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
    case Utils::Menu::Descriptor::Type::UnifiedThresholds:
        break;
    case Utils::Menu::Descriptor::Type::Value:
        break;
    }

    // Draw separator line below header
    ssd1306_draw_line(&m_display, 0, 10, 127, 10);

    // UNIFIED THRESHOLDS SCREEN (cleaned up: clear layout, no clutter)
    if (descriptor_it->second.type == Utils::Menu::Descriptor::Type::UnifiedThresholds && m_drum != nullptr) {
        auto thresholds = m_current_thresholds;

        // Title
        ssd1306_draw_string(&m_display, 0, 0, 1, "Manual Thresholds");
        ssd1306_draw_line(&m_display, 0, 9, 127, 9);

        // Compact help line
        ssd1306_draw_string(&m_display, 0, 11, 1, "UD:sel  LR:adj");

        ssd1306_draw_line(&m_display, 0, 20, 127, 20);

        // 4 thresholds in 2 rows x 2 cols
        // Row 0: KaL (sel 0) | KaR (sel 3)
        // Row 1: DonL(sel 1) | DonR(sel 2)
        char ka_left_str[12], ka_right_str[12], don_left_str[12], don_right_str[12];
        snprintf(ka_left_str,   sizeof(ka_left_str),   "KaL  %4u", thresholds.ka_left);
        snprintf(ka_right_str,  sizeof(ka_right_str),  "KaR  %4u", thresholds.ka_right);
        snprintf(don_left_str,  sizeof(don_left_str),  "DonL %4u", thresholds.don_left);
        snprintf(don_right_str, sizeof(don_right_str), "DonR %4u", thresholds.don_right);

        // Draw selection marker (>) for selected row
        uint8_t sel = (uint8_t)m_menu_state.selected_value;

        // Row y=22: KaL | KaR
        if (sel == 0) ssd1306_draw_string(&m_display, 0, 22, 1, ">");
        if (sel == 3) ssd1306_draw_string(&m_display, 64, 22, 1, ">");
        ssd1306_draw_string(&m_display, 8,  22, 1, ka_left_str);
        ssd1306_draw_string(&m_display, 72, 22, 1, ka_right_str);

        // Row y=31: DonL | DonR
        if (sel == 1) ssd1306_draw_string(&m_display, 0, 31, 1, ">");
        if (sel == 2) ssd1306_draw_string(&m_display, 64, 31, 1, ">");
        ssd1306_draw_string(&m_display, 8,  31, 1, don_left_str);
        ssd1306_draw_string(&m_display, 72, 31, 1, don_right_str);

        ssd1306_draw_line(&m_display, 0, 41, 127, 41);

        // Live drum animation (compact, y center = 48)
        bool ka_left_hit  = m_input_state.drum.ka_left.triggered   && !m_last_ka_left;
        bool don_left_hit = m_input_state.drum.don_left.triggered  && !m_last_don_left;
        bool don_right_hit= m_input_state.drum.don_right.triggered && !m_last_don_right;
        bool ka_right_hit = m_input_state.drum.ka_right.triggered  && !m_last_ka_right;
        if (ka_left_hit)   activateRing(0);
        if (don_left_hit)  activateRing(1);
        if (don_right_hit) activateRing(2);
        if (ka_right_hit)  activateRing(3);
        m_last_ka_left   = m_input_state.drum.ka_left.triggered;
        m_last_don_left  = m_input_state.drum.don_left.triggered;
        m_last_don_right = m_input_state.drum.don_right.triggered;
        m_last_ka_right  = m_input_state.drum.ka_right.triggered;
        updateRings(); drawRings();
        for (int i = 0; i < 4; i++) {
            uint8_t x = DRUM_CENTERS[i][0], y = DRUM_CENTERS[i][1];
            for (int dy=-1;dy<=1;dy++) for (int dx=-1;dx<=1;dx++) ssd1306_draw_pixel(&m_display,x+dx,y+dy);
        }

        // Footer
        ssd1306_draw_line(&m_display, 0, 54, 127, 54);
        ssd1306_draw_string(&m_display, 14, 56, 1, "B:Cancel  A:Save");

        return;
    }

    // VALUE SLIDER SCREEN
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
            if (m_menu_state.page == Utils::Menu::Page::Bootsel || 
                m_menu_state.page == Utils::Menu::Page::Reset ||
                m_menu_state.page == Utils::Menu::Page::DrumTriggerThresholdsReset) {
                if (m_menu_state.selected_value == 0) {
                    selection = "Yes (No)";
                } else {
                    selection = "(Yes) No";
                }
            } else {
                if (m_menu_state.selected_value == 0) {
                    selection = "(Off) On";
                } else {
                    selection = "Off (On)";
                }
            }
            break;
        case Utils::Menu::Descriptor::Type::UnifiedThresholds:
            break;
        case Utils::Menu::Descriptor::Type::Value:
            break;
        }

        // Special handling for RebootInfo pages with multiple lines
        if (descriptor_it->second.type == Utils::Menu::Descriptor::Type::RebootInfo) {
            std::vector<std::string> lines;
            size_t start = 0;
            size_t newline_pos;
            
            while ((newline_pos = selection.find('\n', start)) != std::string::npos) {
                lines.push_back(selection.substr(start, newline_pos - start));
                start = newline_pos + 1;
            }
            lines.push_back(selection.substr(start));
            
            int line_height = 8;
            int total_height = lines.size() * line_height;
            int start_y = (64 - 10 - total_height) / 2 + 10;
            
            for (size_t i = 0; i < lines.size(); i++) {
                int text_x = (128 - (lines[i].length() * 6)) / 2;
                int text_y = start_y + (i * line_height);
                ssd1306_draw_string(&m_display, text_x, text_y, 1, lines[i].c_str());
            }
        } else {
            size_t newline_pos = selection.find('\n');
            if (newline_pos != std::string::npos) {
                std::string line1 = selection.substr(0, newline_pos);
                std::string line2 = selection.substr(newline_pos + 1);
                
                if (line1.length() <= 10 && line2.length() <= 10) {
                    int line1_x = (128 - (line1.length() * 12)) / 2;
                    int line2_x = (128 - (line2.length() * 12)) / 2;
                    ssd1306_draw_string(&m_display, line1_x, 20, 2, line1.c_str());
                    ssd1306_draw_string(&m_display, line2_x, 36, 2, line2.c_str());
                } else {
                    int line1_x = (128 - (line1.length() * 6)) / 2;
                    int line2_x = (128 - (line2.length() * 6)) / 2;
                    ssd1306_draw_string(&m_display, line1_x, 24, 1, line1.c_str());
                    ssd1306_draw_string(&m_display, line2_x, 32, 1, line2.c_str());
                }
            } else {
                if (false) {
                    int text_x = (128 - (selection.length() * 12)) / 2;
                    ssd1306_draw_string(&m_display, text_x, 20, 2, selection.c_str());
                    
                    const char* current_text = "(Current)";
                    int current_x = (128 - (strlen(current_text) * 12)) / 2;
                    ssd1306_draw_string(&m_display, current_x, 36, 2, current_text);
                } else {
                    if (selection.length() <= 10) {
                        int text_x = (128 - (selection.length() * 12)) / 2;
                        ssd1306_draw_string(&m_display, text_x, 28, 2, selection.c_str());
                    } else {
                        int text_x = (128 - (selection.length() * 6)) / 2;
                        ssd1306_draw_string(&m_display, text_x, 28, 1, selection.c_str());
                    }
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
    
    bool is_bootloader = (m_menu_state.page == Utils::Menu::Page::BootselMsg);
    
    if (is_bootloader) {
        uint8_t countdown = 5;
        if (elapsed < REBOOT_COUNTDOWN_MS) {
            countdown = 5 - (elapsed / 1000);
        } else {
            countdown = 0;
        }
        
        const char* line1 = "Entering Bootloader";
        int line1_x = (128 - strlen(line1) * 6) / 2;
        ssd1306_draw_string(&m_display, line1_x, 4, 1, line1);
        
        std::string countdown_text = "Starting in " + std::to_string(countdown) + "...";
        int line2_x = (128 - countdown_text.length() * 6) / 2;
        ssd1306_draw_string(&m_display, line2_x, 16, 1, countdown_text.c_str());
        
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
// GUIDED CALIBRATION WIZARD - Display functions
// ============================================================================

// drawFillRect: unused currently but kept for future screens
// static void drawFillRect(ssd1306_t *d, int x, int y, int w, int h) {
//     for (int row = y; row < y + h; row++)
//         for (int col = x; col < x + w; col++)
//             ssd1306_draw_pixel(d, col, row);
// }

void Display::drawTantrumWelcomeScreen() {
    const char* hdr = "AUTO CALIBRATE";
    ssd1306_draw_string(&m_display, (128 - (int)strlen(hdr)*6)/2, 0, 1, hdr);
    ssd1306_draw_line(&m_display, 0, 10, 127, 10);

    ssd1306_draw_string(&m_display, 4, 14, 1, "Use one hand only.");
    ssd1306_draw_string(&m_display, 4, 23, 1, "Use game-like force.");

    ssd1306_draw_line(&m_display, 0, 34, 127, 34);
    ssd1306_draw_string(&m_display, 4, 37, 1, "3 phases per pad:");
    ssd1306_draw_string(&m_display, 4, 46, 1, "normal, hard, rapid");

    const char* begin = "B: Cancel  A: Begin";
    ssd1306_draw_string(&m_display, (128 - (int)strlen(begin)*6)/2, 56, 1, begin);
}

// Phase dots removed -- each phase now has its own clear standalone screen

void Display::drawTantrumPadHittingScreen() {
    if (!m_drum) return;
    const auto& s = m_drum->getTantrumState();
    bool is_hard = (s.current_mode == Peripherals::Drum::TantrumState::Mode::PadHard);

    // Header: pad name + pad number
    char hdr[24];
    snprintf(hdr, sizeof(hdr), "%s (%u/4)", s.currentPadName(), (unsigned)(s.current_pad + 1));
    int hw = (int)strlen(hdr) * 6;
    ssd1306_draw_string(&m_display, (128 - hw)/2, 0, 1, hdr);
    ssd1306_draw_line(&m_display, 0, 9, 127, 9);

    // Phase label (large, centered)
    const char* phase = is_hard ? "STRONG" : "NORMAL";
    int pw = (int)strlen(phase) * 12;
    ssd1306_draw_string(&m_display, (128 - pw)/2, 12, 2, phase);

    ssd1306_draw_line(&m_display, 0, 30, 127, 30);

    // Hit counter (large, centered)
    uint8_t done = s.hit_count > 3 ? 3 : s.hit_count;
    char hits[8];
    snprintf(hits, sizeof(hits), "%u / 3", (unsigned)done);
    int hcw = (int)strlen(hits) * 12;
    ssd1306_draw_string(&m_display, (128 - hcw)/2, 34, 2, hits);

    ssd1306_draw_line(&m_display, 0, 54, 127, 54);
    // Bottom instruction
    const char* instr = is_hard ? "One hand hardest hit" : "One hand normal hit";
    int iw = (int)strlen(instr) * 6;
    ssd1306_draw_string(&m_display, (128 - iw)/2, 57, 1, instr);
}

void Display::drawTantrumPadRollScreen() {
    if (!m_drum) return;
    const auto& s = m_drum->getTantrumState();

    // Header
    char hdr[24];
    snprintf(hdr, sizeof(hdr), "%s (%u/4)", s.currentPadName(), (unsigned)(s.current_pad + 1));
    int hw = (int)strlen(hdr) * 6;
    ssd1306_draw_string(&m_display, (128 - hw)/2, 0, 1, hdr);
    ssd1306_draw_line(&m_display, 0, 9, 127, 9);

    // Phase label
    const char* phase = "RAPID";
    int pw = (int)strlen(phase) * 12;
    ssd1306_draw_string(&m_display, (128 - pw)/2, 12, 2, phase);
    ssd1306_draw_line(&m_display, 0, 30, 127, 30);

    // Progress bar
    float pct = s.getRollProgress();
    const int BAR_X = 6, BAR_Y = 38, BAR_W = 104, BAR_H = 9;
    ssd1306_draw_line(&m_display, BAR_X, BAR_Y, BAR_X+BAR_W, BAR_Y);
    ssd1306_draw_line(&m_display, BAR_X, BAR_Y+BAR_H, BAR_X+BAR_W, BAR_Y+BAR_H);
    ssd1306_draw_line(&m_display, BAR_X, BAR_Y, BAR_X, BAR_Y+BAR_H);
    ssd1306_draw_line(&m_display, BAR_X+BAR_W, BAR_Y, BAR_X+BAR_W, BAR_Y+BAR_H);
    int fill = (int)(pct * (BAR_W - 4));
    for (int y = BAR_Y+2; y < BAR_Y+BAR_H-1; y++)
        for (int x = BAR_X+2; x < BAR_X+2+fill; x++)
            ssd1306_draw_pixel(&m_display, x, y);

    // Timer
    char ts[12];
    snprintf(ts, sizeof(ts), "%u", (unsigned)s.getRollSecondsRemaining());
    ssd1306_draw_string(&m_display, 114, BAR_Y+1, 1, ts);

    // Instruction
    const char* instr = s.roll_started ? "One hand keep going" : "One hand rapid start";
    int iw = (int)strlen(instr) * 6;
    ssd1306_draw_string(&m_display, (128 - iw)/2, 56, 1, instr);
}

void Display::drawTantrumPhaseTransitionScreen() {
    if (!m_drum) return;
    const auto& s = m_drum->getTantrumState();

    // Big centered checkmark-style "Done"
    const char* done = "Phase Done!";
    int dw = (int)strlen(done) * 6;
    ssd1306_draw_string(&m_display, (128 - dw)/2, 8, 1, done);
    ssd1306_draw_line(&m_display, 0, 18, 127, 18);

    // What's coming next
    const char* next_hdr = "Starting next:";
    int nhw = (int)strlen(next_hdr) * 6;
    ssd1306_draw_string(&m_display, (128 - nhw)/2, 24, 1, next_hdr);

    const char* next_label = s.transition_next_label ? s.transition_next_label : "";
    int nlw = (int)strlen(next_label) * 12;
    ssd1306_draw_string(&m_display, (128 - nlw)/2, 33, 2, next_label);

    ssd1306_draw_line(&m_display, 0, 54, 127, 54);
    // Simple countdown indicator
    uint32_t elapsed = to_ms_since_boot(get_absolute_time()) - s.phase_start;
    uint32_t remaining_ms = (elapsed < 2000) ? (2000 - elapsed) : 0;
    char ts[16];
    snprintf(ts, sizeof(ts), "(%us)...", (unsigned)((remaining_ms + 999) / 1000));
    int tw = (int)strlen(ts) * 6;
    ssd1306_draw_string(&m_display, (128 - tw)/2, 57, 1, ts);
}

void Display::drawTantrumPadDoneScreen() {
    if (!m_drum) return;
    const auto& s = m_drum->getTantrumState();

    uint8_t padIdx = s.currentPadIndex();
    uint16_t thr = s.recommended_thresholds[padIdx];

    // Pad name (large)
    const char* name = s.currentPadName();
    int nw = (int)strlen(name) * 12;
    ssd1306_draw_string(&m_display, (128 - nw)/2, 2, 2, name);
    ssd1306_draw_line(&m_display, 0, 19, 127, 19);

    const char* done_str = "Calibrated!";
    int dw = (int)strlen(done_str) * 6;
    ssd1306_draw_string(&m_display, (128 - dw)/2, 24, 1, done_str);

    char tstr[8];
    snprintf(tstr, sizeof(tstr), "%u", (unsigned)thr);
    int tw = (int)strlen(tstr) * 12;
    ssd1306_draw_string(&m_display, (128 - tw)/2, 33, 2, tstr);

    ssd1306_draw_line(&m_display, 0, 52, 127, 52);
    const char* next = s.current_pad < 3 ? "Next pad loading..." : "Finalizing results...";
    int nextw = (int)strlen(next) * 6;
    ssd1306_draw_string(&m_display, (128 - nextw)/2, 55, 1, next);
}

void Display::drawTantrumOverviewScreen() {
    if (!m_drum) return;
    const auto& s = m_drum->getTantrumState();

    const char* title = "Review Results";
    int tw = (int)strlen(title) * 6;
    ssd1306_draw_string(&m_display, (128 - tw)/2, 0, 1, title);
    ssd1306_draw_line(&m_display, 0, 9, 127, 9);

    // 2x2 threshold grid
    char dl[14], dr[14], kl[14], kr[14];
    snprintf(dl, sizeof(dl), "DonL: %u", (unsigned)s.recommended_thresholds[0]);
    snprintf(dr, sizeof(dr), "DonR: %u", (unsigned)s.recommended_thresholds[2]);
    snprintf(kl, sizeof(kl), "KaL:  %u", (unsigned)s.recommended_thresholds[1]);
    snprintf(kr, sizeof(kr), "KaR:  %u", (unsigned)s.recommended_thresholds[3]);
    ssd1306_draw_string(&m_display, 2,  12, 1, dl);
    ssd1306_draw_string(&m_display, 66, 12, 1, dr);
    ssd1306_draw_string(&m_display, 2,  21, 1, kl);
    ssd1306_draw_string(&m_display, 66, 21, 1, kr);
    ssd1306_draw_line(&m_display, 0, 31, 127, 31);

    // Keep final review screen clean and static for readability.
    const char* hint = s.high_crosstalk_warning ? "High XTALK-check isolate" : "Review before apply";
    int hw = (int)strlen(hint) * 6;
    ssd1306_draw_string(&m_display, (128 - hw)/2, 33, 1, hint);

    ssd1306_draw_line(&m_display, 0, 54, 127, 54);
    const char* foot = "Applying... B:Cancel";
    int fw = (int)strlen(foot) * 6;
    ssd1306_draw_string(&m_display, (128 - fw)/2, 56, 1, foot);
}

void Display::drawTantrumSavingScreen() {
    const char* line1 = "Saving...";
    int l1w = (int)strlen(line1) * 12;
    ssd1306_draw_string(&m_display, (128 - l1w)/2, 12, 2, line1);
    ssd1306_draw_line(&m_display, 0, 32, 127, 32);
    const char* line2 = "Writing to flash";
    int l2w = (int)strlen(line2) * 6;
    ssd1306_draw_string(&m_display, (128 - l2w)/2, 38, 1, line2);
}

void Display::drawTantrumCompleteScreen() {
    const char* line1 = "DONE!";
    int l1w = (int)strlen(line1) * 12;
    ssd1306_draw_string(&m_display, (128 - l1w)/2, 4, 2, line1);
    ssd1306_draw_line(&m_display, 0, 22, 127, 22);
    const char* line2 = "Thresholds saved.";
    int l2w = (int)strlen(line2) * 6;
    ssd1306_draw_string(&m_display, (128 - l2w)/2, 27, 1, line2);
    const char* line3 = "Play mode resumed.";
    int l3w = (int)strlen(line3) * 6;
    ssd1306_draw_string(&m_display, (128 - l3w)/2, 36, 1, line3);
    const char* line4 = "Calibration complete";
    int l4w = (int)strlen(line4) * 6;
    ssd1306_draw_string(&m_display, (128 - l4w)/2, 45, 1, line4);
}

void Display::drawTantrumErrorScreen() {
    if (!m_drum) return;
    const auto& s = m_drum->getTantrumState();
    const char* title = "Redo";
    int titw = (int)strlen(title) * 12;
    ssd1306_draw_string(&m_display, (128 - titw)/2, 4, 2, title);
    ssd1306_draw_line(&m_display, 0, 22, 127, 22);
    const char* msg = s.error_msg ? s.error_msg : "Try again";
    int mw = (int)strlen(msg) * 6;
    ssd1306_draw_string(&m_display, (128 - mw)/2, 30, 1, msg);
    ssd1306_draw_line(&m_display, 0, 54, 127, 54);
    const char* foot = "B:Cancel  A:Retry";
    int fw = (int)strlen(foot) * 6;
    ssd1306_draw_string(&m_display, (128 - fw)/2, 56, 1, foot);
}

// ==========================================================
// CUSTOM BURST ANIMATION FUNCTIONS (Rings)
// ==========================================================

void Display::activateRing(uint8_t drum_index) {
    if (drum_index >= 4) return;
    
    for (int i = 0; i < MAX_RINGS_PER_DRUM; i++) {
        if (!m_rings[drum_index][i].active) {
            m_rings[drum_index][i].active = true;
            m_rings[drum_index][i].radius = 0; 
            break;
        }
    }
}

void Display::updateRings() {
    for (int drum = 0; drum < 4; drum++) {
        for (int ring = 0; ring < MAX_RINGS_PER_DRUM; ring++) {
            if (m_rings[drum][ring].active) {
                if (m_rings[drum][ring].radius < 14) {
                    m_rings[drum][ring].radius += 1;
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

void Display::drawCircleRing(uint8_t x_center, uint8_t y_center, uint8_t radius) {
    if (radius <= 0) return;

    int x = radius;
    int y = 0;
    int err = 0;

    auto draw_pixel_safe = [&](int px, int py) {
        int draw_x = x_center + px;
        int draw_y = y_center + py;
        if (draw_x >= 0 && draw_x < 128 && draw_y >= 0 && draw_y < 64) {
            ssd1306_draw_pixel(&m_display, draw_x, draw_y);
        }
    };

    while (x >= y) {
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

void Display::drawCircleRingBlack(uint8_t x_center, uint8_t y_center, uint8_t radius) {
    if (radius <= 0) return;

    int x = radius;
    int y = 0;
    int err = 0;

    auto clear_pixel_safe = [&](int px, int py) {
        int draw_x = x_center + px;
        int draw_y = y_center + py;
        if (draw_x >= 0 && draw_x < 128 && draw_y >= 0 && draw_y < 64) {
            ssd1306_clear_pixel(&m_display, draw_x, draw_y);
        }
    };

    while (x >= y) {
        clear_pixel_safe(x, y);
        clear_pixel_safe(y, x);
        clear_pixel_safe(-y, x);
        clear_pixel_safe(-x, y);
        clear_pixel_safe(-x, -y);
        clear_pixel_safe(-y, -x);
        clear_pixel_safe(y, -x);
        clear_pixel_safe(x, -y);

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
    case State::TantrumWelcome:
        drawTantrumWelcomeScreen();
        break;
    case State::TantrumPadHitting:
        drawTantrumPadHittingScreen();
        break;
    case State::TantrumPhaseTransition:
        drawTantrumPhaseTransitionScreen();
        break;
    case State::TantrumPadRoll:
        drawTantrumPadRollScreen();
        break;
    case State::TantrumPadDone:
        drawTantrumPadDoneScreen();
        break;
    case State::TantrumOverview:
        drawTantrumOverviewScreen();
        break;
    case State::TantrumSaving:
        drawTantrumSavingScreen();
        break;
    case State::TantrumComplete:
        drawTantrumCompleteScreen();
        break;
    case State::TantrumError:
        drawTantrumErrorScreen();
        break;
    case State::RebootCountdown:
        drawRebootCountdown();
        break;
    }

    ssd1306_show(&m_display);
}

} // namespace OuchiTaiko::Peripherals

// End of file Display.cpp
