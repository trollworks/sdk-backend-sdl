#pragma once

#include "./SDL.hpp"

namespace tw::sdl {
  struct transform {
    SDL_FPoint position;
    SDL_FPoint scale;
  };

  struct background {
    bool stretch;
    SDL_FPoint offset;
    bool repeat_x;
    bool repeat_y;
  };

  struct sprite {
    SDL_Texture* texture;
    SDL_Rect source;

    Uint32 layer;
    Uint32 order_in_layer;
  };

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
