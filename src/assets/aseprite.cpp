#include <nlohmann/json.hpp>

#include "../../include/trollworks-backend-sdl/assets.hpp"
#include "../../include/trollworks-backend-sdl/logging.hpp"

namespace tw::sdl::assets::aseprite {
  spritesheet::loader_type::result_type spritesheet::loader_type::operator()(
    SDL_Renderer* renderer,
    const std::filesystem::path& path
  ) const {
    auto metadata_rw = open_asset(path);
    if (metadata_rw == nullptr) {
      logging::logger().error(
        "Could not open asset",
        logfmtxx::field{"asset-type", "aseprite::spritesheet"},
        logfmtxx::field{"reason", SDL_GetError()},
        logfmtxx::field{"path.metadata", path.c_str()}
      );
      return nullptr;
    }

    auto metadata = nlohmann::json::parse(SDL_RWreadAll(metadata_rw, true));

    auto image_relative_path = std::filesystem::path{
      metadata.at("meta").at("image").get<std::string>()
    };

    auto image_path = (path.parent_path() / image_relative_path).lexically_normal();
    auto image_rw = open_asset(image_path);
    if (image_rw == nullptr) {
      logging::logger().error(
        "Could not open image",
        logfmtxx::field{"asset-type", "aseprite::spritesheet"},
        logfmtxx::field{"reason", SDL_GetError()},
        logfmtxx::field{"path.metadata", path.c_str()},
        logfmtxx::field{"path.image", image_relative_path.c_str()}
      );

      return nullptr;
    }

    auto image = IMG_LoadTexture_RW(renderer, image_rw, true);
    if (image == nullptr) {
      logging::logger().error(
        "Could not load image",
        logfmtxx::field{"asset-type", "aseprite::spritesheet"},
        logfmtxx::field{"reason", IMG_GetError()},
        logfmtxx::field{"path.metadata", path.c_str()},
        logfmtxx::field{"path.image", image_relative_path.c_str()}
      );

      return nullptr;
    }

    auto sheet = std::make_shared<rendering::spritesheet>();
    sheet->texture = image;

    for (auto& [name, frame_meta] : metadata.at("frames").items()) {
      sheet->cells.push_back(SDL_Rect{
        frame_meta.at("frame").at("x").get<int>(),
        frame_meta.at("frame").at("y").get<int>(),
        frame_meta.at("frame").at("w").get<int>(),
        frame_meta.at("frame").at("h").get<int>()
      });
    }

    return sheet;
  }

  animation::loader_type::result_type animation::loader_type::operator()(
    SDL_Renderer* renderer,
    const std::filesystem::path& path
  ) const {
    auto metadata_rw = open_asset(path);
    if (metadata_rw == nullptr) {
      logging::logger().error(
        "Could not open asset",
        logfmtxx::field{"asset-type", "aseprite::animation"},
        logfmtxx::field{"reason", SDL_GetError()},
        logfmtxx::field{"path.metadata", path.c_str()}
      );
      return nullptr;
    }

    auto metadata = nlohmann::json::parse(SDL_RWreadAll(metadata_rw, true));

    auto image_relative_path = std::filesystem::path{
      metadata.at("meta").at("image").get<std::string>()
    };

    auto image_path = (path.parent_path() / image_relative_path).lexically_normal();
    auto image_rw = open_asset(image_path);
    if (image_rw == nullptr) {
      logging::logger().error(
        "Could not open image",
        logfmtxx::field{"asset-type", "aseprite::animation"},
        logfmtxx::field{"reason", SDL_GetError()},
        logfmtxx::field{"path.metadata", path.c_str()},
        logfmtxx::field{"path.image", image_relative_path.c_str()}
      );

      return nullptr;
    }

    auto image = IMG_LoadTexture_RW(renderer, image_rw, true);
    if (image == nullptr) {
      logging::logger().error(
        "Could not load image",
        logfmtxx::field{"asset-type", "aseprite::animation"},
        logfmtxx::field{"reason", IMG_GetError()},
        logfmtxx::field{"path.metadata", path.c_str()},
        logfmtxx::field{"path.image", image_relative_path.c_str()}
      );

      return nullptr;
    }

    auto anim = std::make_shared<rendering::animation>();
    anim->texture = image;

    for (auto& [name, frame_meta] : metadata.at("frames").items()) {
      anim->frames.push_back(rendering::animation::frame{
        .source = SDL_Rect{
          frame_meta.at("frame").at("x").get<int>(),
          frame_meta.at("frame").at("y").get<int>(),
          frame_meta.at("frame").at("w").get<int>(),
          frame_meta.at("frame").at("h").get<int>()
        },
        .duration = frame_meta.at("duration").get<float>() / 1000.0f // ms -> s
      });
    }

    return anim;
  }
}
