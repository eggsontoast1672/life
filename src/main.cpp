#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>

#include "life/board.hpp"
#include "life/dashboard.hpp"

constexpr int GRID_WIDTH = 100;
constexpr int GRID_HEIGHT = 100;
constexpr float SCREEN_WIDTH = 600.0f;
constexpr float SCREEN_HEIGHT = 700.0f;
constexpr int ITERATIONS_PER_SECOND = 6;

struct AppState
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    life::Board board{GRID_WIDTH, GRID_HEIGHT};
    life::Dashboard dashboard;
};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // We need to use the new keyword here so that the lifetime of the application state transcends
    // the scope of this function. We have to be able to write this owning pointer into the raw one
    // pointed to by `appstate`, which would not be possible if we used a simple smart pointer.
    //
    // Another option that could be explored later is that we could have a single static instance
    // of the `AppState` struct that is initialized at compile time and then populate it here.
    AppState *state = new AppState();

    if (!SDL_CreateWindowAndRenderer("Game of Life", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &state->window, &state->renderer))
    {
        SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    *appstate = state;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    AppState *state = static_cast<AppState *>(appstate);

    switch (event->type)
    {
    case SDL_EVENT_KEY_DOWN:
        if (event->key.key == SDLK_SPACE)
        {
            state->board.step_simulation();
        }

        break;

    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;

    default:
        break;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    // This is actually quite unsafe, since we could change the way in which the appstate pointer
    // is initialized at the beginning and it would instantly invalidate this code. It would be
    // good to come up with a better way to do this (if we cared).
    AppState *state = static_cast<AppState *>(appstate);

    SDL_SetRenderDrawColor(state->renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(state->renderer);
    state->board.render(state->renderer);
    state->dashboard.render(state->renderer);
    SDL_RenderPresent(state->renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    delete static_cast<AppState *>(appstate);
}
