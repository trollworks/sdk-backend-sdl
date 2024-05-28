#include"../../../include/trollworks-backend-sdl/rendering/pipeline.hpp"

namespace tw::sdl::rendering {
  color_node::color_node() : m_col(SDL_Color{25, 25, 25, 255}) {}
  color_node::color_node(SDL_Color color) : m_col(color) {}

  void color_node::on_apply(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, m_col.r, m_col.g, m_col.b, m_col.a);
    SDL_RenderClear(renderer);
  }
}
