#pragma once

#include <type_traits>
#include <concepts>

#include <filesystem>
#include <memory>

#include <trollworks.hpp>

#include "./SDL.hpp"
#include "./rendering/data.hpp"

namespace tw::sdl::assets {
  class file_loader {
    public:
      virtual SDL_RWops* open(const std::filesystem::path& filepath);
  };

  template <typename T>
  concept file_loader_trait = std::is_base_of<file_loader, T>::value;

  template <file_loader_trait T, typename ... Args>
  void register_file_loader(Args ... args) {
    entt::locator<file_loader>::emplace<T>(std::forward<Args>(args)...);
  }

  SDL_RWops* open_file(const std::filesystem::path& filepath);


  namespace aseprite {
    struct spritesheet {
      using resource_type = rendering::spritesheet;

      struct loader_type {
        using result_type = std::shared_ptr<resource_type>;

        result_type operator()(
          SDL_Renderer* renderer,
          const std::filesystem::path& path
        ) const;
      };
    };

    struct animation {
      using resource_type = rendering::animation;

      struct loader_type {
        using result_type = std::shared_ptr<resource_type>;

        result_type operator()(
          SDL_Renderer* renderer,
          const std::filesystem::path& path
        ) const;
      };
    };
  }
}
