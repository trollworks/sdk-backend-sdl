#pragma once

#include <memory>

#include <trollworks.hpp>
#include "../SDL.hpp"
#include "./state.hpp"
#include "./ui.hpp"

namespace tw::sdl::input {
  namespace details {
    class binding_check {
      public:
        virtual ~binding_check() {};

        virtual bool check(entt::registry& registry, const state& state) const {
          return false;
        }
    };

    class binding_or_combinator final : public binding_check {
      public:
        binding_or_combinator(
          std::unique_ptr<binding_check> a,
          std::unique_ptr<binding_check> b
        ) : m_a(std::move(a)), m_b(std::move(b)) {}

        virtual bool check(entt::registry& registry, const state& state) const {
          return m_a->check(registry, state) || m_b->check(registry, state);
        }

      private:
        std::unique_ptr<binding_check> m_a;
        std::unique_ptr<binding_check> m_b;
    };

    class binding_and_combinator final : public binding_check {
      public:
        binding_and_combinator(
          std::unique_ptr<binding_check> a,
          std::unique_ptr<binding_check> b
        ) : m_a(std::move(a)), m_b(std::move(b)) {}

        virtual bool check(entt::registry& registry, const state& state) const {
          return m_a->check(registry, state) && m_b->check(registry, state);
        }

      private:
        std::unique_ptr<binding_check> m_a;
        std::unique_ptr<binding_check> m_b;
    };

    template <Uint8 scancode>
    class key_binding final : public binding_check {
      public:
        virtual bool check(entt::registry& registry, const state& state) const {
          return (
            !ui_captured_keyboard(registry)
            && state.keyboard_state[scancode]
          );
        }
    };

    template <Uint32 buttonmask>
    class mouse_button_binding final : public binding_check {
      public:
        virtual bool check(entt::registry& registry, const state& state) const {
          return (
            !ui_captured_mouse(registry)
            && (state.mouse_button_mask & buttonmask) != 0
          );
        }
    };
  }

  using binding_type = std::unique_ptr<details::binding_check>;

  binding_type nobinding();

  template <Uint8 scancode>
  binding_type key() {
    return std::make_unique<details::key_binding<scancode>>();
  }

  template <Uint32 buttonmask>
  binding_type mouse_button() {
    return std::make_unique<details::mouse_button_binding<buttonmask>>();
  }
}

tw::sdl::input::binding_type operator|(
  tw::sdl::input::binding_type a,
  tw::sdl::input::binding_type b
);

tw::sdl::input::binding_type operator&(
  tw::sdl::input::binding_type a,
  tw::sdl::input::binding_type b
);
