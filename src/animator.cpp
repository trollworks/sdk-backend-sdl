#include "../include/trollworks-backend-sdl/animator.hpp"
#include "../include/trollworks-backend-sdl/components.hpp"

namespace tw::sdl {
  void animator::update(float delta_time, void*) {
    auto& registry = scene_manager::main().registry();

    auto animations = registry.view<animation>();

    for (auto e_animation : animations) {
      auto& c_animation = animations.get<animation>(e_animation);
      auto& anim        = *c_animation.animation;

      auto& current_frame = anim.frames[c_animation.frame_index];
      c_animation.frame_time += delta_time;

      if (c_animation.frame_time >= current_frame.duration) {
        c_animation.frame_time -= current_frame.duration;
        c_animation.frame_index = (c_animation.frame_index + 1) % anim.frames.size();
      }
    }
  }
}
