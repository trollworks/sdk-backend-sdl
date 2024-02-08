#pragma once

namespace tw::sdl::input {
  struct action_trigger {
    bool active;
    bool performed;
    bool cancelled;
  };

  struct action_axis {
    float x;
    float y;
  };

  template <typename T>
  struct action_passthrough {
    T raw;
  };
}
