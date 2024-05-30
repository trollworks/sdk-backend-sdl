#include"../../../include/trollworks-backend-sdl/rendering/pipeline.hpp"

namespace tw::sdl::rendering {
  sprite_node::sprite_node(entt::resource<spritesheet> sheet, Uint32 cell)
    : m_sheet(sheet),
      m_cell(cell)
  {}

  void sprite_node::on_apply(SDL_Renderer* renderer) {
    if (m_sheet) {
      auto& source = m_sheet->cells[m_cell];
      SDL_RenderCopy(renderer, m_sheet->texture, &source, nullptr);
    }
  }
}
