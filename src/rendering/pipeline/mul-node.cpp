#include"../../../include/trollworks-backend-sdl/rendering/pipeline.hpp"

namespace tw::sdl::rendering {
  mul_node::mul_node(std::initializer_list<node_ptr> nodes) : m_nodes(nodes) {}

  void mul_node::on_allocate(SDL_Renderer* renderer, Uint32 format, SDL_Point size) {
    details::node::on_allocate(renderer, format, size);

    if (target() != nullptr) {
      for (const auto& node : m_nodes) {
        node->allocate(renderer, format, size);
      }
    }
  }

  void mul_node::on_deallocate() {
    for (const auto& node : m_nodes) {
      node->deallocate();
    }

    details::node::on_deallocate();
  }

  void mul_node::on_update(float delta_time) {
    for (const auto& node : m_nodes) {
      node->update(delta_time);
    }
  }

  void mul_node::on_apply(SDL_Renderer* renderer) {
    for (const auto& node : m_nodes) {
      node->apply(renderer);
      SDL_SetTextureBlendMode(node->target(), SDL_BLENDMODE_MUL);
      SDL_RenderCopy(renderer, node->target(), nullptr, nullptr);
    }
  }
}
