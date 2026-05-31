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
        .board_rect =
            {
                PADDING,
                PADDING,
                SCREEN_WIDTH - PADDING * 2.0f,
                SCREEN_HEIGHT - PADDING * 2.0f,
            },
        .timer = timer_new(0.1),
        .running = false,
    };
}

static void update(AppState *state)
{
    const bool timer_elapsed = timer_tick(&state->timer);

    if (timer_elapsed && state->running)
    {
        board_step(&state->board);
    }
}

static void state_draw(const AppState *state)
{
    if (state->running) ClearBackground(GREEN);
    else ClearBackground(RED);

    board_draw(state->board, state->board_rect);

    // Draw the placement grid
    const float GRID_LINE_WIDTH = 1.0f;
    const Color GRID_LINE_COLOR = (Color){35, 35, 35, 255};
    const float CELL_WIDTH = state->board_rect.width / state->board.width;
    const float CELL_HEIGHT = state->board_rect.height / state->board.height;

    if (!state->running)
    {
        for (unsigned int x = 1; x < state->board.width; x++)
        {
            const Rectangle line = {
                .x = state->board_rect.x + x * CELL_WIDTH,
                .y = state->board_rect.y,
                .width = GRID_LINE_WIDTH,
                .height = state->board_rect.height,
            };

            DrawRectangleRec(line, GRID_LINE_COLOR);
        }

        for (unsigned int y = 1; y < state->board.height; y++)
        {
            const Rectangle line = {
                .x = state->board_rect.x,
                .y = state->board_rect.y + y * CELL_HEIGHT,
                .width = state->board_rect.width,
                .height = GRID_LINE_WIDTH,
            };

            DrawRectangleRec(line, GRID_LINE_COLOR);
        }
    }
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
