#include"../../../include/trollworks-backend-sdl/rendering/pipeline.hpp"

namespace tw::sdl::rendering::pipeline {
  animation_node::animation_node(entt::resource<animation> animation)
    : m_animation(animation)
  {}

  void animation_node::apply(SDL_Renderer* renderer) const {
    auto& frame = m_animation->frames[m_animation->frame_index];
    SDL_RenderCopy(renderer, m_animation->texture, &frame.source, nullptr);
  }
}
