#include <SDL3/SDL.h>

int main(void) {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
    const char *message = SDL_GetError();
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "failed to initialize SDL: %s\n", message);
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow("The Game of Life", 800, 600, 0);
  if (window == NULL) {
    const char *message = SDL_GetError();
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "failed to create window: %s\n", message);
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
  if (renderer == NULL) {
    const char *message = SDL_GetError();
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "failed to create renderer %s\n", message);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  bool done = false;

  while (!done) {
    SDL_Event event = {0};
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        done = true;
      }
    }

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
