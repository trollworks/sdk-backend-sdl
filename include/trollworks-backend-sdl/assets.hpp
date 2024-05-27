#pragma once

#include <type_traits>
#include <concepts>

#include <filesystem>
#include <memory>

#include <trollworks.hpp>

#include "./SDL.hpp"
#include "./rendering/data.hpp"

namespace tw::sdl::assets {
  namespace details {
    class loader {
      public:
        virtual SDL_RWops* open(const std::filesystem::path& filepath);
    };

    template <typename T>
    concept loader_trait = std::is_base_of<loader, T>::value;
  }

  template <details::loader_trait T, typename ... Args>
  void register_asset_loader(Args ... args) {
    entt::locator<details::loader>::emplace(T{std::forward<Args>(args)...});
  }

  SDL_RWops* open_asset(const std::filesystem::path& filepath);


  namespace aseprite {
    struct spritesheet {
      struct loader_type {
        using result_type = std::shared_ptr<rendering::spritesheet>;

        result_type operator()(
          SDL_Renderer* renderer,
          const std::filesystem::path& path
        ) const;
      };
    };

    struct animation {
      struct loader_type {
        using result_type = std::shared_ptr<rendering::animation>;

        result_type operator()(
          SDL_Renderer* renderer,
          const std::filesystem::path& path
        ) const;
      };
    };
  }
}
