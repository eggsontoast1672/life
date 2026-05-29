#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <raylib.h>

const unsigned int SCREEN_WIDTH = 600;
const unsigned int SCREEN_HEIGHT = 700;
const unsigned int BOARD_WIDTH = 100;
const unsigned int BOARD_HEIGHT = 100;

typedef struct
{
    bool *front_buffer;
    bool *back_buffer;
    size_t width;
    size_t height;
} Board;

static bool *board_allocate_buffer(size_t num_cells)
{
    bool *const buffer = calloc(num_cells, sizeof(bool));
    if (buffer == NULL)
    {
        perror("Failed to allocate buffer");
        exit(1);
    }

    return buffer;
}

static Board board_create(size_t width, size_t height)
{
    const size_t num_cells = width * height;
    bool *const front_buffer = board_allocate_buffer(num_cells);
    bool *const back_buffer = board_allocate_buffer(num_cells);

    return (Board){
        .front_buffer = front_buffer,
        .back_buffer = back_buffer,
        .width = width,
        .height = height,
    };
}

static void board_set_cell(Board *board, size_t x, size_t y, bool is_live)
{
    assert(x < BOARD_WIDTH);
    assert(y < BOARD_HEIGHT);

    const size_t index = y * BOARD_WIDTH + x;

    board->back_buffer[index] = is_live;
}

static void board_swap_buffers(Board *board)
{
    bool *temp = board->front_buffer;
    board->front_buffer = board->back_buffer;
    board->back_buffer = temp;
}

static void board_draw(Board board)
{
    const float CELL_WIDTH = (float)SCREEN_WIDTH / BOARD_WIDTH;
    const float CELL_HEIGHT = (float)SCREEN_HEIGHT / BOARD_HEIGHT;

    for (size_t y = 0; y < board.height; y++)
    {
        for (size_t x = 0; x < board.width; x++)
        {
            if (!board.front_buffer[y * BOARD_WIDTH + x])
            {
                continue;
            }

            const Vector2 position = {x * CELL_WIDTH, y * CELL_HEIGHT};
            const Vector2 size = {CELL_WIDTH, CELL_HEIGHT};

            DrawRectangleV(position, size, WHITE);
        }
    }
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game of Life");
    SetTargetFPS(60);

    Board board = board_create(100, 100);

    board_set_cell(&board, 0, 0, true);
    board_set_cell(&board, 1, 1, true);
    board_set_cell(&board, 1, 2, true);
    board_set_cell(&board, 2, 0, true);
    board_set_cell(&board, 2, 1, true);

    board_swap_buffers(&board);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        board_draw(board);
        EndDrawing();
    }
}
