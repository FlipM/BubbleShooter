// classes/Shooter.cpp
#include "Shooter.hpp"
#include <cmath>
#include <iostream>

namespace classes 
{
    static BubbleColor randomColor() 
    {
        return static_cast<BubbleColor>(std::rand() % static_cast<int>(BubbleColor::COUNT));
    }

    Shooter::Shooter(utils::Vec2f basePos)
        : m_basePos(basePos), m_colorProvider(randomColor) 
    {
        advance(); // fill current
        advance(); // fill next
    }

    void Shooter::aimAt(utils::Vec2f mousePos) noexcept 
    {
        float angle = utils::angleBetween(m_basePos, mousePos);
        // Clamp: don't allow shooting downward (below horizontal).
        constexpr float minAngle = -3.14159265f; // straight left
        constexpr float maxAngle = 0.f;          // straight right
        m_aimAngle = utils::clamp(angle, minAngle, maxAngle);
    }

    std::unique_ptr<Bubble> Shooter::shoot() 
    {
        if (!m_current)
            return nullptr;

        // Set launch velocity proportional to direction.
        constexpr float speed = 600.f; // px/s
        m_current->setVelocity(
            {speed * std::cos(m_aimAngle), speed * std::sin(m_aimAngle)});

        auto launched = std::move(m_current);
        advance(); // next → current, generate new next
        return launched;

        // TODO: prevent shooting again until the in-flight bubble lands.
    }

    void Shooter::advance() 
    {
        m_current = std::move(m_next);
        if (m_colorProvider) 
        {
            m_next = std::make_unique<Bubble>(m_colorProvider());
            // Position the "next" bubble in the preview slot.
            m_next->setPixelPos({m_basePos.x + 60.f, m_basePos.y});
        }
        if (m_current) 
        {
            m_current->setPixelPos(m_basePos);
        }
    }

    void Shooter::draw(SDL_Renderer *renderer) const 
    {
        drawPlatform(renderer);
        drawArrow(renderer);
        drawCurrent(renderer);
        drawNext(renderer);
    }

    void Shooter::drawArrow(SDL_Renderer *renderer) const
    {
        // Draw a dashed line in aim direction as a guide.
        constexpr int segments = 5;
        const float segLen = static_cast<float>(m_arrowLength) / segments;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
        for (int i = 0; i < segments; i += 2) 
        { // dashed: draw even segments
            float t0 = i * segLen;
            float t1 = (i + 1) * segLen;
            SDL_RenderDrawLine(
                renderer, static_cast<int>(m_basePos.x + std::cos(m_aimAngle) * t0),
                static_cast<int>(m_basePos.y + std::sin(m_aimAngle) * t0),
                static_cast<int>(m_basePos.x + std::cos(m_aimAngle) * t1),
                static_cast<int>(m_basePos.y + std::sin(m_aimAngle) * t1));
        }
    // TODO: draw arrowhead at the tip.
    }

    void Shooter::drawCurrent(SDL_Renderer *renderer) const 
    {
        if (m_current)
            m_current->draw(renderer);
    }

    void Shooter::drawNext(SDL_Renderer *renderer) const 
    {
        if (m_next)
            m_next->draw(renderer);
    }

    void Shooter::drawPlatform(SDL_Renderer *renderer) const 
    {
        // Draw simple shooter base platform rectangle.
        SDL_SetRenderDrawColor(renderer, 80, 80, 120, 255);
        SDL_Rect platform{static_cast<int>(m_basePos.x - 40),
                            static_cast<int>(m_basePos.y + 20), 80, 20};
        SDL_RenderFillRect(renderer, &platform);
    }

} // namespace classes
