#include "../include/trollworks-backend-sdl/logging.hpp"

namespace tw::sdl::logging {
  void logger_printer::operator()(const std::string& message) {
    SDL_Log("%s", message.c_str());
  }

  logger_type& logger() {
    if (!entt::locator<logger_type>::has_value()) {
      entt::locator<logger_type>::emplace(
        logger_printer{},
        logfmtxx::field{"backend", "sdl"}
      );
    }

    return entt::locator<logger_type>::value();
  }
}
