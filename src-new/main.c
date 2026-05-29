#include <raylib.h>

#include "board.h"

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
        if (IsKeyPressed(KEY_SPACE))
        {
            board_step(&board);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        board_draw(board);
        EndDrawing();
    }
}
