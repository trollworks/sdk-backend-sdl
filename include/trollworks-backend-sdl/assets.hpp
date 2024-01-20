#pragma once

#include <unordered_map>
#include <filesystem>
#include <optional>
#include <memory>
#include <string>

#include <nlohmann/json.hpp>
#include "./SDL.hpp"

#include "./components.hpp"

namespace tw::sdl {
  struct rwops_asset {
    SDL_RWops* handle;

    rwops_asset() = default;
    rwops_asset(const rwops_asset&) = delete;
    rwops_asset(rwops_asset&& other);
    ~rwops_asset();

    struct loader_type {
      using result_type = std::shared_ptr<rwops_asset>;
      struct from_disk_tag{};

      result_type operator()(
        from_disk_tag,
        const std::filesystem::path& path,
        bool binary = false
      ) const;
    };
  };
}

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
