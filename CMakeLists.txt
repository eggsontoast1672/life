cmake_minimum_required(VERSION 3.28)

project(GameOfLife LANGUAGES C)

include(FetchContent)

FetchContent_Declare(raylib GIT_REPOSITORY https://github.com/raysan5/raylib.git GIT_TAG 6.0)
FetchContent_MakeAvailable(raylib)

add_executable(life src/board.c src/main.c)

set_property(TARGET life PROPERTY C_EXTENSIONS OFF)
set_property(TARGET life PROPERTY C_STANDARD 99)
set_property(TARGET life PROPERTY C_STANDARD_REQUIRED ON)

target_link_libraries(life PRIVATE raylib)
