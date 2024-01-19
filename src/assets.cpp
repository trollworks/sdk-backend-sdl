#include "../include/trollworks-backend-sdl/assets.hpp"

namespace tw::sdl::aseprite {
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
        sheet_meta["size"]["w"].get<int>(),
        sheet_meta["size"]["h"].get<int>()
      };

      for (auto& [name, frame_meta] : sheet_meta["frames"].items()) {
        auto frame = SDL_Rect{
          frame_meta["x"].get<int>(),
          frame_meta["y"].get<int>(),
          frame_meta["w"].get<int>(),
          frame_meta["h"].get<int>()
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