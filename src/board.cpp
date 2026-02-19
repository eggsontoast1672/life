#include "life/board.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <vector>

// I want to get rid of this...
#include <iostream>

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

namespace life
{
    Board::Board(std::size_t width, std::size_t height)
        : m_cells(width * height, false),
          m_birth_mask(width * height, false),
          m_width(width),
          m_height(height)
    {
        // TODO: This stuff will go away in the future, just testing.

        assert(width >= 50);
        assert(height >= 50);

        const std::size_t center = 20 * m_width + 20;

        m_cells[center - 1] = true;
        m_cells[center] = true;
        m_cells[center + 1] = true;
    }

    auto Board::step_simulation() -> void
    {
        for (std::size_t index = 0; index < m_cells.size(); index++)
        {
            const std::uint8_t num_live_neighbors = get_live_neighbors(index);
            const bool is_live = m_cells[index] != 0;

            if (is_live && (num_live_neighbors < 2 || num_live_neighbors > 3))
            {
                // The cell should die
                std::cerr << "Cell at index " << index << " died\n";
            }
            else if (!is_live && num_live_neighbors == 3)
            {
                // The cell should be born
                std::cerr << "Cell at index " << index << " was born\n";
            }
        }
    }

    auto Board::render(SDL_Renderer *renderer) const -> void
    {
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

        for (std::size_t index = 0; index < m_cells.size(); index++)
        {
            if (!m_cells[index])
            {
                continue;
            }

            const int cell_x = index % m_width;
            const int cell_y = index / m_width;
            const SDL_FRect rect{
                cell_x * CELL_WIDTH,
                cell_y * CELL_HEIGHT,
                CELL_WIDTH,
                CELL_HEIGHT,
            };

            SDL_RenderFillRect(renderer, &rect);
        }
    }

    auto Board::position_to_index(std::size_t row, std::size_t column) const -> std::size_t
    {
        const std::size_t row_mod = row % m_height;
        const std::size_t column_mod = column % m_width;

        return row_mod * m_width + column_mod;
    }

    auto Board::get_neighbor_indices(std::size_t index) const -> std::array<std::size_t, 8>
    {
        std::array<std::size_t, 8> indices;
        const std::size_t row = index / m_width;
        const std::size_t column = index % m_width;

        indices[0] = position_to_index(row - 1, column - 1);
        indices[1] = position_to_index(row - 1, column);
        indices[2] = position_to_index(row - 1, column + 1);
        indices[3] = position_to_index(row, column - 1);
        indices[4] = position_to_index(row, column + 1);
        indices[5] = position_to_index(row + 1, column - 1);
        indices[6] = position_to_index(row + 1, column);
        indices[7] = position_to_index(row + 1, column + 1);

        return indices;
    }

    auto Board::get_live_neighbors(std::size_t index) const -> std::uint8_t
    {
        const std::array<std::size_t, 8> neighbors = get_neighbor_indices(index);
        std::size_t count = 0;

        for (std::size_t neighbor : neighbors)
        {
            if (m_cells[neighbor])
            {
                count++;
            }
        }

        return count;

        // return std::count_if(neighbors.cbegin(), neighbors.cend(),
        //                      [this](std::size_t neighbor)
        //                      { return m_cells[neighbor]; });
    }
}
