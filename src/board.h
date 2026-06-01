#ifndef LIFE_BOARD_H
#define LIFE_BOARD_H

#include <stdbool.h>
#include <stddef.h>

#include <raylib.h>

#include "types.h"

extern const uint SCREEN_WIDTH;
extern const uint SCREEN_HEIGHT;

typedef struct
{
    bool *front_buffer, *back_buffer;
    uint width, height;
    Vector2 cell_size;
    Rectangle rect;
    UVector2 selected_square;
    bool has_selected_square;
} Board;

/// Create a new board.
///
/// This function creates a new board with the specified dimensions. If the cell buffers cannot be
/// allocated, then an error message is printed and the program exits immediately. To free the
/// memory allocated by this function, one should call `board_destroy`.
///
/// @param width The width of the board, in cells.
/// @param height The height of the board, in cells.
/// @return A new board with the specified dimensions.
Board board_create(uint width, uint height);

/// Destroy a board.
///
/// This function frees the memory associated with a board. The board passed to this function should
/// have front and back buffer pointers which point to memory on the heap, or are `NULL`.
///
/// @param board The board to destroy.
void board_destroy(Board board);

/// Get the value of a board cell.
///
/// This function retrieves the state of a cell in a board. In the case that `position` is not on
/// the board, an assertion error is generated.
///
/// @param board The board to be queried.
/// @param position The position of the cell whose state is to be gotten.
/// @return `true` if the cell is live, and `false` otherwise.
bool board_get_cell(Board board, UVector2 position);

/// Set the value of a board cell.
///
/// If `position` is outside of the allowable bounds for the board that was given, an assertion
/// error is generated.
///
/// @param board The board whose state is to be altered.
/// @param position The position of the cell to be altered.
/// @param is_live The new state that the cell should have.
void board_set_cell(Board *board, UVector2 position, bool is_live);

/// Step the simulation once.
///
/// The core functionality of the program comes from here.
///
/// @param board The board to update.
void board_step(Board *board);

/// Swap the front and back buffers.
///
/// The state of the board cannot be changed as it is being read, since each cell changes based on
/// what is around it at the current moment. For this reason, we need two buffers for the cells. The
/// front buffer holds the "current" state, and it is the one which is drawn to the screen. The back
/// buffer is written to when a step occurs. It is the responsibility of this function to swap the
/// pointers to the buffers to display the new state.
///
/// @param board The board whose buffers are to be swapped.
void board_swap_buffers(Board *board);

/// Draw the board to the screen.
///
/// @param board The board to be drawn.
/// @param running Whether or not the simulation is running.
void board_draw(Board board, bool running);

#endif
