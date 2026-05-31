#include "board.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <raylib.h>

const uint SCREEN_WIDTH = 600;
const uint SCREEN_HEIGHT = 600;

/// Allocate one of the board's underlying buffers.
///
/// This is just a small utility function for allocating one of the board's cell buffers and
/// exiting the program immediately if something didn't work.
///
/// @param num_cells The number of cells the buffer should be able to hold.
/// @return A pointer to a heap-allocated buffer of booleans.
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

/// Convert an `(x, y)` position on the board to an index.
///
/// This function computes the index of the cell whose horizontal position is `x` and vertical
/// position is `y`. An assertion error will occur if the resulting index would exceed the valid
/// range for the given board.
///
/// @param board The board in which to compute the index.
/// @param position The position of the target cell.
/// @return The index of the target cell.
static size_t position_to_index(Board board, UVector2 position)
{
    assert(position.x < board.width);
    assert(position.y < board.height);

    // TODO: Will this ever overflow?
    return position.y * board.width + position.x;
}

/// Get the indices of the cells which neighbor the one at `index`.
///
/// This function returns a pointer to a static buffer, so it is only guaranteed to be valid up
/// until the next time the function is called. An assertion error will occur if `index` is outside
/// of the acceptible range for the given board.
///
/// @param board The board to be queried.
/// @param index The index of the cell whose neighboring indices are desired.
/// @return A pointer to an 8-long `size_t` array containing the indices.
static const size_t *get_neighboring_indices(Board board, size_t index)
{
    static size_t indices[8];

    const uint x = index % board.width;
    const uint y = index / board.width;

    const uint leftward = (x + board.width - 1) % board.width;
    const uint rightward = (x + 1) % board.width;
    const uint upward = (y + board.height - 1) % board.height;
    const uint downward = (y + 1) % board.height;

    indices[0] = position_to_index(board, (UVector2){leftward, upward});
    indices[1] = position_to_index(board, (UVector2){leftward, y});
    indices[2] = position_to_index(board, (UVector2){leftward, downward});
    indices[3] = position_to_index(board, (UVector2){x, upward});
    indices[4] = position_to_index(board, (UVector2){x, downward});
    indices[5] = position_to_index(board, (UVector2){rightward, upward});
    indices[6] = position_to_index(board, (UVector2){rightward, y});
    indices[7] = position_to_index(board, (UVector2){rightward, downward});

    return indices;
}

/// Retrieve the number of neighbors to the cell at `index` which are live.
///
/// @param board The board which is to be queried.
/// @param index The index of the cell whose neighbors are to be counted.
/// @return The number of live cells which neighbor the one at index `index`.
static uint8_t get_num_live_neighbors(Board board, size_t index)
{
    const size_t *const indices = get_neighboring_indices(board, index);
    uint8_t num_live_neighbors = 0;
    for (size_t i = 0; i < 8; i++)
    {
        if (board.front_buffer[indices[i]])
        {
            num_live_neighbors += 1;
        }
    }

    return num_live_neighbors;
}

Board board_create(uint width, uint height)
{
    const float PADDING = 5.0f;

    // Safety: The below computation can never overflow since the square of `UINT32_MAX` is less
    // than `UINT64_MAX`.
    const size_t num_cells = width * height;
    const Rectangle rect = {
        .x = PADDING,
        .y = PADDING,
        .width = SCREEN_WIDTH - PADDING * 2.0f,
        .height = SCREEN_HEIGHT - PADDING * 2.0f,
    };

    return (Board){
        .front_buffer = board_allocate_buffer(num_cells),
        .back_buffer = board_allocate_buffer(num_cells),
        .width = width,
        .height = height,
        .rect = rect,

        // This field dictates whether or not the `selected_square` field contains meaningful
        // information. Since this gets initialized to `false`, we can safely leave
        // `selected_square` unset for maximum efficiency. I hope I don't regret this decision
        // later.
        .has_selected_square = false,
    };
}

bool board_get_cell(Board board, UVector2 position)
{
    const size_t index = position_to_index(board, position);
    return board.front_buffer[index];
}

void board_set_cell(Board *board, UVector2 position, bool is_live)
{
    const size_t index = position_to_index(*board, position);
    board->front_buffer[index] = is_live;
}

void board_step(Board *board)
{
    for (size_t index = 0; index < board->width * board->height; index++)
    {
        const uint8_t num_live_neighbors = get_num_live_neighbors(*board, index);
        const bool this_cell_is_live = board->front_buffer[index];

        if (this_cell_is_live && (num_live_neighbors < 2 || num_live_neighbors > 3))
        {
            board->back_buffer[index] = false;
        }
        else if (!this_cell_is_live && num_live_neighbors == 3)
        {
            board->back_buffer[index] = true;
        }
        else
        {
            board->back_buffer[index] = this_cell_is_live;
        }
    }

    board_swap_buffers(board);
}

void board_swap_buffers(Board *board)
{
    bool *temp = board->front_buffer;
    board->front_buffer = board->back_buffer;
    board->back_buffer = temp;
}

void board_draw(Board board)
{
    const float CELL_WIDTH = (float)board.rect.width / board.width;
    const float CELL_HEIGHT = (float)board.rect.height / board.height;

    DrawRectangleRec(board.rect, BLACK);

    for (size_t y = 0; y < board.height; y++)
    {
        for (size_t x = 0; x < board.width; x++)
        {
            if (!board.front_buffer[y * board.width + x])
            {
                continue;
            }

            const Vector2 position = {board.rect.x + x * CELL_WIDTH,
                                      board.rect.y + y * CELL_HEIGHT};
            const Vector2 size = {CELL_WIDTH, CELL_HEIGHT};

            DrawRectangleV(position, size, WHITE);
        }
    }
}
