#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include <nlohmann/json.hpp>
#include "./SDL.hpp"

namespace tw::sdl::aseprite {
  struct spritesheet {
    SDL_Texture* texture;
    SDL_Point size;
    std::unordered_map<std::string, SDL_Rect> frames;

    spritesheet() = default;
    spritesheet(const spritesheet&) = delete;
    spritesheet(spritesheet&& other) {
      texture = other.texture;
      size = other.size;
      frames = std::move(other.frames);
      other.texture = nullptr;
    }

    ~spritesheet() {
      if (texture != nullptr) {
        SDL_DestroyTexture(texture);
      }
    }

    struct loader_type {
      using result_type = std::shared_ptr<spritesheet>;

      result_type operator()(
        SDL_Renderer* renderer,
        SDL_RWops* sheet_data,
        const nlohmann::json& sheet_meta
      ) const;
    };
  };
}
