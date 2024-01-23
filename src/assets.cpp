#include "../include/trollworks-backend-sdl/assets.hpp"

namespace tw::sdl::aseprite {
  spritesheet::spritesheet(spritesheet&& other) {
    texture = other.texture;
    size = other.size;
    frames = std::move(other.frames);
    other.texture = nullptr;
  }

  spritesheet::~spritesheet() {
    if (texture != nullptr) {
      SDL_DestroyTexture(texture);
    }
  }

  std::optional<sprite> spritesheet::get_sprite(const std::string& name) const {
    if (auto it = frames.find(name); it != frames.end()) {
      return sprite{
        texture,
        it->second,
        0,
        0
      };
    }

    return std::nullopt;
  }

  spritesheet::loader_type::result_type spritesheet::loader_type::operator()(
    SDL_Renderer* renderer,
    SDL_RWops* sheet_data,
    const nlohmann::json& sheet_meta
  ) const  {
    auto sheet = spritesheet{};

    sheet.texture = IMG_LoadTexture_RW(renderer, sheet_data, 1);
    if (sheet.texture == nullptr) {
      SDL_Log("Could not load spritesheet: %s", SDL_GetError());
      return nullptr;
    }

    try {
      sheet.size = SDL_Point{
        sheet_meta.at("meta").at("size").at("w").get<int>(),
        sheet_meta.at("meta").at("size").at("h").get<int>()
      };

      for (auto& [name, frame_meta] : sheet_meta.at("frames").items()) {
        auto frame = SDL_Rect{
          frame_meta.at("frame").at("x").get<int>(),
          frame_meta.at("frame").at("y").get<int>(),
          frame_meta.at("frame").at("w").get<int>(),
          frame_meta.at("frame").at("h").get<int>()
        };
        sheet.frames[name] = frame;
      }
    }
    catch (const std::exception& e) {
      SDL_Log("Could not parse spritesheet meta: %s", e.what());
      return nullptr;
    }

    return std::make_shared<spritesheet>(std::move(sheet));
  }
}