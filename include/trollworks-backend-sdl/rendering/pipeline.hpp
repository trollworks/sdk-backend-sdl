#pragma once

#include <type_traits>
#include <concepts>

#include <memory>
#include <vector>

#include <trollworks.hpp>

#include "../SDL.hpp"
#include "./data.hpp"

namespace tw::sdl::rendering {
  class pipeline;

  namespace details {
    class node {
      public:
        SDL_Texture* target();

        void allocate(SDL_Renderer* renderer, Uint32 format, SDL_Point size);
        void deallocate();
        void update(float delta_time);
        void apply(SDL_Renderer* renderer);

      protected:
        virtual void on_allocate(SDL_Renderer* renderer, Uint32 format, SDL_Point size);
        virtual void on_deallocate();

        virtual void on_update(float delta_time);
        virtual void on_apply(SDL_Renderer* renderer);

      private:
        SDL_Texture* m_texture{nullptr};
    };

    template <typename T>
    concept node_trait = std::is_base_of<node, T>::value;
  }

  using node_ptr = std::shared_ptr<details::node>;

  template <details::node_trait Node, typename ... Args>
  node_ptr make_node(Args ... args) {
    return std::make_shared<Node>(std::forward(args)...);
  }

  class pipeline {
    public:
      pipeline(SDL_Point size, node_ptr root);

      void allocate(SDL_Renderer* renderer, Uint32 format);
      void deallocate();

      void update(float delta_time);
      void apply(SDL_Renderer* renderer);

      SDL_Texture* target();

    private:
      SDL_Point m_size;
      node_ptr m_root;
  };

  class color_node final : public details::node {
    public:
      color_node();
      color_node(SDL_Color color);

    private:
      void on_apply(SDL_Renderer* renderer) override;

    private:
      SDL_Color m_col;
  };

  class texture_node final : public details::node {
    public:
      texture_node(SDL_Texture* texture, SDL_Rect source);

    private:
      void on_apply(SDL_Renderer* renderer) override;

    private:
      SDL_Texture* m_texture;
      SDL_Rect m_source;
  };

  class sprite_node final : public details::node {
    public:
      sprite_node(entt::resource<spritesheet> sheet, Uint32 cell);

    private:
      void on_apply(SDL_Renderer* renderer) override;

    private:
      entt::resource<spritesheet> m_sheet;
      Uint32 m_cell;
  };

  class animation_node final : public details::node {
    public:
      animation_node(entt::resource<animation> animation);

    private:
      void on_update(float delta_time) override;
      void on_apply(SDL_Renderer* renderer) override;

    private:
      entt::resource<animation> m_animation;
      Uint32 m_frame_index;
      float m_frame_time;
  };

  class blend_node final : public details::node {
    public:
      blend_node(std::initializer_list<node_ptr> nodes);

    private:
      void on_allocate(SDL_Renderer* renderer, Uint32 format, SDL_Point size) override;
      void on_deallocate() override;

      void on_update(float delta_time) override;
      void on_apply(SDL_Renderer* renderer) override;

    private:
      std::vector<node_ptr> m_nodes;
  };

  class add_node final : public details::node {
    public:
      add_node(std::initializer_list<node_ptr> nodes);

    private:
      void on_allocate(SDL_Renderer* renderer, Uint32 format, SDL_Point size) override;
      void on_deallocate() override;

      void on_update(float delta_time) override;
      void on_apply(SDL_Renderer* renderer) override;

    private:
      std::vector<node_ptr> m_nodes;
  };

  class mul_node final : public details::node {
    public:
      mul_node(std::initializer_list<node_ptr> nodes);

    private:
      void on_allocate(SDL_Renderer* renderer, Uint32 format, SDL_Point size) override;
      void on_deallocate() override;

      void on_update(float delta_time) override;
      void on_apply(SDL_Renderer* renderer) override;

    private:
      std::vector<node_ptr> m_nodes;
  };

  class modulo_node final : public details::node {
    public:
      modulo_node(std::initializer_list<node_ptr> nodes);

    private:
      void on_allocate(SDL_Renderer* renderer, Uint32 format, SDL_Point size) override;
      void on_deallocate() override;

      void on_update(float delta_time) override;
      void on_apply(SDL_Renderer* renderer) override;

    private:
      std::vector<node_ptr> m_nodes;
  };
}
