#include "../../include/trollworks-backend-sdl/assets.hpp"

namespace tw::sdl::assets {
  SDL_RWops* details::loader::open(const std::filesystem::path& filepath) {
    return SDL_RWFromFile(filepath.c_str(), "rb");
  }

  SDL_RWops* open_asset(const std::filesystem::path& filepath) {
    auto& loader = entt::locator<details::loader>::value_or<details::loader>();
    return loader.open(filepath);
  }
}
