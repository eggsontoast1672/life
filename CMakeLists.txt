cmake_minimum_required(VERSION 3.10)

project(GameOfLife LANGUAGES CXX)

find_package(PkgConfig)

pkg_check_modules(SDL3 REQUIRED sdl3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(
  gameoflife
  include/life/board.hpp
  include/life/dashboard.hpp
  src/board.cpp
  src/dashboard.cpp
  src/main.cpp
)

target_include_directories(gameoflife PRIVATE include ${SDL3_INCLUDE_DIRS})
target_link_libraries(gameoflife PRIVATE ${SDL3_LIBRARIES})
