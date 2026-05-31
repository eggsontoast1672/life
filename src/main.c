#include <stdlib.h>

#include <raylib.h>

#include "board.h"
#include "timer.h"

typedef struct
{
    unsigned int x;
    unsigned int y;
} UVector2;

typedef struct
{
    Board board;
    Timer timer;
    Rectangle board_rect;
    UVector2 selected_square;
    bool is_selected;
    bool running;
} AppState;

// TODO: Having these here as static variables is not desireable to me. I am not exactly sure where
// to put them yet, perhaps with the other board data?
static float s_cell_width = 0;
static float s_cell_height = 0;

static AppState state_init(void)
{
    const float PADDING = 5.0f;

    return (AppState){
        .board = board_create(10, 10),
        .board_rect =
            {
                PADDING,
                PADDING,
                SCREEN_WIDTH - PADDING * 2.0f,
                SCREEN_HEIGHT - PADDING * 2.0f,
            },
        .timer = timer_new(0.1),
        .is_selected = false,
        .running = false,
    };
}

static void update(AppState *state)
{
    const bool timer_elapsed = timer_tick(&state->timer);
    const Vector2 mouse_position = GetMousePosition();

    s_cell_width = state->board_rect.width / state->board.width;
    s_cell_height = state->board_rect.height / state->board.height;

    if (state->running)
    {
        if (timer_elapsed)
        {
            board_step(&state->board);
        }
    }
    else
    {
        if (CheckCollisionPointRec(mouse_position, state->board_rect))
        {
            const unsigned int x = (mouse_position.x - state->board_rect.x) / s_cell_width;
            const unsigned int y = (mouse_position.y - state->board_rect.y) / s_cell_height;

            state->selected_square.x = x;
            state->selected_square.y = y;
            state->is_selected = true;
        }
        else
        {
            state->is_selected = false;
        }

        if (state->is_selected && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            const bool live =
                board_get_cell(state->board, state->selected_square.x, state->selected_square.y);
            board_set_cell(&state->board, state->selected_square.x, state->selected_square.y,
                           !live);
        }
    }
}

static void draw_grid_lines(const AppState *state)
{
    const float GRID_LINE_WIDTH = 1.0f;
    const Color GRID_LINE_COLOR = (Color){35, 35, 35, 255};
    const float CELL_WIDTH = state->board_rect.width / state->board.width;
    const float CELL_HEIGHT = state->board_rect.height / state->board.height;

    // TODO: This could be done in a single loop if the index is used as both a horizontal and
    // vertical position at once.
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

static void draw_highlighted_cell(const AppState *state)
{
    if (!state->is_selected)
    {
        return;
    }

    // It may look like the operations done above are just getting undone here, but notice that we
    // are assigning the result of the computations to unsigned integer variables, thereby
    // truncating them.
    const Rectangle cell = {
        .x = state->board_rect.x + state->selected_square.x * s_cell_width,
        .y = state->board_rect.y + state->selected_square.y * s_cell_height,
        .width = s_cell_width,
        .height = s_cell_height,
    };

    DrawRectangleRec(cell, (Color){255, 255, 255, 50});
}

static void state_draw(const AppState *state)
{
    if (state->running) ClearBackground(GREEN);
    else ClearBackground(RED);

    board_draw(state->board, state->board_rect);

    if (!state->running)
    {
        draw_grid_lines(state);
        draw_highlighted_cell(state);
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
