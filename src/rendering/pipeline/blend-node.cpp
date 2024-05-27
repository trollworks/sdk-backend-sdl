#include"../../../include/trollworks-backend-sdl/rendering/pipeline.hpp"

namespace tw::sdl::rendering::pipeline {
  blend_node::blend_node(std::initializer_list<node_ptr> nodes) : m_nodes(nodes) {}

  void blend_node::apply(SDL_Renderer* renderer) const {
    auto texture = SDL_GetRenderTarget(renderer);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    for (const auto& node : m_nodes) {
      node->apply(renderer);
    }
  }
}
