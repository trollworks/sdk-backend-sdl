#include "../../include/trollworks-backend-sdl/rendering/data.hpp"

namespace tw::sdl::rendering {
  spritesheet::~spritesheet() {
    if (texture != nullptr) {
      SDL_DestroyTexture(texture);
    }
  }

  animation::~animation() {
    if (texture != nullptr) {
      SDL_DestroyTexture(texture);
    }
  }
}
