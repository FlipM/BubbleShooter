// classes/Bubble.cpp
#include "Bubble.hpp"
#include "core/Renderer.hpp"
#include "core/UI.hpp"
#include <cstdlib>
#include <iostream>

namespace classes 
{

    BubbleColorRGB bubbleColorToRGB(BubbleColor c) noexcept 
    {
        switch (c) 
        {
            case BubbleColor::Red:
                return {220, 50, 50};
            case BubbleColor::Orange:
                return {255, 140, 0};
            case BubbleColor::Yellow:
                return {255, 220, 0};
            case BubbleColor::Green:
                return {50, 200, 80};
            case BubbleColor::Blue:
                return {50, 130, 220};
            case BubbleColor::Purple:
                return {160, 60, 200};
            default:
                return {200, 200, 200};
        }
    }

    Bubble::Bubble(BubbleColor color, utils::HexCoord gridPos)
        : m_color(color), m_gridPos(gridPos) {}

    void Bubble::pop() 
    {
        // TODO: trigger pop animation (shrink + fade), then set m_active = false.
        m_active = false;
        std::clog << "[Bubble] pop() — color " << static_cast<int>(m_color) << '\n';
    }

    bool Bubble::matches(const BubbleColor &color) const noexcept 
    {
        return m_color == color;
    }

    void Bubble::draw(core::Renderer &renderer) const 
    {
        if (!m_active)
            return;

        BubbleColorRGB rgb = bubbleColorToRGB(m_color);
        core::UI::Color col(rgb.r, rgb.g, rgb.b, 255);
        renderer.drawCircle(static_cast<int>(m_pixelPos.x), 
                           static_cast<int>(m_pixelPos.y), 
                           m_radius, col);

        // TODO: draw bubble shine / texture on top.
    }

    void Bubble::updateMovement(float deltaSeconds) 
    {
        // TODO: integrate velocity, check wall bounce, check grid landing.
        m_pixelPos.x += m_velocity.x * deltaSeconds;
        m_pixelPos.y += m_velocity.y * deltaSeconds;
    }

    bool Bubble::collides(const Bubble &other) const noexcept 
    {        
        if (!m_active || !other.m_active)
            return false;
            
        float dx = m_pixelPos.x - other.m_pixelPos.x;
        float dy = m_pixelPos.y - other.m_pixelPos.y;
        float distanceSq = dx * dx + dy * dy;
        float radiusSum = m_radius + other.m_radius;
        return distanceSq <= radiusSum * radiusSum;
    }

    void Bubble::onCollisionWithBubble(Bubble & other) 
    {
        // TODO: snap to nearest grid cell, trigger BubbleGrid::findMatches().
        std::clog << "[Bubble] onCollisionWithBubble() stub\n";
    }

    void Bubble::onCollisionWithRoof() 
    {
        // TODO: snap to the roof row, attach to grid.
        std::clog << "[Bubble] onCollisionWithRoof() stub\n";
    }

    void Bubble::onWallBounce()
    {
        // Reflect horizontal velocity component.
        m_velocity.x = -m_velocity.x;
    }

} // namespace classes
