#include"../../../include/trollworks-backend-sdl/rendering/pipeline.hpp"

namespace tw::sdl::rendering {
  modulo_node::modulo_node(std::initializer_list<node_ptr> nodes) : m_nodes(nodes) {}

  void modulo_node::on_allocate(SDL_Renderer* renderer, SDL_Point size) {
    details::node::on_allocate(renderer, size);

    if (target() != nullptr) {
      for (const auto& node : m_nodes) {
        node->allocate(renderer, size);
      }
    }
  }

  void modulo_node::on_deallocate() {
    for (const auto& node : m_nodes) {
      node->deallocate();
    }

    details::node::on_deallocate();
  }

  void modulo_node::on_update(float delta_time) {
    for (const auto& node : m_nodes) {
      node->update(delta_time);
    }
  }

  void modulo_node::on_apply(SDL_Renderer* renderer) {
    for (const auto& node : m_nodes) {
      node->apply(renderer);
      SDL_SetTextureBlendMode(node->target(), SDL_BLENDMODE_MOD);
      SDL_RenderCopy(renderer, node->target(), nullptr, nullptr);
    }
  }
}
