// classes/Bubble.cpp
#include "Bubble.hpp"
#include <cstdlib>
#include <iostream>

namespace classes 
{

    SDL_Color bubbleColorToSDL(BubbleColor c) noexcept 
    {
        switch (c) 
        {
            case BubbleColor::Red:
                return {220, 50, 50, 255};
            case BubbleColor::Orange:
                return {255, 140, 0, 255};
            case BubbleColor::Yellow:
                return {255, 220, 0, 255};
            case BubbleColor::Green:
                return {50, 200, 80, 255};
            case BubbleColor::Blue:
                return {50, 130, 220, 255};
            case BubbleColor::Purple:
                return {160, 60, 200, 255};
            default:
                return {200, 200, 200, 255};
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

    bool Bubble::matches(const Bubble &other) const noexcept 
    {
        return m_color == other.m_color;
    }

    void Bubble::draw(SDL_Renderer *renderer) const 
    {
        if (!m_active)
            return;

        SDL_Color col = bubbleColorToSDL(m_color);
        SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);

        // Draw filled circle approximation.
        const int cx = static_cast<int>(m_pixelPos.x);
        const int cy = static_cast<int>(m_pixelPos.y);
        for (int dy = -m_radius; dy <= m_radius; ++dy) {
            int dx = static_cast<int>(
                std::sqrt(static_cast<float>(m_radius * m_radius - dy * dy)));
            SDL_RenderDrawLine(renderer, cx - dx, cy + dy, cx + dx, cy + dy);
        }

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
        // TODO: clamp to viewport boundaries.
    }

} // namespace classes
