#include "../../include/trollworks-backend-sdl/input/ui.hpp"

using namespace entt::literals;

namespace tw::sdl::input {
  bool ui_captured_mouse(entt::registry& registry) {
    auto captured = registry.ctx().find<bool>(TW_SDL_UI_CAPTURE_MOUSE_KEY);
    return captured != nullptr ? *captured : false;
  }

  bool ui_captured_keyboard(entt::registry& registry) {
    auto captured = registry.ctx().find<bool>(TW_SDL_UI_CAPTURE_KEYBOARD_KEY);
    return captured != nullptr ? *captured : false;
  }
}
