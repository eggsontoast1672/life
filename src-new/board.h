#ifndef LIFE_BOARD_H
#define LIFE_BOARD_H

#include <stdbool.h>
#include <stddef.h>

extern const unsigned int SCREEN_WIDTH;
extern const unsigned int SCREEN_HEIGHT;
extern const unsigned int BOARD_WIDTH;
extern const unsigned int BOARD_HEIGHT;

typedef struct
{
    bool *front_buffer;
    bool *back_buffer;
    size_t width;
    size_t height;
} Board;

Board board_create(size_t width, size_t height);
void board_set_cell(Board *board, size_t x, size_t y, bool is_live);
void board_step(Board *board);
void board_swap_buffers(Board *board);
void board_draw(Board board);

#endif
