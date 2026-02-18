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
          m_height(height) {}

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
            const int cell_y = index / m_height;
            const SDL_FRect rect{
                cell_x * CELL_WIDTH,
                cell_y * CELL_HEIGHT,
                CELL_WIDTH,
                CELL_HEIGHT,
            };

            SDL_RenderFillRect(renderer, &rect);
        }
    }

    auto Board::get_neighbor_indices(std::size_t index) const -> std::array<std::size_t, 8>
    {
        std::array<std::size_t, 8> indices;

        indices[0] = indices[1] - 1;
        indices[1] = index - m_width;
        indices[2] = indices[1] + 1;
        indices[3] = index - 1;
        indices[4] = index + 1;
        indices[5] = indices[6] - 1;
        indices[6] = index + m_width;
        indices[7] = indices[6] + 1;

        return indices;
    }

    auto Board::get_live_neighbors(std::size_t index) const -> std::uint8_t
    {
        const std::array<std::size_t, 8> neighbors = get_neighbor_indices(index);
        return std::count_if(neighbors.cbegin(), neighbors.cend(),
                             [this](std::size_t neighbor)
                             { return m_cells[neighbor]; });
    }
}
