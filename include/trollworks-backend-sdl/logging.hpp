#pragma once

#include <logfmtxx.hpp>

#include <trollworks.hpp>

#include "./SDL.hpp"

namespace tw::sdl::logging {
  struct logger_printer {
    void operator()(const std::string& message);
  };

  using logger_type = logfmtxx::logger<std::chrono::system_clock, logger_printer>;

  logger_type& logger();
}
