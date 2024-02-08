#pragma once

#include <unordered_map>
#include <string>

#include "./bindings.hpp"
#include "./passthrough.hpp"
#include "./actions.hpp"

namespace tw::sdl::input {
  /* forward */ class manager;

  class controls {
    friend class manager;

    public:
      controls() = default;
      controls(const controls&) = delete;

    public:
      struct trigger_context {
        action_trigger value;
        binding_type bindings;
      };

      struct axis_context {
        action_axis value;
        binding_type left_bindings;
        binding_type right_bindings;
        binding_type up_bindings;
        binding_type down_bindings;
      };

      template <typename T>
      struct passthrough_context {
        using source_type = typename passthrough_source<T>::type;

        action_passthrough<T> value;
        source_type source;
      };

    public:
      void add_trigger(const std::string& name, binding_type bindings);
      void add_axis(
        const std::string& name,
        binding_type left_bindings,
        binding_type right_bindings,
        binding_type up_bindings,
        binding_type down_bindings
      );
      void add_passthrough_point(
        const std::string& name,
        passthrough_point_source_type source
      );

    private:
      std::unordered_map<std::string, trigger_context> m_triggers;
      std::unordered_map<std::string, axis_context> m_axis;
      std::unordered_map<std::string, passthrough_context<SDL_Point>> m_passthrough_point;
  };
}
