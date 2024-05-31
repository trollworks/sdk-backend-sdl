#include <cstdlib>

#include <trollworks/jobs.hpp>

#include "../include/trollworks-backend-sdl/backend.hpp"
#include "../include/trollworks-backend-sdl/logging.hpp"
#include "../include/trollworks-backend-sdl/assets.hpp"
#include "../include/trollworks-backend-sdl/components.hpp"
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
    if (SDL_GetDesktopDisplayMode(0, &m_dpy) != 0) {
      logging::logger().error(
        "Could not get desktop display mode",
        logfmtxx::field{"reason", SDL_GetError()}
      );
      std::exit(EXIT_FAILURE);
    }

    int window_width = m_window_fullscreen ? m_dpy.w : m_window_size.x;
    int window_height = m_window_fullscreen ? m_dpy.h : m_window_size.y;

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

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengles2");

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

    SDL_RendererInfo info;
    SDL_GetRendererInfo(m_renderer, &info);
    if (std::string{info.name} != "opengles2") {
      logging::logger().error(
        "Could not create renderer with OpenGL ES 2.0",
        logfmtxx::field{"renderer", info.name}
      );
      std::exit(EXIT_FAILURE);
    }

    logging::logger().debug("Create Application Surface");
    m_application_surface = SDL_CreateTexture(
      m_renderer,
      m_dpy.format,
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

    job_manager::main().attach(
      [](float delta_time, void*, auto, auto) {
        auto& registry = scene_manager::main().registry();
        auto drawables = registry.view<drawable>();

        for (auto e_drawable : drawables) {
          auto& c_drawable = drawables.get<drawable>(e_drawable);
          c_drawable.pipeline.update(delta_time);
        }
      }
    );

    auto& registry = scene_manager::main().registry();
    registry.on_construct<drawable>().connect<&sdl_backend::on_construct_drawable>(this);
    registry.on_destroy<drawable>().connect<&sdl_backend::on_destroy_drawable>(this);

    registry.on_construct<rendering::camera>().connect<&sdl_backend::on_construct_camera>(this);
    registry.on_construct<ordering>().connect<&sdl_backend::on_construct_ordering>(this);
  }

  void sdl_backend::on_construct_drawable(entt::registry& registry, entt::entity entity) {
    auto& c_drawable = registry.get<drawable>(entity);
    c_drawable.pipeline.allocate(m_renderer, m_dpy.format);
  }

  void sdl_backend::on_destroy_drawable(entt::registry& registry, entt::entity entity) {
    auto& c_drawable = registry.get<drawable>(entity);
    c_drawable.pipeline.deallocate();
  }

  void sdl_backend::on_construct_camera(entt::registry& registry, entt::entity) {
    registry.sort<rendering::camera>([](auto& lhs, auto& rhs) {
      return lhs.depth < rhs.depth;
    });
  }

  void sdl_backend::on_construct_ordering(entt::registry& registry, entt::entity) {
    registry.sort<ordering>([](auto& lhs, auto& rhs) {
      if (lhs.layer == rhs.layer) {
        return lhs.order_in_layer < rhs.order_in_layer;
      }

      return lhs.layer < rhs.layer;
    });
  }

  void sdl_backend::teardown() {
    logging::logger().debug("Clear asset caches");
    tw::asset_manager<assets::aseprite::spritesheet>::cache().clear();
    tw::asset_manager<assets::aseprite::animation>::cache().clear();

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

    SDL_SetRenderTarget(m_renderer, m_application_surface);
    SDL_SetRenderDrawColor(m_renderer, 25, 25, 25, 255);
    SDL_RenderClear(m_renderer);

    rendering::system::run(m_renderer);

    SDL_SetRenderTarget(m_renderer, target);
    SDL_RenderCopy(m_renderer, m_application_surface, nullptr, nullptr);

    m_sigh_gui.publish();

    SDL_RenderPresent(m_renderer);
  }
}
