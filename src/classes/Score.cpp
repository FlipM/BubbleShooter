// classes/Score.cpp
#include "Score.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <string>

namespace classes {

    void Score::addPoints(int pts) 
    {
        m_current += pts;
        if (m_current > m_high)
            m_high = m_current;
        // TODO: milestone triggers (e.g., every 1000 pts → bonus bubble)
    }

    void Score::reset() { m_current = 0; }

    void Score::draw(SDL_Renderer *renderer, int viewX, int viewY) const 
    {
        // TODO: render score text using SDL2_ttf once ResourceManager font is ready.
        // Placeholder: draw a small rectangle indicator at top of viewport.
        SDL_SetRenderDrawColor(renderer, 200, 200, 60, 200);
        SDL_Rect bar{viewX + 4, viewY + 4, m_current / 10 + 10, 8};
        SDL_RenderFillRect(renderer, &bar);

        std::clog << "[Score] current=" << m_current << " high=" << m_high << '\n';
    }

} // namespace classes
