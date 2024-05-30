#include "../../include/trollworks-backend-sdl/assets.hpp"

namespace tw::sdl::assets {
  SDL_RWops* file_loader::open(const std::filesystem::path& filepath) {
    return SDL_RWFromFile(filepath.string().c_str(), "rb");
  }

  SDL_RWops* open_file(const std::filesystem::path& filepath) {
    auto& loader = entt::locator<file_loader>::value_or<file_loader>();
    return loader.open(filepath);
  }
}
