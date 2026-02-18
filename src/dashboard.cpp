#include "life/dashboard.hpp"

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

namespace life
{
    void Dashboard::render(SDL_Renderer *renderer) const
    {
        render_step_button(renderer);
        render_simulate_button(renderer);
    }

    void Dashboard::render_step_button(SDL_Renderer *renderer) const
    {
        // TODO: Remove these magic numbers!
        SDL_FRect button_rect{0.0f, 600.0f, 300.0f, 100.0f};

        SDL_SetRenderDrawColor(renderer, 0x34, 0xc0, 0xeb, 0xff);
        SDL_RenderFillRect(renderer, &button_rect);
    }

    void Dashboard::render_simulate_button(SDL_Renderer *renderer) const
    {
        SDL_FRect button_rect{300.0f, 600.0f, 300.0f, 100.0f};

        if (m_simulating)
        {
            SDL_SetRenderDrawColor(renderer, 0xeb, 0x58, 0x34, 0xff);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 0x34, 0xeb, 0x3a, 0xff);
        }

        SDL_RenderFillRect(renderer, &button_rect);
    }
}
