cmake_minimum_required(VERSION 3.22)
project(nlohmann_json)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(upstream)
