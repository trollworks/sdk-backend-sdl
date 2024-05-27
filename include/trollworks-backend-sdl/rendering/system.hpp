#pragma once

#include <trollworks.hpp>

#include "../SDL.hpp"
#include "./camera.hpp"

namespace tw::sdl::rendering {
  class system {
    public:
      static void prerender_phase(SDL_Renderer* renderer, entt::registry& registry);
      static void background_phase(SDL_Renderer* renderer, entt::registry& registry, camera& camera);
      static void object_phase(SDL_Renderer* renderer, entt::registry& registry, camera& camera);
  };
}
