// classes/Shooter.cpp
#include "Shooter.hpp"
#include "core/Renderer.hpp"
#include "core/UI.hpp"
#include <cmath>
#include <iostream>

namespace classes 
{
    classes::BubbleColor Shooter::randomColor() 
    {
        if(m_upcomingColors.empty())
            return static_cast<BubbleColor>(std::rand() % static_cast<int>(BubbleColor::COUNT));

        auto it = m_upcomingColors.begin();
        std::advance(it, std::rand() % m_upcomingColors.size());
        return *it;
    }

    void Shooter::initiate(std::vector<classes::BubbleColor> palette) 
    {
        m_upcomingColors = std::set<BubbleColor>(palette.begin(), palette.end());
        advance(); // generate initial current and next bubbles.
        advance();
    }


    void Shooter::aimAt(utils::Vec2f mousePos) noexcept 
    {
        float angle = utils::angleBetween(m_basePos, mousePos);
        // Clamp: don't allow shooting downward (below horizontal).
        constexpr float minAngle = -(utils::PI * 0.8); 
        constexpr float maxAngle = -(utils::PI * 0.2);     
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
        m_next = std::make_unique<Bubble>(randomColor());
        // Position the "next" bubble in the preview slot.
        m_next->setPixelPos({m_basePos.x + 60.f, m_basePos.y});

        if (m_current) 
        {
            m_current->setPixelPos(m_basePos);
        }
    }

    void Shooter::draw(core::Renderer &renderer) const 
    {
        drawPlatform(renderer);
        drawArrow(renderer);
        drawCurrent(renderer);
        drawNext(renderer);
    }

    void Shooter::drawArrow(core::Renderer &renderer) const
    {
        // Draw a dashed line in aim direction as a guide.
        constexpr int segments = 5;
        const float segLen = static_cast<float>(m_arrowLength) / segments;
        core::UI::Color color(255, 255, 255, 200);
        for (int i = 0; i < segments; i += 2) 
        { // dashed: draw even segments
            float t0 = i * segLen;
            float t1 = (i + 1) * segLen;
            renderer.drawLine(
                static_cast<int>(m_basePos.x + std::cos(m_aimAngle) * t0),
                static_cast<int>(m_basePos.y + std::sin(m_aimAngle) * t0),
                static_cast<int>(m_basePos.x + std::cos(m_aimAngle) * t1),
                static_cast<int>(m_basePos.y + std::sin(m_aimAngle) * t1),
                color);
        }
    // TODO: draw arrowhead at the tip.
    }

    void Shooter::drawCurrent(core::Renderer &renderer) const 
    {
        if (m_current)
            m_current->draw(renderer);
    }

    void Shooter::drawNext(core::Renderer &renderer) const 
    {
        if (m_next)
            m_next->draw(renderer);
    }

    void Shooter::drawPlatform(core::Renderer &renderer) const 
    {
        // Draw simple shooter base platform rectangle.
        renderer.drawRect(static_cast<int>(m_basePos.x - 40),
                         static_cast<int>(m_basePos.y + 20), 80, 20,
                         core::UI::Color(80, 80, 120, 255));
    }

} // namespace classes
