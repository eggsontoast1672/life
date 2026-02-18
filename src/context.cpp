#include <stdexcept>

#include <SDL3/SDL.h>

class Context {
public:
  Context() {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
      throw std::runtime_error{""};
    }
  }

  ~Context() {
    SDL_Quit();
  }
};
