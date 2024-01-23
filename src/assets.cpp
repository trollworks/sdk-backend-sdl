#include "../include/trollworks-backend-sdl/assets.hpp"

namespace tw::sdl::aseprite {
  spritesheet::spritesheet(spritesheet&& other) {
    m_texture = other.m_texture;
    m_size = other.m_size;
    m_frames = std::move(other.m_frames);
    other.m_texture = nullptr;
  }

  spritesheet::~spritesheet() {
    if (m_texture != nullptr) {
      SDL_DestroyTexture(m_texture);
    }
  }

  SDL_Point spritesheet::get_size() const {
    return m_size;
  }

  SDL_Texture* spritesheet::get_texture() const {
    return m_texture;
  }

  std::optional<spritesheet::frame> spritesheet::get_frame(
    const std::string& name
  ) const {
    if (auto it = m_frames.find(name); it != m_frames.end()) {
      return it->second;
    }

    return std::nullopt;
  }

  spritesheet::loader_type::result_type spritesheet::loader_type::operator()(
    SDL_Renderer* renderer,
    SDL_RWops* sheet_data,
    const nlohmann::json& sheet_meta
  ) const  {
    auto sheet = spritesheet{};

    sheet.m_texture = IMG_LoadTexture_RW(renderer, sheet_data, 1);
    if (sheet.m_texture == nullptr) {
      SDL_Log("Could not load spritesheet: %s", SDL_GetError());
      return nullptr;
    }

    try {
      sheet.m_size = SDL_Point{
        sheet_meta.at("meta").at("size").at("w").get<int>(),
        sheet_meta.at("meta").at("size").at("h").get<int>()
      };

      for (auto& [name, frame_meta] : sheet_meta.at("frames").items()) {
        auto source = SDL_Rect{
          frame_meta.at("frame").at("x").get<int>(),
          frame_meta.at("frame").at("y").get<int>(),
          frame_meta.at("frame").at("w").get<int>(),
          frame_meta.at("frame").at("h").get<int>()
        };
        auto duration = frame_meta.at("duration").get<float>() / 1000.0f; // ms -> s

        sheet.m_frames[name] = frame{
          .source = source,
          .duration = duration
        };
      }
    }
    catch (const std::exception& e) {
      SDL_Log("Could not parse spritesheet meta: %s", e.what());
      return nullptr;
    }

    return std::make_shared<spritesheet>(std::move(sheet));
  }
}
