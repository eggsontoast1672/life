# The Game of Life

This is a somewhat minimal implementation of John Conway's classic Game of
Life. It is sometimes referred to as a "zero-player game", since once the
initial state is set, you can watch it develop all on its own without requiring
any further interaction. My focus is on robustness and correctness over sheer
volume of features. After all, how many features does this type of application
need anyway?

For anyone checking out my software portfolio, this project displays general
proficiency in code correctness. I have tried to structure the code as clearly
as possible, trying to stay away from unnecessary abstraction.

## Building

I have designed the project in such a way that it should build on every major
operating system. No matter which OS you are using, you will need to have the
following on your computer:

- A C compiler which supports C99 or later
- The CMake build system
- All Raylib dependencies\*

If you are on MacOS or Linux, you should already have an appropriate C compiler
installed by default. It would be easy enough to install a build tool such as
GNU Make or Ninja, the build files for which can be outputted by CMake. If you
are on Windows, I would instead recommend downloading Visual Studio and
enabling the Desktop Development with C++ preset, and then configuring the
project through the build-in CMake functionality. \*Finally, the dependencies
required for Raylib can vary between systems, so you should consult their
documentation to find out what you need.

Once you have acquired the correct software, you can configure the build,
compile the project, and run the program with the following sequence of
commands:

```bash
cmake -B build
cmake --build build/
build/life
```
