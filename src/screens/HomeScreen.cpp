// screens/HomeScreen.cpp
#include "HomeScreen.hpp"
#include <SDL2/SDL.h>

namespace screens 
{
    HomeScreen::HomeScreen(Callback onStart, Callback onOptions, SDL_Rect viewport)
        : m_onStart(std::move(onStart)), m_onOptions(std::move(onOptions)),
        m_viewport(viewport) 
    {
        const int cx = viewport.x + viewport.w / 2;
        const int bw = 200, bh = 50, gap = 20;
        const int by = viewport.y + viewport.h / 2;

        m_startBtn = {{cx - bw / 2, by - bh - gap / 2, bw, bh}, "START"};
        m_optionsBtn = {{cx - bw / 2, by + gap / 2, bw, bh}, "OPTIONS"};
    }

    void HomeScreen::handleEvent(const SDL_Event &event,
                                const core::InputHandler &input) 
    {
        if (event.type == SDL_MOUSEMOTION) 
        {
            int mx = event.motion.x, my = event.motion.y;
            m_startBtn.hovered = m_startBtn.contains(mx, my);
            m_optionsBtn.hovered = m_optionsBtn.contains(mx, my);
        }

        if (event.type == SDL_MOUSEBUTTONUP &&
            event.button.button == SDL_BUTTON_LEFT) 
        {
            int mx = event.button.x, my = event.button.y;
            if (m_startBtn.contains(mx, my) && m_onStart)
            m_onStart();
            if (m_optionsBtn.contains(mx, my) && m_onOptions)
            m_onOptions();
        }

        (void)input; // kept for interface consistency
    }

    void HomeScreen::update(float /*deltaSeconds*/) 
    {
        // TODO: animate title, pulsing buttons, particle background, etc.
    }

    void HomeScreen::render(SDL_Renderer *renderer) 
    {
        drawBackground(renderer);
        drawTitle(renderer);
        drawButton(renderer, m_startBtn);
        drawButton(renderer, m_optionsBtn);
    }

    void HomeScreen::drawBackground(SDL_Renderer *renderer) const 
    {
        // Gradient simulation: vertical dark-blue to slightly lighter.
        for (int dy = 0; dy < m_viewport.h; ++dy) 
        {
            float t = static_cast<float>(dy) / m_viewport.h;
            SDL_SetRenderDrawColor(renderer, static_cast<Uint8>(10 + t * 20),
                                static_cast<Uint8>(10 + t * 30),
                                static_cast<Uint8>(40 + t * 40), 255);
            SDL_RenderDrawLine(renderer, m_viewport.x, m_viewport.y + dy,
                            m_viewport.x + m_viewport.w, m_viewport.y + dy);
        }
    }

    void HomeScreen::drawTitle(SDL_Renderer *renderer) const 
    {
        // TODO: render "BUBBLE SHOOTER" with SDL2_ttf.
        // Placeholder: a simple coloured bar where the title will sit.
        SDL_SetRenderDrawColor(renderer, 80, 200, 255, 200);
        SDL_Rect titleBar{m_viewport.x + m_viewport.w / 4,
                            m_viewport.y + m_viewport.h / 4, m_viewport.w / 2, 40};
        SDL_RenderFillRect(renderer, &titleBar);
    }

    void HomeScreen::drawButton(SDL_Renderer *renderer, const Button &btn) const 
    {
        // Fill: lighter when hovered.
        SDL_Color fill = btn.hovered ? SDL_Color{100, 180, 255, 255}
                                    : SDL_Color{50, 110, 200, 255};
        SDL_SetRenderDrawColor(renderer, fill.r, fill.g, fill.b, fill.a);
        SDL_RenderFillRect(renderer, &btn.rect);

        // Outline.
        SDL_SetRenderDrawColor(renderer, 200, 230, 255, 255);
        SDL_RenderDrawRect(renderer, &btn.rect);

        // TODO: draw btn.label text with SDL2_ttf.
    }

} // namespace screens
