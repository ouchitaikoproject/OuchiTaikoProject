#ifndef UTILS_INPUTSTATE_H_
#define UTILS_INPUTSTATE_H_

#include <cstdint>

namespace OuchiTaiko::Utils {

struct InputState {
  public:
    struct Drum {
        struct Pad {
            bool triggered;
            uint16_t analog;
            uint16_t raw;
        };

        struct DebugEvent {
            // Bit map follows pad order: [0]=don_l [1]=ka_l [2]=don_r [3]=ka_r
            uint8_t hit_mask;
            uint8_t cross_block_mask;
            uint8_t arb_block_mask;
            uint8_t held_high_mask;
            uint32_t timestamp_ms;
        };

        Pad don_left, ka_left, don_right, ka_right;
        uint16_t current_roll;
        uint16_t previous_roll;
        DebugEvent debug_event{};
    };

    struct Controller {
        struct DPad {
            bool up, down, left, right;
        };

        struct Buttons {
            bool north, east, south, west;
            bool l, r;
            bool start, select, home, share;
        };

        DPad dpad;
        Buttons buttons;
    };

    Drum drum{};
    Controller controller{};

    void releaseAll() {
        drum = {};
        controller = {};
    };
};

} // namespace OuchiTaiko::Utils

#endif // UTILS_INPUTSTATE_H_
