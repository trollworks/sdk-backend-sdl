# Trollworks SDK - SDL Backend

This repository contains an SDL backend for the
[Trollworks SDK](https://github.com/trollworks/sdk-core).

## Installation

> :construction: **TODO**

## Usage

```cpp
#include <trollworks.hpp>
#include <trollworks-backend-sdl.hpp>

int main(int argc, char** argv) {
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
