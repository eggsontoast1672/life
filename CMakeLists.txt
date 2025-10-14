cmake_minimum_required(VERSION 3.10)
project(GameOfLife)

find_package(PkgConfig)
pkg_check_modules(SDL3 REQUIRED sdl3)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(gameoflife src/main.c)
target_include_directories(gameoflife PRIVATE ${SDL3_INCLUDE_DIRS})
target_link_libraries(gameoflife PRIVATE ${SDL3_LIBRARIES})
