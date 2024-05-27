#include"../../../include/trollworks-backend-sdl/rendering/pipeline.hpp"

namespace tw::sdl::rendering::pipeline {
  texture_node::texture_node(SDL_Texture* texture, SDL_Rect source)
    : m_texture(texture),
      m_source(source)
  {}

  void texture_node::apply(SDL_Renderer* renderer) const {
    SDL_RenderCopy(renderer, m_texture, &m_source, nullptr);
  }
}
