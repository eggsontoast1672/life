#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include <SDL3/SDL_render.h>

namespace life
{
    class Board
    {
    public:
        static constexpr float CELL_WIDTH = 6.0f;
        static constexpr float CELL_HEIGHT = 6.0f;

        Board(std::size_t width, std::size_t height);

        bool is_cell_live(std::size_t row, std::size_t column) const;
        void set_cell(std::size_t row, std::size_t column, bool live);
        void render(SDL_Renderer *renderer) const;

    private:
        /// Logically, each cell in the board is either dead or alive. The reason that we use a vector
        /// of integers here instead of a vector of booleans is that the standard library has a
        /// specialization of `std::vector` for booleans which is basically a bit set, and I do not
        /// want that.
        std::vector<std::uint8_t> m_cells;
        std::size_t m_width;
        std::size_t m_height;
    };
}
