generate:
  cmake -B build

build:
  cmake --build build

run: build
  build/gameoflife
