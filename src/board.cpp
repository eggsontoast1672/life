#include "life/board.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <utility>
#include <vector>

#include <iostream>

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

namespace life
{
    Board::Board(std::size_t width, std::size_t height)
        : m_buffers{std::vector(width * height, false), std::vector(width * height, false)},
          m_width(width),
          m_height(height),
          m_front_buffer(&m_buffers[0]),
          m_back_buffer(&m_buffers[1])
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
        (*m_front_buffer)[index] = state == CellState::Live;
    }

    auto Board::step_simulation() -> void
    {
        for (std::size_t index = 0; index < m_buffers[0].size(); index++)
        {
            const std::uint8_t num_live_neighbors = get_live_neighbors(index);
            const bool is_live = (*m_front_buffer)[index];

            if (is_live && (num_live_neighbors < 2 || num_live_neighbors > 3))
            {
                (*m_back_buffer)[index] = false;
            }
            else if (!is_live && num_live_neighbors == 3)
            {
                (*m_back_buffer)[index] = true;
            }
            else
            {
                (*m_back_buffer)[index] = is_live;
            }
        }

        // The front and back buffer system allows us to simply swap the pointers instead of
        // copying every element over. Sort of like a swapchain in graphics land!
        std::swap(m_front_buffer, m_back_buffer);
    }

    auto Board::render(SDL_Renderer *renderer) const -> void
    {
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

        for (std::size_t index = 0; index < m_buffers[0].size(); index++)
        {
            if (!(*m_front_buffer)[index])
            {
                continue;
            }

            if (m_is_logging)
            {
                std::cerr << "The index is " << index << '\n';
            }

            // This handy function computes the quotient and remainder of this division in one fell
            // swoop. The quotient is first, and it represents the y coordinate of the cell.
            // Naturally, the x coordinate of the cell is second.
            const auto [cell_y, cell_x] = std::lldiv(index, m_width);

            // TODO: Remove me.
            if (m_is_logging)
            {
                std::cerr << "Drawing cell at (" << cell_x << ", " << cell_y << ")\n";
            }

            const SDL_FRect rect{
                cell_x * CELL_WIDTH,
                cell_y * CELL_HEIGHT,
                CELL_WIDTH,
                CELL_HEIGHT,
            };

            SDL_RenderFillRect(renderer, &rect);
        }

        m_is_logging = false;
    }

    auto Board::position_to_index(std::int32_t row, std::int32_t column) const -> std::size_t
    {
        // We cannot simply use the modulus operator on its own, since this will not give the
        // correct result for negative numbers. In particular, we want the true mathematical
        // modulus here.
        const std::size_t row_mod = (row % m_height + m_height) % m_height;
        const std::size_t column_mod = column % m_width;

        return row_mod * m_width + column_mod;
    }

    auto Board::get_neighbor_indices(std::size_t index) const -> std::array<std::size_t, 8>
    {
        std::array<std::size_t, 8> indices;
        const std::int32_t row = index / m_width;
        const std::int32_t column = index % m_width;

        // FIXME: If the row or column index is zero, subtracting one will cause it to underflow,
        // which is creating this repeating behavior.
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
            if ((*m_front_buffer)[neighbor])
            {
                count++;
            }
        }

        return count;
    }
}
