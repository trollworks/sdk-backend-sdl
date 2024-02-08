#include "../../include/trollworks-backend-sdl/input/manager.hpp"

namespace tw::sdl::input {
  manager& manager::main() {
    if (!entt::locator<manager>::has_value()) {
      entt::locator<manager>::emplace();
    }

    return entt::locator<manager>::value();
  }

  void manager::configure(std::function<void(controls &)> fn) {
    fn(m_controls);
  }

  void manager::process_event(SDL_Event& event) {
    switch (event.type) {
      case SDL_MOUSEWHEEL:
        m_state.mouse_wheel_x = event.wheel.x;
        m_state.mouse_wheel_y = event.wheel.y;
        break;

      case SDL_MOUSEMOTION:
        m_state.mouse_motion_x = event.motion.xrel;
        m_state.mouse_motion_y = event.motion.yrel;
        break;

      default:
        break;
    }
  }

  void manager::frame_begin() {
    m_state.mouse_wheel_x = 0;
    m_state.mouse_wheel_y = 0;
    m_state.mouse_motion_x = 0;
    m_state.mouse_motion_y = 0;
  }

  void manager::update(entt::registry &registry) {
    m_state.keyboard_state = SDL_GetKeyboardState(nullptr);
    m_state.mouse_button_mask = SDL_GetMouseState(&m_state.mouse_x, &m_state.mouse_y);

    for (auto &[_, action_context] : m_controls.m_triggers) {
      bool was_active = action_context.value.active;
      action_context.value.active = action_context.bindings->check(registry, m_state);
      action_context.value.performed = action_context.value.active && !was_active;
      action_context.value.cancelled = !action_context.value.active && was_active;
    }

    for (auto &[_, action_context] : m_controls.m_axis) {
      action_context.value.x = 0;
      action_context.value.y = 0;

      if (action_context.left_bindings->check(registry, m_state)) {
        action_context.value.x -= 1;
      }

      if (action_context.right_bindings->check(registry, m_state)) {
        action_context.value.x += 1;
      }

      if (action_context.up_bindings->check(registry, m_state)) {
        action_context.value.y -= 1;
      }

      if (action_context.down_bindings->check(registry, m_state)) {
        action_context.value.y += 1;
      }
    }

    for (auto &[_, action_context] : m_controls.m_passthrough_point) {
      passthrough_source<SDL_Point>::read(
        action_context.source,
        registry,
        m_state,
        action_context.value.raw
      );
    }
  }

  const action_trigger &manager::read_trigger(const std::string &name) {
    return m_controls.m_triggers.at(name).value;
  }

  const action_axis &manager::read_axis(const std::string &name) {
    return m_controls.m_axis.at(name).value;
  }

  const action_passthrough<SDL_Point> &manager::read_passthrough_point(const std::string &name) {
    return m_controls.m_passthrough_point.at(name).value;
  }
}
