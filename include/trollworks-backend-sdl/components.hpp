#pragma once

#include "./SDL.hpp"
#include "./rendering/pipeline.hpp"

namespace tw::sdl {
  struct transform {
    SDL_FPoint position;
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
    SDL_FRect box;
    SDL_Texture* target;
    rendering::pipeline::node_ptr pipeline;
  };

  struct animation {
    entt::resource<rendering::animation> animation;
  };
}
