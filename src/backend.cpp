#include <cstdlib>

#include <trollworks/jobs.hpp>

#include "../include/trollworks-backend-sdl/backend.hpp"
#include "../include/trollworks-backend-sdl/logging.hpp"
#include "../include/trollworks-backend-sdl/components.hpp"
#include "../include/trollworks-backend-sdl/animator.hpp"
#include "../include/trollworks-backend-sdl/input/manager.hpp"
#include "../include/trollworks-backend-sdl/rendering/camera.hpp"
#include "../include/trollworks-backend-sdl/rendering/system.hpp"

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

  SDL_Window* sdl_backend::window() {
    return m_window;
  }

  SDL_Renderer* sdl_backend::renderer() {
    return m_renderer;
  }

  void sdl_backend::setup(tw::controlflow&) {
    SDL_SetMainReady();

    logging::logger().debug("Initialize SDL");
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
      logging::logger().error(
        "Could not initialize SDL",
        logfmtxx::field{"reason", SDL_GetError()}
      );
      std::exit(EXIT_FAILURE);
    }

    logging::logger().debug("Initialize SDL Image");
    Uint32 sdlimg_flags = 0
      | IMG_INIT_PNG
      | IMG_INIT_JPG
      ;

    if ((IMG_Init(sdlimg_flags) & sdlimg_flags) != sdlimg_flags) {
      logging::logger().error(
        "Could not initialize SDL Image",
        logfmtxx::field{"reason", IMG_GetError()}
      );
      std::exit(EXIT_FAILURE);
    }

    logging::logger().debug("Detect Desktop Display Mode");
    SDL_DisplayMode dpy;
    if (SDL_GetDesktopDisplayMode(0, &dpy) != 0) {
      logging::logger().error(
        "Could not get desktop display mode",
        logfmtxx::field{"reason", SDL_GetError()}
      );
      std::exit(EXIT_FAILURE);
    }

    int window_width = m_window_fullscreen ? dpy.w : m_window_size.x;
    int window_height = m_window_fullscreen ? dpy.h : m_window_size.y;

    logging::logger().debug("Create Window");
    m_window = SDL_CreateWindow(
      m_window_title.c_str(),
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      window_width, window_height,
      SDL_WINDOW_SHOWN | (m_window_fullscreen ? SDL_WINDOW_FULLSCREEN : 0)
    );
    if (m_window == nullptr) {
      logging::logger().error(
        "Could not create window",
        logfmtxx::field{"reason", SDL_GetError()}
      );
      std::exit(EXIT_FAILURE);
    }

    logging::logger().debug("Create Renderer");
    m_renderer = SDL_CreateRenderer(
      m_window,
      -1,
      SDL_RENDERER_PRESENTVSYNC |
      SDL_RENDERER_ACCELERATED |
      SDL_RENDERER_TARGETTEXTURE
    );
    if (m_renderer == nullptr) {
      logging::logger().error(
        "Could not create renderer",
        logfmtxx::field{"reason", SDL_GetError()}
      );
      std::exit(EXIT_FAILURE);
    }

    SDL_RenderSetLogicalSize(m_renderer, m_window_size.x, m_window_size.y);

    logging::logger().debug("Create Application Surface");
    m_application_surface = SDL_CreateTexture(
      m_renderer,
      dpy.format,
      SDL_TEXTUREACCESS_TARGET,
      window_width,
      window_height
    );
    if (m_application_surface == nullptr) {
      logging::logger().error(
        "Could not create application surface",
        logfmtxx::field{"reason", SDL_GetError()}
      );
      std::exit(EXIT_FAILURE);
    }

    job_manager::main().attach<animator>();

    auto& registry = scene_manager::main().registry();
    registry.on_construct<drawable>().connect<&sdl_backend::on_construct_drawable>(this);
    registry.on_destroy<drawable>().connect<&sdl_backend::on_destroy_drawable>(this);
  }

  void sdl_backend::on_construct_drawable(entt::registry& registry, entt::entity entity) {
    SDL_RendererInfo info;
    SDL_GetRendererInfo(m_renderer, &info);

    auto& c_drawable = registry.get<drawable>(entity);
    c_drawable.target = SDL_CreateTexture(
      m_renderer,
      info.texture_formats[0], // there is always at least one format
      SDL_TEXTUREACCESS_TARGET,
      c_drawable.box.w,
      c_drawable.box.h
    );

    if (c_drawable.target == nullptr) {
      logging::logger().error(
        "Could not create texture",
        logfmtxx::field{"reason", SDL_GetError()},
        logfmtxx::field{"entity", static_cast<ENTT_ID_TYPE>(entity)}
      );
    }
  }

  void sdl_backend::on_destroy_drawable(entt::registry& registry, entt::entity entity) {
    auto& c_drawable = registry.get<drawable>(entity);

    if (c_drawable.target != nullptr) {
      SDL_DestroyTexture(c_drawable.target);
      c_drawable.target = nullptr;
    }
  }

  void sdl_backend::teardown() {
    logging::logger().debug("Teardown Application Surface");
    SDL_DestroyTexture(m_application_surface);

    logging::logger().debug("Teardown Renderer");
    SDL_DestroyRenderer(m_renderer);

    logging::logger().debug("Teardown Window");
    SDL_DestroyWindow(m_window);

    logging::logger().debug("Teardown SDL Image");
    IMG_Quit();

    logging::logger().debug("Teardown SDL");
    SDL_Quit();
  }

  void sdl_backend::poll_events(tw::controlflow& cf) {
    SDL_Event event;

    input::manager::main().frame_begin();

    while (SDL_PollEvent(&event)) {
      input::manager::main().process_event(event);
      m_sigh_event.publish(event, cf);

      if (event.type == SDL_QUIT) {
        logging::logger().info("Exit event received");
        cf = tw::controlflow::exit;
      }
    }

    auto& registry = scene_manager::main().registry();
    input::manager::main().update(registry);
  }

  void sdl_backend::render() {
    auto target = SDL_GetRenderTarget(m_renderer);

    auto& registry = scene_manager::main().registry();

    rendering::system::prerender_phase(m_renderer, registry);

    SDL_SetRenderTarget(m_renderer, m_application_surface);
    SDL_SetRenderDrawColor(m_renderer, 25, 25, 25, 255);
    SDL_RenderClear(m_renderer);

    auto cameras = registry.view<rendering::camera, ordering>();
    cameras.use<ordering>();

    for (auto e_camera : cameras) {
      auto& c_camera = cameras.get<rendering::camera>(e_camera);

      SDL_Rect viewport = {
        .x = static_cast<int>(std::floor(c_camera.viewport.x)),
        .y = static_cast<int>(std::floor(c_camera.viewport.y)),
        .w = static_cast<int>(std::ceil(c_camera.viewport.w)),
        .h = static_cast<int>(std::ceil(c_camera.viewport.h))
      };
      SDL_RenderSetViewport(m_renderer, &viewport);

      rendering::system::background_phase(m_renderer, registry, c_camera);
      rendering::system::object_phase(m_renderer, registry, c_camera);
    }

    SDL_RenderSetViewport(m_renderer, nullptr);

    SDL_SetRenderTarget(m_renderer, target);
    SDL_RenderCopy(m_renderer, m_application_surface, nullptr, nullptr);

    m_sigh_gui.publish();

    SDL_RenderPresent(m_renderer);
  }
}
