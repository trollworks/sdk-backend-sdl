#pragma once

#include <variant>

#include <trollworks.hpp>
#include "../SDL.hpp"
#include "./state.hpp"
#include "./ui.hpp"

namespace tw::sdl::input {
  template <typename T>
  class passthrough_source {
    static_assert(sizeof(T) == 0, "unsupported passthrough data type");
  };

  template <>
  class passthrough_source<SDL_Point> {
    public:
      struct mouse_pointer {};
      struct mouse_delta {};
      struct mouse_wheel {};

      using type = std::variant<
        mouse_pointer,
        mouse_delta,
        mouse_wheel
      >;

    public:
      static void read(
        type source,
        entt::registry& registry,
        const state& state,
        SDL_Point& out
      ) {
        std::visit(
          [&](auto& src) { read_source(src, registry, state, out); },
          source
        );
      }

    private:
      static void read_source(
        mouse_wheel source,
        entt::registry& registry,
        const state& state,
        SDL_Point& out
      ) {
        if (!ui_captured_mouse(registry)) {
          out.x = state.mouse_wheel_x;
          out.y = state.mouse_wheel_y;
        }
      }

      static void read_source(
        mouse_delta source,
        entt::registry& registry,
        const state& state,
        SDL_Point& out
      ) {
        if (!ui_captured_mouse(registry)) {
          out.x = state.mouse_motion_x;
          out.y = state.mouse_motion_y;
        }
      }

      static void read_source(
        mouse_pointer source,
        entt::registry& registry,
        const state& state,
        SDL_Point& out
      ) {
        if (!ui_captured_mouse(registry)) {
          out.x = state.mouse_x;
          out.y = state.mouse_y;
        }
      }
  };

  using passthrough_point_source_type = typename passthrough_source<SDL_Point>::type;
}
