#pragma once

#include <unordered_map>
#include <optional>
#include <utility>
#include <memory>
#include <string>

#include <nlohmann/json.hpp>
#include "./SDL.hpp"

namespace tw::sdl::aseprite {
  class spritesheet {
    public:
      struct frame {
        SDL_Rect source;
        float duration;
      };

      spritesheet() = default;
      spritesheet(const spritesheet&) = delete;
      spritesheet(spritesheet&& other);

      ~spritesheet();

      SDL_Point get_size() const;
      SDL_Texture* get_texture() const;
      std::optional<frame> get_frame(const std::string& name) const;

    private:
      SDL_Texture* m_texture;
      SDL_Point m_size;
      std::unordered_map<std::string, frame> m_frames;

    public:
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
