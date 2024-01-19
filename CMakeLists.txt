cmake_minimum_required(VERSION 3.22)
project(trollworks-sdk-backend-sdl CXX)

include(cmake/assert.cmake)
include(cmake/config.cmake)

add_subdirectory(vendor/SDL)
add_subdirectory(vendor/SDL_image)

file(GLOB_RECURSE TROLLWORKS_BACKEND_SDL_SOURCES
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

file(GLOB_RECURSE TROLLWORKS_BACKEND_SDL_HEADERS
  ${PROJECT_SOURCE_DIR}/include/*.hpp
)

add_library(${PROJECT_NAME} STATIC
  ${TROLLWORKS_BACKEND_SDL_SOURCES}
  ${TROLLWORKS_BACKEND_SDL_HEADERS}
)

if(WIN32)
  target_link_libraries(${PROJECT_NAME}
    SDL2-static
    SDL2_image
    opengl32
    glu32
  )
elseif(APPLE)
  target_link_libraries(${PROJECT_NAME}
    SDL2-static
    SDL2_image
    opengl
    glu
  )
elseif(UNIX)
  target_link_libraries(${PROJECT_NAME}
    SDL2-static
    SDL2_image
    opengl
    glu
  )
else()
  message(FATAL_ERROR "Platform not supported")
endif()

target_include_directories(${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/include
  PRIVATE ${PROJECT_SOURCE_DIR}/vendor/trollworks-sdk-core/include
  PRIVATE "$<TARGET_PROPERTY:SDL2-static,INTERFACE_INCLUDE_DIRECTORIES>"
  PRIVATE "$<TARGET_PROPERTY:SDL2_image,INTERFACE_INCLUDE_DIRECTORIES>"
)

target_compile_options(${PROJECT_NAME} PRIVATE
  -O2 -Wall -Werror -Wextra -std=c++23
)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/dist/debug")
else()
  set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/dist/release")
endif()
