#pragma once

#include <trollworks.hpp>
#include <trollworks/jobs.hpp>

namespace tw::sdl {
  struct animator final : public job<animator> {
    public:
      void update(float delta_time, void*);
  };
}