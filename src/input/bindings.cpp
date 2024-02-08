#include "../../include/trollworks-backend-sdl/input/bindings.hpp"

namespace tw::sdl::input {
  binding_type nobinding() {
    return std::make_unique<details::binding_check>();
  }
}

tw::sdl::input::binding_type operator|(
  tw::sdl::input::binding_type a,
  tw::sdl::input::binding_type b
) {
  return std::make_unique<tw::sdl::input::details::binding_or_combinator>(
    std::move(a),
    std::move(b)
  );
}

tw::sdl::input::binding_type operator&(
  tw::sdl::input::binding_type a,
  tw::sdl::input::binding_type b
) {
  return std::make_unique<tw::sdl::input::details::binding_and_combinator>(
    std::move(a),
    std::move(b)
  );
}
