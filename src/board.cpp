#include "life/board.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <vector>

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

        set_cell(0, 0, CellState::Live);
        set_cell(2, 0, CellState::Live);
        set_cell(2, 1, CellState::Live);
        set_cell(1, 1, CellState::Live);
        set_cell(1, 2, CellState::Live);
    }

    auto Board::set_cell(std::size_t row, std::size_t column, CellState state) -> void
    {
        const std::size_t index = row * m_width + column;
        m_cells[index] = state == CellState::Live;
    }

    auto Board::step_simulation() -> void
    {
        // Since the entire board has to be updated at once, we need to set up the birth mask in
        // the first pass and then update the state of the board according to it on the second
        // pass.
        for (std::size_t index = 0; index < m_cells.size(); index++)
        {
            const std::uint8_t num_live_neighbors = get_live_neighbors(index);
            const bool is_live = m_cells[index];

            if (is_live && (num_live_neighbors < 2 || num_live_neighbors > 3))
            {
                m_birth_mask[index] = false;
            }
            else if (!is_live && num_live_neighbors == 3)
            {
                m_birth_mask[index] = true;
            }
            else
            {
                m_birth_mask[index] = is_live;
            }
        }

        for (std::size_t index = 0; index < m_cells.size(); index++)
        {
            m_cells[index] = m_birth_mask[index];
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

            // This handy function computes the quotient and remainder of this division in one fell
            // swoop. The quotient is first, and it represents the y coordinate of the cell.
            // Naturally, the x coordinate of the cell is second.
            const auto [cell_y, cell_x] = std::lldiv(index, m_width);
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
        std::array<std::size_t, 8> neighbors = get_neighbor_indices(index);
        std::size_t count = 0;

        // I am deliberately choosing not to use the algorithm `std::count_if` here even though it
        // does exactly what I want. It makes the code harder to debug.
        for (std::size_t neighbor : neighbors)
        {
            if (m_cells[neighbor])
            {
                count++;
            }
        }

        return count;
    }
}
