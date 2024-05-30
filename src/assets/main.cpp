#include "../../include/trollworks-backend-sdl/assets.hpp"

namespace tw::sdl::assets {
  SDL_RWops* file_loader::open(const std::filesystem::path& filepath) {
    return SDL_RWFromFile(filepath.string().c_str(), "rb");
  }

  SDL_RWops* open_file(const std::filesystem::path& filepath) {
    if (!entt::locator<file_loader>::has_value()) {
      return file_loader{}.open(filepath);
    }
    else {
      auto& loader = entt::locator<file_loader>::value();
      return loader.open(filepath);
    }
  }
}
