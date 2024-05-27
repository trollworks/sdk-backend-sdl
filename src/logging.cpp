#include "../include/trollworks-backend-sdl/logging.hpp"

namespace tw::sdl::logging {
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
