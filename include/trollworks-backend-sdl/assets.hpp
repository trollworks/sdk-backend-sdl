#pragma once

#include <unordered_map>
#include <optional>
#include <memory>
#include <string>

#include <nlohmann/json.hpp>
#include "./SDL.hpp"

#include "./components.hpp"

namespace tw::sdl::aseprite {
  struct spritesheet {
    SDL_Texture* texture;
    SDL_Point size;
    std::unordered_map<std::string, SDL_Rect> frames;

    spritesheet() = default;
    spritesheet(const spritesheet&) = delete;
    spritesheet(spritesheet&& other);

    ~spritesheet();

    std::optional<sprite> get_sprite(const std::string& name) const;

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
