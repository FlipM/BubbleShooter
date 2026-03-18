// screens/GameOverScreen.cpp
#include "GameOverScreen.hpp"

namespace screens {

    GameOverScreen::GameOverScreen(int finalScore, Callback onRetry,
                                Callback onHome, SDL_Rect viewport)
        : m_finalScore(finalScore), m_onRetry(std::move(onRetry)),
        m_onHome(std::move(onHome)), m_viewport(viewport) 
    {
        const int cx = viewport.x + viewport.w / 2;
        const int mx = viewport.y + viewport.h / 2;

        m_retryBtn = {{cx - 100, mx + 20, 200, 45}, "RETRY"};
        m_homeBtn = {{cx - 100, mx + 80, 200, 45}, "HOME"};
    }

    void GameOverScreen::handleEvent(const SDL_Event &event,
                                    const core::InputHandler &input) 
    {
        if (event.type == SDL_MOUSEMOTION) 
        {
            m_retryBtn.hovered = m_retryBtn.contains(event.motion.x, event.motion.y);
            m_homeBtn.hovered = m_homeBtn.contains(event.motion.x, event.motion.y);
        }

        if (event.type == SDL_MOUSEBUTTONUP &&
            event.button.button == SDL_BUTTON_LEFT) {
            if (m_retryBtn.contains(event.button.x, event.button.y) && m_onRetry)
            m_onRetry();
            if (m_homeBtn.contains(event.button.x, event.button.y) && m_onHome)
            m_onHome();
        }
        (void)input;
    }

    void GameOverScreen::update(float /*deltaSeconds*/) 
    {
        // TODO: fade-in animation, score tally counter.
    }

    void GameOverScreen::render(SDL_Renderer *renderer) 
    {
        // Dark overlay.
        SDL_SetRenderDrawColor(renderer, 10, 5, 25, 240);
        SDL_RenderFillRect(renderer, &m_viewport);

        // "GAME OVER" placeholder bar.
        SDL_SetRenderDrawColor(renderer, 220, 50, 50, 255);
        SDL_Rect titleBar{m_viewport.x + m_viewport.w / 4,
                            m_viewport.y + m_viewport.h / 4, m_viewport.w / 2, 40};
        SDL_RenderFillRect(renderer, &titleBar);

        // Score placeholder bar (width encodes score value).
        SDL_SetRenderDrawColor(renderer, 200, 200, 60, 200);
        SDL_Rect scoreBar{m_viewport.x + m_viewport.w / 4,
                            m_viewport.y + m_viewport.h / 4 + 60, m_finalScore / 5 + 20,
                            20};
        SDL_RenderFillRect(renderer, &scoreBar);

        // Buttons.
        auto drawBtn = [&](const Button &btn) 
        {
            SDL_Color c = btn.hovered ? SDL_Color{120, 180, 255, 255}
                                    : SDL_Color{60, 100, 200, 255};
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
            SDL_RenderFillRect(renderer, &btn.rect);
            SDL_SetRenderDrawColor(renderer, 200, 230, 255, 255);
            SDL_RenderDrawRect(renderer, &btn.rect);
            // TODO: draw button label text.
        };
        drawBtn(m_retryBtn);
        drawBtn(m_homeBtn);
    }

} // namespace screens
