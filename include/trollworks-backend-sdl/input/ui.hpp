#pragma once

#include <trollworks.hpp>

#define TW_SDL_UI_CAPTURE_MOUSE_KEY "ui_capture_mouse"_hs
#define TW_SDL_UI_CAPTURE_KEYBOARD_KEY "ui_capture_keyboard"_hs

namespace tw::sdl::input {
  bool ui_captured_mouse(entt::registry& registry);
  bool ui_captured_keyboard(entt::registry& registry);
}
