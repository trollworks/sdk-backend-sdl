#include "../../include/trollworks-backend-sdl/rendering/camera.hpp"

struct mvp {
  float a, b, c;
  float d, e, f;
  float g, h, i;

  static mvp identiy() {
    return {
      1, 0, 0,
      0, 1, 0,
      0, 0, 1
    };
  }

  static mvp translation(SDL_FPoint t) {
    return {
      1, 0, t.x,
      0, 1, t.y,
      0, 0, 1
    };
  }

  static mvp scale(SDL_FPoint s) {
    return {
      s.x, 0, 0,
      0, s.y, 0,
      0, 0, 1
    };
  }
};

mvp operator*(const mvp& m, float factor) {
  return {
    m.a * factor, m.b * factor, m.c * factor,
    m.d * factor, m.e * factor, m.f * factor,
    m.g * factor, m.h * factor, m.i * factor
  };
}

mvp operator*(float factor, const mvp& m) {
  return m * factor;
}

mvp operator*(const mvp& m1, const mvp& m2) {
  return {
    m1.a * m2.a + m1.b * m2.d + m1.c * m2.g,
    m1.a * m2.b + m1.b * m2.e + m1.c * m2.h,
    m1.a * m2.c + m1.b * m2.f + m1.c * m2.i,

    m1.d * m2.a + m1.e * m2.d + m1.f * m2.g,
    m1.d * m2.b + m1.e * m2.e + m1.f * m2.h,
    m1.d * m2.c + m1.e * m2.f + m1.f * m2.i,

    m1.g * m2.a + m1.h * m2.d + m1.i * m2.g,
    m1.g * m2.b + m1.h * m2.e + m1.i * m2.h,
    m1.g * m2.c + m1.h * m2.f + m1.i * m2.i
  };
}

SDL_FPoint operator*(const mvp& m, SDL_FPoint p) {
  return {
    m.a * p.x + m.b * p.y + m.c,
    m.d * p.x + m.e * p.y + m.f
  };
}

namespace tw::sdl::rendering {
  SDL_FPoint camera::center() const {
    return SDL_FPoint{viewport.w / 2.0f, viewport.h / 2.0f};
  }

  SDL_FPoint camera::scale() const {
    return SDL_FPoint{view.w / viewport.w, view.h / viewport.h};
  }

  SDL_FPoint camera::offset() const {
    return SDL_FPoint{-view.x, -view.y};
  }

  SDL_FPoint camera::world_to_screen(SDL_FPoint world_pos) const {
    auto center_mat = mvp::translation(center());
    auto scale_mat = mvp::scale(scale());
    auto offset_mat = mvp::translation(offset());

    auto world_transform = (center_mat * scale_mat) * offset_mat;
    auto screen_pos = world_transform * world_pos;

    return screen_pos;
  }
}
