#include <cstdlib>

#include "../include/trollworks-backend-sdl/backend.hpp"
#include "../include/trollworks-backend-sdl/components.hpp"

namespace tw::sdl {
  sdl_backend::sdl_backend(const std::string& window_title)
    : m_window_title(window_title)
  {}

  sdl_backend& sdl_backend::with_logical_size(SDL_Point size) {
    m_window_size = size;
    return *this;
  }

  sdl_backend& sdl_backend::with_fullscreen(bool fullscreen) {
    m_window_fullscreen = fullscreen;
    return *this;
  }

  void sdl_backend::setup(tw::controlflow&) {
    SDL_Log("Initialize SDL");
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
      SDL_Log("Could not initialize SDL: %s", SDL_GetError());
      std::exit(EXIT_FAILURE);
    }

    SDL_Log("Detect Desktop Display Mode");
    SDL_DisplayMode dpy;
    if (SDL_GetDesktopDisplayMode(0, &dpy) != 0) {
      SDL_Log("Could not get desktop display mode: %s", SDL_GetError());
      std::exit(EXIT_FAILURE);
    }

    int window_width = m_window_fullscreen ? dpy.w : m_window_size.x;
    int window_height = m_window_fullscreen ? dpy.h : m_window_size.y;

    SDL_Log("Create Window");
    m_window = SDL_CreateWindow(
      m_window_title.c_str(),
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      window_width, window_height,
      SDL_WINDOW_SHOWN | (m_window_fullscreen ? SDL_WINDOW_FULLSCREEN : 0)
    );
    if (m_window == nullptr) {
      SDL_Log("Could not create window: %s", SDL_GetError());
      std::exit(EXIT_FAILURE);
    }

    SDL_Log("Create Renderer");
    m_renderer = SDL_CreateRenderer(
      m_window,
      -1,
      SDL_RENDERER_PRESENTVSYNC |
      SDL_RENDERER_ACCELERATED |
      SDL_RENDERER_TARGETTEXTURE
    );
    if (m_renderer == nullptr) {
      SDL_Log("Could not create renderer: %s", SDL_GetError());
      std::exit(EXIT_FAILURE);
    }

    SDL_RenderSetLogicalSize(m_renderer, m_window_size.x, m_window_size.y);

    SDL_Log("Create Application Surface");
    m_application_surface = SDL_CreateTexture(
      m_renderer,
      dpy.format,
      SDL_TEXTUREACCESS_TARGET,
      dpy.w,
      dpy.h
    );
    if (m_application_surface == nullptr) {
      SDL_Log("Could not create application surface: %s", SDL_GetError());
      std::exit(EXIT_FAILURE);
    }
  }

  void sdl_backend::teardown() {
    SDL_Log("Teardown Application Surface");
    SDL_DestroyTexture(m_application_surface);

    SDL_Log("Teardown Renderer");
    SDL_DestroyRenderer(m_renderer);

    SDL_Log("Teardown Window");
    SDL_DestroyWindow(m_window);

    SDL_Log("Teardown SDL");
    SDL_Quit();
  }

  void sdl_backend::poll_events(tw::controlflow& cf) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      m_sigh_event.publish(event, cf);

      if (event.type == SDL_QUIT) {
        SDL_Log("Exit event received");
        cf = tw::controlflow::exit;
      }
    }
  }

  void sdl_backend::render() {
    auto target = SDL_GetRenderTarget(m_renderer);
    SDL_SetRenderTarget(m_renderer, m_application_surface);

    SDL_SetRenderDrawColor(m_renderer, 25, 25, 25, 255);
    SDL_RenderClear(m_renderer);

    auto& registry = scene_manager::main().registry();

    auto cameras = registry.view<camera>();
    auto backgrounds = registry.view<background, sprite>();
    auto sprites = registry.view<transform, sprite>();

    backgrounds.use<sprite>();
    sprites.use<sprite>();

    for (auto e_camera : cameras) {
      auto& c_camera = cameras.get<camera>(e_camera);

      SDL_Rect viewport = {
        .x = static_cast<int>(std::floor(c_camera.viewport.x)),
        .y = static_cast<int>(std::floor(c_camera.viewport.y)),
        .w = static_cast<int>(std::ceil(c_camera.viewport.w)),
        .h = static_cast<int>(std::ceil(c_camera.viewport.h))
      };
      SDL_RenderSetViewport(m_renderer, &viewport);

      for (auto e_background : backgrounds) {
        auto& c_background = backgrounds.get<background>(e_background);
        auto& c_sprite = backgrounds.get<sprite>(e_background);

        if (c_background.stretch) {
          SDL_FRect dest = {
            .x = 0.0f,
            .y = 0.0f,
            .w = c_camera.viewport.w,
            .h = c_camera.viewport.h
          };

          SDL_RenderCopyF(m_renderer, c_sprite.texture, &c_sprite.source, &dest);
        }
        else {
          auto base_pos = c_camera.world_to_screen(c_background.offset);
          auto cam_scale = c_camera.scale();
          auto base_size = SDL_FPoint{
            .x = c_sprite.source.w * cam_scale.x,
            .y = c_sprite.source.h * cam_scale.y
          };

          Uint32 start_x = 0, end_x = 0;
          Uint32 start_y = 0, end_y = 0;

          if (c_background.repeat_x) {
            start_x = static_cast<Uint32>(std::floor(-base_pos.x / base_size.x)) - 1;
            end_x = static_cast<Uint32>(std::ceil((c_camera.viewport.w - base_pos.x) / base_size.x)) + 1;
          }

          if (c_background.repeat_y) {
            start_y = static_cast<Uint32>(std::floor(-base_pos.y / base_size.y)) - 1;
            end_y = static_cast<Uint32>(std::ceil((c_camera.viewport.h - base_pos.y) / base_size.y)) + 1;
          }

          for (auto x = start_x; x <= end_x; x++) {
            for (auto y = start_y; y <= end_y; y++) {
              SDL_FRect dest = {
                .x = base_pos.x + (x * base_size.x),
                .y = base_pos.y + (y * base_size.y),
                .w = base_size.x,
                .h = base_size.y
              };

              SDL_RenderCopyF(m_renderer, c_sprite.texture, &c_sprite.source, &dest);
            }
          }
        }
      }

      for (auto e_sprite : sprites) {
        auto& c_transform = sprites.get<transform>(e_sprite);
        auto& c_sprite = sprites.get<sprite>(e_sprite);

        auto screen_pos = c_camera.world_to_screen(c_transform.position);
        auto cam_scale = c_camera.scale();

        SDL_FRect dest = {
          .x = screen_pos.x,
          .y = screen_pos.y,
          .w = c_sprite.source.w * c_transform.scale.x * cam_scale.x,
          .h = c_sprite.source.h * c_transform.scale.y * cam_scale.y
        };

        SDL_RenderCopyF(m_renderer, c_sprite.texture, &c_sprite.source, &dest);
      }
    }

    SDL_RenderSetViewport(m_renderer, nullptr);

    SDL_SetRenderTarget(m_renderer, target);
    SDL_RenderCopy(m_renderer, m_application_surface, nullptr, nullptr);

    m_sigh_gui.publish();

    SDL_RenderPresent(m_renderer);
  }
}
