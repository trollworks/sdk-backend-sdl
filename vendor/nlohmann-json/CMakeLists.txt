cmake_minimum_required(VERSION 3.22)
project(nlohmann_json)

add_library(nlohmann_json INTERFACE)
add_library(${PROJECT_NAME}::nlohmann_json ALIAS nlohmann_json)

target_include_directories(nlohmann_json
  INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
