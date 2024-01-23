#pragma once

#include <unordered_map>
#include <optional>
#include <utility>
#include <memory>
#include <string>

#include <nlohmann/json.hpp>
#include "./SDL.hpp"

namespace tw::sdl::aseprite {
  struct spritesheet {
    using frame_type = std::pair<SDL_Texture*, SDL_Rect>;

    SDL_Texture* texture;
    SDL_Point size;
    std::unordered_map<std::string, SDL_Rect> frames;

    spritesheet() = default;
    spritesheet(const spritesheet&) = delete;
    spritesheet(spritesheet&& other);

    ~spritesheet();

    std::optional<frame_type> get_frame(const std::string& name) const;

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
