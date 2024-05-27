#pragma once

#include "../SDL.hpp"

namespace tw::sdl::rendering {
  struct camera {
    Sint32 depth;
    SDL_FRect view;
    SDL_FRect viewport;

    SDL_FPoint center() const;
    SDL_FPoint scale() const;
    SDL_FPoint offset() const;
    SDL_FPoint world_to_screen(SDL_FPoint world_pos) const;
  };
}
