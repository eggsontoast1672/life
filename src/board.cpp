#include "life/board.hpp"

#include <cassert>
#include <cstddef>

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

namespace life
{
    Board::Board(std::size_t width, std::size_t height) : m_cells(width * height, 0), m_width(width), m_height(height)
    {
    }

    bool Board::is_cell_live(std::size_t row, std::size_t column) const
    {
        assert(row < m_width && "row out of range");
        assert(column < m_height && "column out of range");

        return m_cells[row * m_width + column] != 0;
    }

    void Board::set_cell(std::size_t row, std::size_t column, bool live)
    {
        assert(row < m_width && "row out of range");
        assert(column < m_height && "column out of range");

        m_cells[row * m_width + column] = live ? 1 : 0;
    }

    void Board::render(SDL_Renderer *renderer) const
    {
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

        for (std::size_t row = 0; row < m_width; row++)
        {
            for (std::size_t column = 0; column < m_height; column++)
            {
                if (!is_cell_live(row, column))
                    continue;

                SDL_FRect rect{column * CELL_WIDTH, row * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}
