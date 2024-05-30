#include "../../../include/trollworks-backend-sdl/rendering/pipeline.hpp"
#include "../../../include/trollworks-backend-sdl/logging.hpp"

namespace tw::sdl::rendering {
  namespace details {
    SDL_Texture* node::target() {
      return m_texture;
    }

    void node::allocate(SDL_Renderer* renderer, Uint32 format, SDL_Point size) {
      if (m_texture == nullptr) {
        on_allocate(renderer, format, size);
      }
    }

    void node::deallocate() {
      if (m_texture != nullptr) {
        on_deallocate();
      }
    }

    void node::update(float delta_time) {
      on_update(delta_time);
    }

    void node::apply(SDL_Renderer* renderer) {
      if (m_texture != nullptr) {
        auto old_target = SDL_GetRenderTarget(renderer);
        auto old_viewport = SDL_Rect{};
        SDL_RenderGetViewport(renderer, &old_viewport);

        SDL_SetRenderTarget(renderer, m_texture);
        SDL_RenderSetViewport(renderer, nullptr);

        on_apply(renderer);

        SDL_SetRenderTarget(renderer, old_target);
        SDL_RenderSetViewport(renderer, &old_viewport);
      }
    }

    void node::on_allocate(SDL_Renderer* renderer, Uint32 format, SDL_Point size) {
      m_texture = SDL_CreateTexture(
        renderer,
        format,
        SDL_TEXTUREACCESS_TARGET,
        size.x,
        size.y
      );

      if (m_texture == nullptr) {
        logging::logger().error(
          "Could not create texture",
          logfmtxx::field{"reason", SDL_GetError()}
        );
      }
    }

    void node::on_deallocate() {
      SDL_DestroyTexture(m_texture);
      m_texture = nullptr;
    }

    void node::on_update(float delta_time) {}
    void node::on_apply(SDL_Renderer* renderer) {}
  }

  pipeline::pipeline(SDL_Point size, node_ptr root) : m_size(size), m_root(root) {}

  void pipeline::allocate(SDL_Renderer* renderer, Uint32 format) {
    m_root->allocate(renderer, format, m_size);
  }

  void pipeline::deallocate() {
    m_root->deallocate();
  }

  void pipeline::update(float delta_time) {
    m_root->update(delta_time);
  }

  void pipeline::apply(SDL_Renderer* renderer) {
    m_root->apply(renderer);
  }

  SDL_Texture* pipeline::target() {
    return m_root->target();
  }
}
