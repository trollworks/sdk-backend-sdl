#pragma once

#include <trollworks.hpp>

#include "./SDL.hpp"

namespace tw::sdl {
  class sdl_backend {
    private:
      using sigh_event_type = entt::sigh<void(const SDL_Event&, controlflow&)>;
      using sigh_gui_type = entt::sigh<void()>;

      sigh_event_type m_sigh_event;
      sigh_gui_type m_sigh_gui;

      entt::sink<sigh_event_type> m_sink_event{m_sigh_event};
      entt::sink<sigh_gui_type> m_sink_gui{m_sigh_gui};

    public:
      sdl_backend(const std::string& window_title);

      template <auto Candidate, typename... Args>
      sdl_backend& on_event(Args&&... args) {
        m_sink_event.template connect<Candidate>(std::forward<Args>(args)...);
        return *this;
      }

      template <auto Candidate, typename... Args>
      sdl_backend& on_gui(Args&&... args) {
        m_sink_gui.template connect<Candidate>(std::forward<Args>(args)...);
        return *this;
      }

      void setup(tw::controlflow& cf);
      void teardown();
      void poll_events(tw::controlflow& cf);
      void render();

    private:
      std::string m_window_title;
      SDL_Window* m_window{nullptr};
      SDL_Renderer* m_renderer{nullptr};
      SDL_Texture* m_application_surface{nullptr};
  };
}
