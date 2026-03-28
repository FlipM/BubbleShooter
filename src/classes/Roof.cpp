// classes/Roof.cpp
#include "Roof.hpp"
#include "core/Renderer.hpp"
#include "core/UI.hpp"
#include <iostream>

namespace classes {

    Roof::Roof(int x, int y, int w, int h) : m_x(x), m_y(y), m_w(w), m_h(h) {}

    bool Roof::collides(const Bubble &bubble) const noexcept 
    {
        const int by = static_cast<int>(bubble.pixelPos().y);
        const int r = bubble.radius();

        return (by - r) <= (m_y + m_h);
    }

    void Roof::attachBubble(std::shared_ptr<Bubble> bubble) 
    {
        // TODO: hand bubble off to BubbleGrid.addBubble() at the proper grid cell.
        std::clog << "[Roof] attachBubble() stub\n";
        (void)bubble; // suppress warning until implemented
    }

    void Roof::draw(core::Renderer &renderer) const 
    {
        // Draw roof as a solid bar with a subtle gradient-like effect.
        renderer.drawRect(m_x, m_y, m_w, m_h, core::UI::Color(50, 50, 80, 255));

        // Draw bright edge line.
        renderer.drawLine(m_x, m_y + m_h, m_x + m_w, m_y + m_h, 
                         core::UI::Color(120, 120, 200, 255));
    }

} // namespace classes
