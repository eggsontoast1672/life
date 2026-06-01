cmake_minimum_required(VERSION 3.28)

project(GameOfLife LANGUAGES C)

# Setting the `EXCLUDE_FROM_ALL` option here prevends the raylib header and library files from
# being added to the distribution archive.
add_subdirectory(vendor/raylib EXCLUDE_FROM_ALL)

add_executable(life src/board.c src/main.c src/timer.c)

set_property(TARGET life PROPERTY C_EXTENSIONS OFF)
set_property(TARGET life PROPERTY C_STANDARD 99)
set_property(TARGET life PROPERTY C_STANDARD_REQUIRED ON)

target_link_libraries(life PRIVATE raylib)

install(TARGETS life DESTINATION bin)
install(FILES LICENSE README.md DESTINATION .)

set(CPACK_PACKAGE_NAME "life")
set(CPACK_PACKAGE_VERSION "0.1.0")
set(CPACK_PACKAGE_VENDOR "Paul Zupan")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Implementation of John Conway's Game of Life")

set(CPACK_GENERATOR "TGZ;ZIP")

include(CPack)
