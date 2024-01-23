#include "../include/trollworks-backend-sdl/animator.hpp"
#include "../include/trollworks-backend-sdl/components.hpp"

namespace tw::sdl {
  void animator::update(float delta_time, void*) {
    auto& registry = scene_manager::main().registry();

    auto animations = registry.view<sprite, animation>();

    for (auto e_animation : animations) {
      auto& c_sprite = animations.get<sprite>(e_animation);
      auto& c_animation = animations.get<animation>(e_animation);

      auto& current_frame = c_animation.frames[c_animation.frame_index];
      c_animation.frame_time += delta_time;

      if (c_animation.frame_time >= current_frame.frame_duration) {
        c_animation.frame_time -= current_frame.frame_duration;
        c_animation.frame_index = (c_animation.frame_index + 1) % c_animation.frames.size();
      }

      auto& next_frame = c_animation.frames[c_animation.frame_index];
      c_sprite.texture = next_frame.texture;
      c_sprite.source = next_frame.source;
    }
  }
}
