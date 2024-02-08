#pragma once

#include "../SDL.hpp"

namespace tw::sdl::input {
  struct state {
    const Uint8* keyboard_state;

    Uint32 mouse_button_mask;
    int mouse_x;
    int mouse_y;
    int mouse_motion_x;
    int mouse_motion_y;
    int mouse_wheel_x;
    int mouse_wheel_y;
  };
}
