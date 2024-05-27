#include"../../../include/trollworks-backend-sdl/rendering/pipeline.hpp"

namespace tw::sdl::rendering::pipeline {
  add_node::add_node(std::initializer_list<node_ptr> nodes) : m_nodes(nodes) {}

  void add_node::apply(SDL_Renderer* renderer) const {
    auto texture = SDL_GetRenderTarget(renderer);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);

    for (const auto& node : m_nodes) {
      node->apply(renderer);
    }
  }
}
