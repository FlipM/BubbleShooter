// classes/Roof.cpp
#include "Roof.hpp"
#include <iostream>

namespace classes {

    Roof::Roof(int x, int y, int w, int h) : m_rect{x, y, w, h} {}

    bool Roof::collides(const Bubble &bubble) const noexcept 
    {
        const int bx = static_cast<int>(bubble.pixelPos().x);
        const int by = static_cast<int>(bubble.pixelPos().y);
        const int r = bubble.radius();

        return (by - r) <= (m_rect.y + m_rect.h);
    }

    void Roof::attachBubble(std::shared_ptr<Bubble> bubble) 
    {
        // TODO: hand bubble off to BubbleGrid.addBubble() at the proper grid cell.
        std::clog << "[Roof] attachBubble() stub\n";
        (void)bubble; // suppress warning until implemented
    }

    void Roof::draw(SDL_Renderer *renderer) const 
    {
        // Draw roof as a solid bar with a subtle gradient-like effect.
        SDL_SetRenderDrawColor(renderer, 50, 50, 80, 255);
        SDL_RenderFillRect(renderer, &m_rect);

        // Draw bright edge line.
        SDL_SetRenderDrawColor(renderer, 120, 120, 200, 255);
        SDL_RenderDrawLine(renderer, m_rect.x, m_rect.y + m_rect.h,
                            m_rect.x + m_rect.w, m_rect.y + m_rect.h);
    }

} // namespace classes
