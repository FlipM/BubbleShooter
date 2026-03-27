// screens/GameOverScreen.cpp
#include "GameOverScreen.hpp"
#include "core/Renderer.hpp"

namespace screens 
{

    GameOverScreen::GameOverScreen(Callback onRetry, Callback onHome, levels::GameData &gd, SDL_Rect viewport) :
            m_onRetry(std::move(onRetry)),
            m_onHome(std::move(onHome)),
            m_gd(gd),
            m_viewport(viewport) 
    {
        const int cx = viewport.x + viewport.w / 2;
        const int mx = viewport.y + viewport.h / 2;

        m_retryBtn = core::UI::Button(cx - 100, mx + 20, 200, 45, "RETRY");
        m_homeBtn = core::UI::Button(cx - 100, mx + 80, 200, 45, "HOME");
    }

    void GameOverScreen::handleEvent(const SDL_Event &event, const core::InputHandler &input) 
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

    void GameOverScreen::render(core::Renderer &renderer) 
    {
        // Dark overlay.
        renderer.drawRect(m_viewport.x, m_viewport.y, m_viewport.w, m_viewport.h,
                         core::UI::Color(10, 5, 25, 240));

        // "GAME OVER" placeholder bar.
        renderer.drawRect(m_viewport.x + m_viewport.w / 4,
                         m_viewport.y + m_viewport.h / 4, m_viewport.w / 2, 40,
                         core::UI::Color(220, 50, 50, 255));

        // Score placeholder bar (width encodes score value).
        renderer.drawRect(m_viewport.x + m_viewport.w / 4,
                         m_viewport.y + m_viewport.h / 4 + 60, m_gd.score.highScore() / 5 + 20, 20,
                         core::UI::Color(200, 200, 60, 200));

        // Buttons.
        renderer.drawButton(m_retryBtn,
                           core::UI::Color(60, 100, 200, 255),  // fill
                           core::UI::Color(200, 230, 255, 255), // outline
                           core::UI::Color(120, 180, 255, 255)); // hover
        renderer.drawButton(m_homeBtn,
                           core::UI::Color(60, 100, 200, 255),  // fill
                           core::UI::Color(200, 230, 255, 255), // outline
                           core::UI::Color(120, 180, 255, 255)); // hover
    }

} // namespace screens
