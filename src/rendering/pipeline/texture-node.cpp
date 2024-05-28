#include"../../../include/trollworks-backend-sdl/rendering/pipeline.hpp"

namespace tw::sdl::rendering {
  texture_node::texture_node(SDL_Texture* texture, SDL_Rect source)
    : m_texture(texture),
      m_source(source)
  {}

  void texture_node::on_apply(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, m_texture, &m_source, nullptr);
  }
}
