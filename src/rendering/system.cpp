#include "../../include/trollworks-backend-sdl/rendering/system.hpp"
#include "../../include/trollworks-backend-sdl/components.hpp"

namespace tw::sdl::rendering {
  void system::run(SDL_Renderer* renderer) {
    auto& registry = scene_manager::main().registry();

    prerender_phase(renderer, registry);

    auto cameras = registry.view<camera>();

    for (auto e_camera : cameras) {
      auto& c_camera = cameras.get<camera>(e_camera);

      SDL_Rect viewport = {
        .x = static_cast<int>(std::floor(c_camera.viewport.x)),
        .y = static_cast<int>(std::floor(c_camera.viewport.y)),
        .w = static_cast<int>(std::ceil(c_camera.viewport.w)),
        .h = static_cast<int>(std::ceil(c_camera.viewport.h))
      };
      SDL_RenderSetViewport(renderer, &viewport);

      background_phase(renderer, registry, c_camera);
      object_phase(renderer, registry, c_camera);
    }

    SDL_RenderSetViewport(renderer, nullptr);
  }

  void system::prerender_phase(SDL_Renderer* renderer, entt::registry& registry) {
    auto drawables = registry.view<drawable>();

    for (auto e_drawable : drawables) {
      auto& c_drawable = drawables.get<drawable>(e_drawable);
      c_drawable.pipeline.apply(renderer);
    }
  }

  void system::background_phase(
    SDL_Renderer* renderer,
    entt::registry& registry,
    camera& camera
  ) {
    auto backgrounds = registry.view<background, drawable, ordering>();
    backgrounds.use<ordering>();

    for (auto e_background : backgrounds) {
      auto& c_background = backgrounds.get<background>(e_background);
      auto& c_drawable   = backgrounds.get<drawable>(e_background);
      if (c_drawable.pipeline.target() == nullptr) continue;

      if (c_background.stretch) {
        SDL_FRect dest = {
          .x = 0.0f,
          .y = 0.0f,
          .w = camera.viewport.w,
          .h = camera.viewport.h
        };

        SDL_RenderCopyF(renderer, c_drawable.pipeline.target(), nullptr, &dest);
      }
      else {
        int w = 0, h = 0;
        SDL_QueryTexture(c_drawable.pipeline.target(), nullptr, nullptr, &w, &h);

        auto base_pos = camera.world_to_screen(c_background.offset);
        auto cam_scale = camera.scale();
        auto base_size = SDL_FPoint{
          .x = w * cam_scale.x,
          .y = h * cam_scale.y
        };

        Uint32 start_x = 0, end_x = 0;
        Uint32 start_y = 0, end_y = 0;

        if (c_background.repeat_x) {
          start_x = static_cast<Uint32>(std::floor(-base_pos.x / base_size.x)) - 1;
          end_x = static_cast<Uint32>(std::ceil((camera.viewport.w - base_pos.x) / base_size.x)) + 1;
        }

        if (c_background.repeat_y) {
          start_y = static_cast<Uint32>(std::floor(-base_pos.y / base_size.y)) - 1;
          end_y = static_cast<Uint32>(std::ceil((camera.viewport.h - base_pos.y) / base_size.y)) + 1;
        }

        for (auto x = start_x; x <= end_x; x++) {
          for (auto y = start_y; y <= end_y; y++) {
            SDL_FRect dest = {
              .x = base_pos.x + (x * base_size.x),
              .y = base_pos.y + (y * base_size.y),
              .w = base_size.x,
              .h = base_size.y
            };

            SDL_RenderCopyF(renderer, c_drawable.pipeline.target(), nullptr, &dest);
          }
        }
      }
    }
  }

  void system::object_phase(
    SDL_Renderer* renderer,
    entt::registry& registry,
    camera& camera
  ) {
    auto objects = registry.view<transform, drawable, ordering>();
    objects.use<ordering>();

    for (auto e_object : objects) {
      auto& c_transform = objects.get<transform>(e_object);
      auto& c_drawable = objects.get<drawable>(e_object);
      if (c_drawable.pipeline.target() == nullptr) continue;

      auto screen_pos = camera.world_to_screen(c_transform.position);
      auto cam_scale = camera.scale();

      SDL_FRect dest = {
        .x = screen_pos.x,
        .y = screen_pos.y,
        .w = c_transform.box.w * c_transform.scale.x * cam_scale.x,
        .h = c_transform.box.h * c_transform.scale.y * cam_scale.y
      };

      dest.x -= c_transform.box.x * dest.w;
      dest.y -= c_transform.box.y * dest.h;

      SDL_RenderCopyF(renderer, c_drawable.pipeline.target(), nullptr, &dest);
    }
  }
}
