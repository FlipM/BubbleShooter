// classes/Score.cpp
#include "Score.hpp"
#include "core/Renderer.hpp"
#include "core/UI.hpp"
#include <iostream>
#include <string>

namespace classes 
{

    void Score::addPoints(int pts) 
    {
        m_current += pts;
        if (m_current > m_high)
            m_high = m_current;
        // TODO: milestone triggers (e.g., every 1000 pts → bonus bubble)
        std::clog << "[Score] current=" << m_current << " high=" << m_high << '\n';

    }

    void Score::reset() { m_current = 0; }

    void Score::draw(core::Renderer &renderer, int viewX, int viewY, int viewWidth) const 
    {
        // Place the current score at the top-left, and the high score at the top-right of the game viewport.
        std::string scoreText = "Score: " + std::format("{:10d}", m_current);
        std::string highScoreText = "High Score: " + std::format("{:10d}", m_high);
        renderer.drawText(scoreText, viewX + 4, viewY + 4, core::UI::WHITE_COLOR);
        renderer.drawText(highScoreText, viewX + viewWidth - classes::Score::HIGHSCORE_WIDTH, viewY + 4, core::UI::WHITE_COLOR);
    }

} // namespace classes
