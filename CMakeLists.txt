cmake_minimum_required(VERSION 3.10)

project(GameOfLife LANGUAGES C CXX)

find_package(PkgConfig)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

pkg_check_modules(SDL3 REQUIRED sdl3)

FetchContent_Declare(raylib GIT_REPOSITORY https://github.com/raysan5/raylib.git GIT_TAG 6.0)
FetchContent_MakeAvailable(raylib)

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

#############
# C Rewrite #
#############

add_executable(life src-new/main.c)

set_property(TARGET life PROPERTY C_EXTENSIONS OFF)
set_property(TARGET life PROPERTY C_STANDARD 99)
set_property(TARGET life PROPERTY C_STANDARD_REQUIRED ON)

target_link_libraries(life PRIVATE raylib)
