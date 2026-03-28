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
        int mx = m_viewport.x,
            my = m_viewport.y,
            mw = m_viewport.w,
            mh = m_viewport.h;
        // Dark overlay.
        renderer.drawRect(mx, my, mw, mh, core::UI::ENTRY_LEVEL_BACKGROUND);

        // "GAME OVER" placeholder bar.
        renderer.drawPlate(mx + mw / 4,
                         my + mh / 4, mw / 2, GAME_OVER_PLATE_HEIGHT, 
                         "GAME OVER", core::UI::RED, core::UI::WHITE_COLOR);

        // Score placeholder bar (width encodes score value).
        renderer.drawText("Score: " + std::to_string(m_gd.score.current()), 
            mx + mw / 4, my + mh / 4  + 2*GAME_OVER_PLATE_HEIGHT, core::UI::WHITE_COLOR);
        

        // Buttons.
        renderer.drawButton(m_retryBtn, core::UI::BLUE, core::UI::LIGHTEST_BLUE, core::UI::LIGHT_BLUE); 
        renderer.drawButton(m_homeBtn,  core::UI::BLUE, core::UI::LIGHTEST_BLUE, core::UI::LIGHT_BLUE); 
    }

} // namespace screens
