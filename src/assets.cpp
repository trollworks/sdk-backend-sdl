#include "../include/trollworks-backend-sdl/assets.hpp"

namespace tw::sdl {
  rwops_asset::rwops_asset(rwops_asset&& other) {
    handle = other.handle;
    other.handle = nullptr;
  }

  rwops_asset::~rwops_asset() {
    if (handle != nullptr) {
      SDL_RWclose(handle);
    }
  }

  rwops_asset::loader_type::result_type rwops_asset::loader_type::operator()(
    from_disk_tag,
    const std::filesystem::path& path,
    bool binary
  ) const {
    auto asset = rwops_asset{};

    asset.handle = SDL_RWFromFile(path.string().c_str(), binary ? "rb" : "r");
    if (asset.handle == nullptr) {
      SDL_Log("Could not load asset: %s", SDL_GetError());
      return nullptr;
    }

    return std::make_shared<rwops_asset>(std::move(asset));
  }
}

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