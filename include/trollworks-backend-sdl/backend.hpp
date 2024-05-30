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
      sdl_backend(const sdl_backend&) = delete;

      sdl_backend& with_logical_size(SDL_Point size);
      sdl_backend& with_fullscreen(bool fullscreen);

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

      SDL_Window* window();
      SDL_Renderer* renderer();

      void setup(tw::controlflow& cf);
      void teardown();
      void poll_events(tw::controlflow& cf);
      void render();

    private:
      void on_construct_drawable(entt::registry&, entt::entity);
      void on_destroy_drawable(entt::registry&, entt::entity);

      void on_construct_camera(entt::registry&, entt::entity);
      void on_construct_ordering(entt::registry&, entt::entity);

    private:
      std::string m_window_title;
      SDL_Point m_window_size{1366, 768};
      bool m_window_fullscreen{false};

      SDL_Window* m_window{nullptr};
      SDL_Renderer* m_renderer{nullptr};
      SDL_Texture* m_application_surface{nullptr};
  };
}
