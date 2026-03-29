// classes/Roof.cpp
#include "Roof.hpp"
#include "core/Renderer.hpp"
#include "core/UI.hpp"
#include <iostream>

namespace classes 
{

    Roof::Roof(int x, int y, int w, int h) : m_x(x), m_y(y), m_w(w), m_h(h) {}

    /// Check if this bubble's position overlaps the roof rect.
    bool Roof::collides(const Bubble &bubble) const noexcept 
    {
        const int by = static_cast<int>(bubble.pixelPos().y);
        const int r = bubble.radius();

        return (by - r) <= (m_y + m_h);
    }

    /// Hand off a bubble to the grid when it reaches the roof.
    void Roof::attachBubble(std::shared_ptr<Bubble> bubble) 
    {
        std::clog << "[Roof] attachBubble()\n";
        (void)bubble;
    }

    /// Draw the roof as a solid bar with a bright edge line.
    void Roof::draw(core::Renderer &renderer) const 
    {
        renderer.drawRect(m_x, m_y, m_w, m_h, core::UI::ROOF_COLOR);
        renderer.drawLine(m_x, m_y + m_h, m_x + m_w, m_y + m_h, core::UI::LIGHT_BLUE);
    }

} // namespace classes
