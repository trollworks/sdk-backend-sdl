#pragma once

#include <logfmtxx.hpp>

#include <trollworks.hpp>

#include "./SDL.hpp"

namespace tw::sdl::logging {
  namespace details {
    struct printer {
      void operator()(const std::string& message);
    };
  }

  using logger_type = logfmtxx::logger<std::chrono::system_clock, details::printer>;

  logger_type& logger();
}
