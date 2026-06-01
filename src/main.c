#include <stdlib.h>

#include <raylib.h>

#include "board.h"
#include "timer.h"

typedef struct
{
    Board board;
    Timer timer;
    bool running;
} AppState;

// TODO: Having these here as static variables is not desireable to me. I am not exactly sure where
// to put them yet, perhaps with the other board data?
static float s_cell_width = 0;
static float s_cell_height = 0;

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

    s_cell_width = state->board.rect.width / state->board.width;
    s_cell_height = state->board.rect.height / state->board.height;

    if (state->running)
    {
        if (timer_elapsed)
        {
            board_step(&state->board);
        }
    }
    else
    {
        if (CheckCollisionPointRec(mouse_position, state->board.rect))
        {
            const uint x = (mouse_position.x - state->board.rect.x) / s_cell_width;
            const uint y = (mouse_position.y - state->board.rect.y) / s_cell_height;

            state->board.selected_square.x = x;
            state->board.selected_square.y = y;
            state->board.has_selected_square = true;
        }
        else
        {
            state->board.has_selected_square = false;
        }

        if (state->board.has_selected_square && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            const bool live = board_get_cell(state->board, state->board.selected_square);
            board_set_cell(&state->board, state->board.selected_square, !live);
        }
    }
}

static void draw_highlighted_cell(const AppState *state)
{
    if (!state->board.has_selected_square)
    {
        return;
    }

    // It may look like the operations done above are just getting undone here, but notice that we
    // are assigning the result of the computations to unsigned integer variables, thereby
    // truncating them.
    const Rectangle cell = {
        .x = state->board.rect.x + state->board.selected_square.x * s_cell_width,
        .y = state->board.rect.y + state->board.selected_square.y * s_cell_height,
        .width = s_cell_width,
        .height = s_cell_height,
    };

    DrawRectangleRec(cell, (Color){255, 255, 255, 50});
}

static void state_draw(const AppState *state)
{
    if (state->running) ClearBackground(GREEN);
    else ClearBackground(RED);

    board_draw(state->board, state->running);

    if (!state->running)
    {
        draw_highlighted_cell(state);
    }
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game of Life");
    SetTargetFPS(60);

    AppState state = state_init();

    board_set_cell(&state.board, (UVector2){0, 0}, true);
    board_set_cell(&state.board, (UVector2){1, 1}, true);
    board_set_cell(&state.board, (UVector2){1, 2}, true);
    board_set_cell(&state.board, (UVector2){2, 0}, true);
    board_set_cell(&state.board, (UVector2){2, 1}, true);

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
