#include <stdlib.h>

#include <raylib.h>

#include "board.h"
#include "timer.h"

typedef struct
{
    Board board;
    Rectangle board_rect;
    Timer timer;
    bool running;
} AppState;

static AppState state_init(void)
{
    const float PADDING = 5.0f;

    return (AppState){
        .board = board_create(50, 50),
        .board_rect = {
            PADDING,
            PADDING,
            SCREEN_WIDTH - PADDING * 2.0f,
            SCREEN_HEIGHT - PADDING * 2.0f,
        },
        .timer = timer_new(20),
        .running = false,
    };
}

static void update(AppState *state)
{
    timer_start_frame(&state->timer);

    if (state->running)
    {
        board_step(&state->board);
    }

    timer_end_frame(&state->timer);
}

static void state_draw(const AppState *state)
{
    if (state->running)
        ClearBackground(GREEN);
    else
        ClearBackground(RED);

    board_draw(state->board, state->board_rect);
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game of Life");
    SetTargetFPS(60);

    AppState state = state_init();

    board_set_cell(&state.board, 0, 0, true);
    board_set_cell(&state.board, 1, 1, true);
    board_set_cell(&state.board, 1, 2, true);
    board_set_cell(&state.board, 2, 0, true);
    board_set_cell(&state.board, 2, 1, true);

    board_swap_buffers(&state.board);

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

    free(state.board.front_buffer);
    free(state.board.back_buffer);
}
