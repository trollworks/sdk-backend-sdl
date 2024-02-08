#pragma once

#include <string>
#include <functional>

#include <trollworks.hpp>
#include "../SDL.hpp"

#include "./state.hpp"
#include "./actions.hpp"
#include "./controls.hpp"

namespace tw::sdl::input {
  class manager {
    public:
      static manager& main();

      manager() = default;
      manager(const manager&) = delete;

    public:
      void configure(std::function<void(controls&)> fn);

      void process_event(SDL_Event& event);
      void frame_begin();
      void update(entt::registry& registry);

      const action_trigger& read_trigger(const std::string& name);
      const action_axis& read_axis(const std::string& name);
      const action_passthrough<SDL_Point>& read_passthrough_point(const std::string& name);

    private:
      controls m_controls;
      state m_state;
  };
}
