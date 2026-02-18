#pragma once

#include <array>
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

        auto step_simulation() -> void;
        auto render(SDL_Renderer *renderer) const -> void;

    private:
        std::vector<bool> m_cells;
        std::vector<bool> m_birth_mask;
        std::size_t m_width;
        std::size_t m_height;

        auto get_neighbor_indices(std::size_t index) const -> std::array<std::size_t, 8>;
        auto get_live_neighbors(std::size_t index) const -> std::uint8_t;
    };
}
