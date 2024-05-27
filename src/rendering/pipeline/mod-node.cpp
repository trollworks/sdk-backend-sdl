#include"../../../include/trollworks-backend-sdl/rendering/pipeline.hpp"

namespace tw::sdl::rendering::pipeline {
  modulo_node::modulo_node(std::initializer_list<node_ptr> nodes) : m_nodes(nodes) {}

  void modulo_node::apply(SDL_Renderer* renderer) const {
    auto texture = SDL_GetRenderTarget(renderer);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_MOD);

    for (const auto& node : m_nodes) {
      node->apply(renderer);
    }
  }
}
