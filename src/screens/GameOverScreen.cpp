// screens/GameOverScreen.cpp
#include "GameOverScreen.hpp"
#include "core/Renderer.hpp"

namespace screens 
{
    /// Initialize game over screen with buttons and display final score.
    GameOverScreen::GameOverScreen(Callback onRetry, Callback onHome, levels::GameData &gd, 
                                   SDL_Rect viewport) :
            m_onRetry(std::move(onRetry)),
            m_onHome(std::move(onHome)),
            m_gd(gd),
            m_viewport(viewport) 
    {
        const int cx = viewport.x + viewport.w / 2;
        const int my = viewport.y + viewport.h / 2;

        m_retryBtn = core::UI::Button(cx - core::UI::BUTTON_DEFAULT_WIDTH / 2, 
                                      my + BUTTON_Y_OFFSET, 
                                      core::UI::BUTTON_DEFAULT_WIDTH, 
                                      core::UI::BUTTON_DEFAULT_HEIGHT, 
                                      "RETRY");
        m_homeBtn = core::UI::Button(cx - core::UI::BUTTON_DEFAULT_WIDTH / 2, 
                                      my + BUTTON_Y_OFFSET_2, 
                                     core::UI::BUTTON_DEFAULT_WIDTH, 
                                     core::UI::BUTTON_DEFAULT_HEIGHT, 
                                     "HOME");

    }

    /// Handle mouse motion and click events for button interaction.
    void GameOverScreen::handleEvent(const SDL_Event &event, const core::InputHandler &input) 
    {
        if (event.type == SDL_MOUSEMOTION) 
        {
            m_retryBtn.hovered = m_retryBtn.contains(event.motion.x, event.motion.y);
            m_homeBtn.hovered = m_homeBtn.contains(event.motion.x, event.motion.y);
        }

        if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) 
        {
            if (m_retryBtn.contains(event.button.x, event.button.y) && m_onRetry)
                m_onRetry();
            if (m_homeBtn.contains(event.button.x, event.button.y) && m_onHome)
                m_onHome();
        }

        (void)input; // Kept for interface consistency with virtual base class.
    }

    /// Update game over screen logic
    void GameOverScreen::update(float deltaSeconds) 
    {
        (void)deltaSeconds; 
    }

    /// Render game over message, final score, and action buttons.
    void GameOverScreen::render(core::Renderer &renderer) 
    {
        int mx = m_viewport.x;
        int my = m_viewport.y;
        int mw = m_viewport.w;
        int mh = m_viewport.h;

        renderer.drawRect(mx, my, mw, mh, core::UI::ENTRY_LEVEL_BACKGROUND);

        renderer.drawPlate(mx + mw / 4,
                         my + mh / 4, mw / 2, core::UI::PLATE_DEFAULT_HEIGHT, 
                         "GAME OVER", core::UI::RED, core::UI::WHITE_COLOR);

        renderer.drawText("Score: " + std::to_string(m_gd.score.current()), 
                         mx + mw / 4, my + mh / 4 + static_cast<int>(core::UI::PLATE_DEFAULT_HEIGHT * 1.5), 
                         core::UI::WHITE_COLOR);

        renderer.drawButton(m_retryBtn, core::UI::BLUE, core::UI::LIGHTEST_BLUE, 
                           core::UI::LIGHT_BLUE); 
        renderer.drawButton(m_homeBtn, core::UI::BLUE, core::UI::LIGHTEST_BLUE, 
                           core::UI::LIGHT_BLUE); 
    }

} // namespace screens
