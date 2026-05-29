#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

#include <SDL3/SDL_render.h>

namespace life
{
    /**
     * The state of a cell on the board.
     *
     * This is not what is used under the hood to store the state of the board, it just exists to
     * provide a clearer API for changing the state.
     */
    enum class CellState
    {
        Live,
        Dead,
    };

    class Board
    {
    public:
        static constexpr float CELL_WIDTH = 6.0f;
        static constexpr float CELL_HEIGHT = 6.0f;

        Board(std::size_t width, std::size_t height);

        auto set_cell(std::size_t row, std::size_t column, CellState state) -> void;
        auto step_simulation() -> void;
        auto render(SDL_Renderer *renderer) const -> void;

        // This function is just to help with debugging, it should probably be removed.
        inline auto log_next_draw() -> void
        {
            m_is_logging = true;
        }

    private:
        std::array<std::vector<bool>, 2> m_buffers;
        std::size_t m_width;
        std::size_t m_height;

        // Somewhat unfortunately, we cannot use simple pointers to booleans here. This is the
        // tradeoff for having the fancy dynamic bitsets, we cannot get a simple contiguous array
        // of booleans from the vector.
        std::vector<bool> *m_front_buffer;
        std::vector<bool> *m_back_buffer;

        mutable bool m_is_logging = false;

        /**
         * Convert row and column indices into a linear index.
         *
         * The reason why this method accepts unsigned 32-bit integers for the row and column is
         * that we want to accept negative inputs. This is so that they wrap around properly to the
         * other side of the board. They are 32 bits wide so that their product is guaranteed to
         * fit inside an unsigned 64-bit integer.
         */
        auto position_to_index(std::int32_t row, std::int32_t column) const -> std::size_t;
        auto get_neighbor_indices(std::size_t index) const -> std::array<std::size_t, 8>;
        auto get_live_neighbors(std::size_t index) const -> std::uint8_t;
    };
}
