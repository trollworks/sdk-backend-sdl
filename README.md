# Trollworks SDK - SDL Backend

This repository contains an SDL backend for the
[Trollworks SDK](https://github.com/trollworks/sdk-core).

## Installation

> :construction: **TODO**

## Usage

```cpp
#include <trollworks.hpp>
#include <trollworks-backend-sdl.hpp>

int SDL_main(int argc, char** argv) {
  auto backend = tw::sdl::sdl_backend("My SDL Window");
  auto loop = tw::game_loop{};

  loop
    .with_fps(60)
    .with_ups(50)
    .with_backend(backend)
    .run();

  return 0;
}
```

> **NB:** `SDL.h` define a macro to replace `main` with `SDL_main`. I don't like
> this, so when including SDL I defined the macro `SDL_MAIN_HANDLED`. This means
> that you have to call your main function `SDL_main`, the SDL library will
> provide a proper entrypoint according to your platform.

The backend provides 2 events:

```cpp
struct listener {
  void on_event(const SDL_Event &evt, tw::controlflow& cf) {
    // do something with evt
  }

  void on_gui() {
    // the scene was rendered, you can now render your gui
  }
};

auto l = listener{};
auto backend = sdl_backend{"My Window Title"};

backend
  .on_event<&listener::on_event>(l)
  .on_gui<&listener::on_gui>(l);
```

## License

This project is released under the terms of the [MIT License](./LICENSE.txt).
