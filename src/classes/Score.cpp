// classes/Score.cpp
#include "Score.hpp"
#include "core/UI.hpp"
#include <iostream>
#include <string>
#include <format>

namespace classes 
{
    constexpr int MAX_SCORE = 99999999;
    constexpr int HIGHSCORE_WIDTH = 200;

    /// Add points to the current score, updating the high score if needed.
    void Score::addPoints(int pts) 
    {
        if (m_current + pts > MAX_SCORE) 
        {
            m_current = MAX_SCORE;
            m_high = MAX_SCORE;
            std::clog << "[Score] Maximum score reached: " << MAX_SCORE << '\n';
            return;
        }

        m_current += pts;
        if (m_current > m_high)
            m_high = m_current;
            
        std::clog << "[Score] current=" << m_current << " high=" << m_high << '\n';
    }

    /// Reset the current score to zero (high score is preserved).
    void Score::reset()
    {
        m_current = 0;
    }

    /// Render the current score and high score as overlays at the top of the viewport.
    /// Current score is placed at top-left, high score at top-right.
    void Score::draw(core::Renderer &renderer, int viewX, int viewY, int viewWidth) const 
    {
        std::string scoreText = "Score: " + std::format("{:0>8}", m_current);
        std::string highScoreText = "Hi-Score: " + std::format("{:0>8}", m_high);
        renderer.drawText(scoreText, viewX + 4, viewY + 4, core::UI::WHITE_COLOR);
        renderer.drawText(highScoreText, viewX + viewWidth - HIGHSCORE_WIDTH, viewY + 4, core::UI::WHITE_COLOR);
    }

} // namespace classes
