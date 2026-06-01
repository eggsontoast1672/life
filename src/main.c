#include <stdio.h>
#include <string.h>

#include <raylib.h>

#include "board.h"
#include "str_utils.h"
#include "timer.h"

typedef struct
{
    UVector2 board_size;
} Config;

static Config parse_config(int argc, char **argv)
{
    for (size_t i = 1; i < argc; i++)
    {
        if (str_strip_prefix((const char **)&argv[i], "--size="))
        {
            printf("size = %s\n", argv[i]);
        }
    }

    // TODO: Actually parse the size
    return (Config){.board_size = (UVector2){50, 50}};
}

typedef struct
{
    Board board;
    Timer timer;
    bool running;
} AppState;

static AppState state_init(void)
{
    return (AppState){
        .board = board_create(10, 10),
        .timer = timer_new(0.1),
        .running = false,
    };
}

static void update(AppState *state)
{
    const bool timer_elapsed = timer_tick(&state->timer);
    const Vector2 mouse_position = GetMousePosition();

    if (state->running)
    {
        if (timer_elapsed) board_step(&state->board);
        return;
    }

    board_update(&state->board, state->running);
}

static void state_draw(const AppState *state)
{
    if (state->running) ClearBackground(GREEN);
    else ClearBackground(RED);

    board_draw(state->board, state->running);
}

int main(int argc, char **argv)
{
    const Config config = parse_config(argc, argv);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game of Life");
    SetTargetFPS(60);

    AppState state = state_init();

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            state.running = !state.running;
        }

        update(&state);

        BeginDrawing();
        state_draw(&state);
        EndDrawing();
    }

    board_destroy(state.board);
}
