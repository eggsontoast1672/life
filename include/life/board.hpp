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

    private:
        std::vector<bool> m_cells;
        std::vector<bool> m_birth_mask;
        std::size_t m_width;
        std::size_t m_height;

        auto position_to_index(std::size_t row, std::size_t column) const -> std::size_t;
        auto get_neighbor_indices(std::size_t index) const -> std::array<std::size_t, 8>;
        auto get_live_neighbors(std::size_t index) const -> std::uint8_t;
    };
}
