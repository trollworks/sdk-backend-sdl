#pragma once

#include "./SDL.hpp"
#include "./rendering/pipeline.hpp"

namespace tw::sdl {
  struct transform {
    SDL_FPoint position;
    SDL_FRect box;
    SDL_FPoint scale;
  };

  struct ordering {
    Uint32 layer;
    Uint32 order_in_layer;
  };

  struct background {
    bool stretch;
    SDL_FPoint offset;
    bool repeat_x;
    bool repeat_y;
  };

  struct drawable {
    rendering::pipeline pipeline;
  };
}
