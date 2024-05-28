#include"../../../include/trollworks-backend-sdl/rendering/pipeline.hpp"

namespace tw::sdl::rendering {
  animation_node::animation_node(entt::resource<animation> animation)
    : m_animation(animation)
  {}

  void animation_node::on_update(float delta_time) {
    auto& current_frame = m_animation->frames[m_frame_index];
    m_frame_time += delta_time;

    if (m_frame_time >= current_frame.duration) {
      m_frame_time -= current_frame.duration;
      m_frame_index = (m_frame_index + 1) % m_animation->frames.size();
    }
  }

  void animation_node::on_apply(SDL_Renderer* renderer) {
    auto& frame = m_animation->frames[m_frame_index];
    SDL_RenderCopy(renderer, m_animation->texture, &frame.source, nullptr);
  }
}
