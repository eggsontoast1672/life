#pragma once

#include <SDL3/SDL_render.h>

namespace life
{
    class Dashboard
    {
    public:
        // Buttons for Stepping, toggling continuous simulation
        void render(SDL_Renderer *renderer) const;

    private:
        bool m_simulating = false;

        void render_step_button(SDL_Renderer *renderer) const;
        void render_simulate_button(SDL_Renderer *renderer) const;
    };
}
