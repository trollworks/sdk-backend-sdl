cmake_minimum_required(VERSION 3.22)
project(SDL2_image)

set(SDL2IMAGE_SAMPLES OFF)
set(SDL2IMAGE_INSTALL ON)

set(SDL2_LIBRARY SDL2-static)
set(SDL2_INCLUDE_DIR "$<TARGET_PROPERTY:SDL2-static,INTERFACE_INCLUDE_DIRECTORIES>")

add_subdirectory(upstream)
