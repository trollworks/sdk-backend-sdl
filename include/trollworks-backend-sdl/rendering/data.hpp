#pragma once

#include <vector>

#include "../SDL.hpp"


namespace tw::sdl::rendering {
  struct spritesheet {
    SDL_Texture* texture;
    std::vector<SDL_Rect> cells;

    ~spritesheet();
  };

  struct animation {
    struct frame {
      SDL_Rect source;
      float duration;
    };

    SDL_Texture* texture;
    std::vector<frame> frames;
    Uint32 frame_index;
    float frame_time;

    ~animation();
  };
}
