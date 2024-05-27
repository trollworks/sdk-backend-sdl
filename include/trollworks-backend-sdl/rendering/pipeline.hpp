#pragma once

#include <type_traits>
#include <concepts>

#include <memory>
#include <vector>

#include <trollworks.hpp>

#include "../SDL.hpp"
#include "./data.hpp"

namespace tw::sdl::rendering::pipeline {
  namespace details {
    class node {
      public:
        virtual void apply(SDL_Renderer* renderer) const = 0;
    };

    template <typename T>
    concept node_trait = std::is_base_of<node, T>::value;
  }

  using node_ptr = std::shared_ptr<details::node>;

  template <details::node_trait Node, typename ... Args>
  node_ptr make_node(Args ... args) {
    return std::make_shared<Node>(std::forward(args)...);
  }

  class color_node final : public details::node {
    public:
      color_node();
      color_node(SDL_Color color);

      void apply(SDL_Renderer* renderer) const override;

    private:
      SDL_Color m_col;
  };

  class texture_node final : public details::node {
    public:
      texture_node(SDL_Texture* texture, SDL_Rect source);

      void apply(SDL_Renderer* renderer) const override;

    private:
      SDL_Texture* m_texture;
      SDL_Rect m_source;
  };

  class sprite_node final : public details::node {
    public:
      sprite_node(entt::resource<spritesheet> sheet, Uint32 cell);

      void apply(SDL_Renderer* renderer) const override;

    private:
      entt::resource<spritesheet> m_sheet;
      Uint32 m_cell;
  };

  class animation_node final : public details::node {
    public:
      animation_node(entt::resource<animation> animation);

      void apply(SDL_Renderer* renderer) const override;

    private:
      entt::resource<animation> m_animation;
  };

  class blend_node final : public details::node {
    public:
      blend_node(std::initializer_list<node_ptr> nodes);

      void apply(SDL_Renderer* renderer) const override;

    private:
      std::vector<node_ptr> m_nodes;
  };

  class add_node final : public details::node {
    public:
      add_node(std::initializer_list<node_ptr> nodes);

      void apply(SDL_Renderer* renderer) const override;

    private:
      std::vector<node_ptr> m_nodes;
  };

  class mul_node final : public details::node {
    public:
      mul_node(std::initializer_list<node_ptr> nodes);

      void apply(SDL_Renderer* renderer) const override;

    private:
      std::vector<node_ptr> m_nodes;
  };

  class modulo_node final : public details::node {
    public:
      modulo_node(std::initializer_list<node_ptr> nodes);

      void apply(SDL_Renderer* renderer) const override;

    private:
      std::vector<node_ptr> m_nodes;
  };
}
