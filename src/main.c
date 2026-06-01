#include <raylib.h>

#include "board.h"
#include "timer.h"

typedef struct
{
    Board board;
    Timer timer;
    bool running;
} AppState;

static AppState state_init(void)
{
    return (AppState){
        .board = board_create(50, 50),
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

int main(void)
{
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
