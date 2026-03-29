#include "EndingScreen.hpp"

namespace screens 
{
    /// Initialize ending screen and start auto-return countdown.
    EndingScreen::EndingScreen(Callback onHome, int finalScore, SDL_Rect viewport)
        :   m_onHome(std::move(onHome)),
            m_finalScore(finalScore),
            m_viewport(viewport) 
    {
        std::clog << "[EndingScreen] constructed, viewport " << viewport.w << 'x'
                  << viewport.h << '\n';
    }

    /// Handle input events (placeholder for skip functionality).
    void EndingScreen::handleEvent(const SDL_Event &event, const core::InputHandler &input) 
    {
        (void)event;     
        (void)input;  
    }

    /// Update countdown timer and auto-return to home when duration expires.
    void EndingScreen::update(float deltaSeconds) 
    {
        m_timer += deltaSeconds;
        if (m_timer > AUTO_END_DURATION)
            m_onHome();
    }

    /// Render congratulations message and thank you text.
    void EndingScreen::render(core::Renderer &renderer) 
    {
        int mx = m_viewport.x;
        int my = m_viewport.y;
        int mw = m_viewport.w;
        int mh = m_viewport.h;

        renderer.drawRect(mx, my, mw, mh, core::UI::DARKEST_BLUE);

        renderer.drawPlate(mx + mw * core::UI::PLATE_WIDTH_DISP, 
                          my + mh * core::UI::PLATE_HEIGHT_DISP,   
                          mw * core::UI::PLATE_WIDTH, 
                          core::UI::PLATE_DEFAULT_HEIGHT, 
                          "CONGRATULATIONS!", core::UI::RED);

        const std::vector<std::string> finalMessages = 
        {
            "Thank you for playing!",
            "I hope you enjoyed as much as",
            "I enjoyed making it."  
        };

        renderer.drawTextVector(finalMessages, 
                               mx + mw * core::UI::PLATE_WIDTH_DISP,
                               my + (mh * core::UI::PLATE_HEIGHT_DISP * 3), 
                               core::UI::LIGHT_BLUE, 24);

        std::string scoreText = "Final Score: " + std::to_string(m_finalScore);
        renderer.drawText(scoreText,
                         mx + mw * core::UI::PLATE_WIDTH_DISP,
                         my + (mh * core::UI::PLATE_HEIGHT_DISP * 4),
                         core::UI::WHITE_COLOR);
    }

} // namespace screens
