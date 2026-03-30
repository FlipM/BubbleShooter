// classes/Shooter.cpp
#include "Shooter.hpp"
#include "core/UI.hpp"
#include <memory>
#include <cmath>

namespace classes 
{
    constexpr float DEFAULT_AIM_ANGLE = -utils::PI / 2.f;   // Straight up (in radians).
    constexpr int   ARROW_LENGTH = 80;                      // Length of aim guide in pixels.
    constexpr float LAUNCH_SPEED = 600.f;                   // Pixels per second.
    constexpr float MIN_AIM_ANGLE = -(utils::PI * 0.8f);    // Upper bound on aim angle.
    constexpr float MAX_AIM_ANGLE = -(utils::PI * 0.2f);    // Lower bound on aim angle.
    constexpr float NEXT_BUBBLE_OFFSET_X = 60.f;            // X offset for preview bubble.
    constexpr int   PLATFORM_WIDTH = 80;                    // Launcher platform width.
    constexpr int   PLATFORM_HEIGHT = 20;                   // Launcher platform height.
    constexpr int   PLATFORM_OFFSET_Y = 20;                 // Y offset for platform below base.
    constexpr int   ARROW_SEGMENTS = 5;                     // Number of segments in dashed aim line.

    Shooter::Shooter(utils::Vec2f basePos) 
        : m_basePos(basePos), m_aimAngle(DEFAULT_AIM_ANGLE), m_arrowLength(ARROW_LENGTH) {}

    /// Initialize the shooter with 2 bubbles. Colors come from an available color palette.
    void Shooter::initiate(std::vector<classes::BubbleColor> palette) 
    {
        m_upcomingColors = std::set<BubbleColor>(palette.begin(), palette.end());
        advance();
        advance();
    }

    /// Get a random color from the available palette.
    /// If no colors are available, selects a random color from all possible colors.
    classes::BubbleColor Shooter::randomColor() 
    {
        if(m_upcomingColors.empty())
            return static_cast<BubbleColor>(std::rand() % static_cast<int>(BubbleColor::COUNT));

        auto it = m_upcomingColors.begin();
        std::advance(it, std::rand() % m_upcomingColors.size());
        return *it;
    }

    /// Update aim direction based on mouse position.
    /// Clamps the angle to prevent shooting downward  or "infinite" shots.
    void Shooter::aimAt(utils::Vec2f mousePos) noexcept 
    {
        float angle = utils::angleBetween(m_basePos, mousePos);
        m_aimAngle = utils::clamp(angle, MIN_AIM_ANGLE, MAX_AIM_ANGLE);
    }

    /// Launch the current bubble in the aimed direction.
    /// @return The launched bubble as a unique_ptr, or nullptr if no current bubble.
    std::unique_ptr<Bubble> Shooter::shoot() 
    {
        if (!m_current)
            return nullptr;

        // Set launch velocity proportional to direction.
        m_current->setVelocity(
            {LAUNCH_SPEED * std::cos(m_aimAngle), LAUNCH_SPEED * std::sin(m_aimAngle)});

        auto launched = std::move(m_current);
        advance();
        return launched;
    }

    /// Return the set of remaining colors available in the shooter.
    std::set<classes::BubbleColor> Shooter::remainingColors() const
    {
        return m_upcomingColors;
    }

    /// Advance the bubble queue: next becomes current, a new next is generated.
    void Shooter::advance() 
    {
        m_current = std::move(m_next);
        m_next = std::make_unique<Bubble>(randomColor());
        m_next->setPixelPos({m_basePos.x + NEXT_BUBBLE_OFFSET_X, m_basePos.y});

        if (m_current) 
        {
            m_current->setPixelPos(m_basePos);
        }
    }

    /// Remove a color from the available palette.
    /// Advances to next bubble if current or next matches the removed color.
    void Shooter::removeColor(BubbleColor color) 
    {
        m_upcomingColors.erase(color);
        int count = 0;
        while(m_current && m_current->color() == color) 
        {
            advance(); // if the next bubble is of the removed color, advance to get a new next bubble.
            if (count++ > 2)
                break;
        }
        if(m_next && m_next->color() == color)
        {
            m_next->setColor(randomColor());
        }
    }

    /// Render the shooter: platform, aim guide, current bubble, and preview.
    void Shooter::draw(core::Renderer &renderer) const 
    {
        drawPlatform(renderer);
        drawArrow(renderer);
        drawCurrent(renderer);
        drawNext(renderer);
    }

    /// Draw a dashed line showing the aim direction.
    void Shooter::drawArrow(core::Renderer &renderer) const
    {
        const float segLen = static_cast<float>(m_arrowLength) / ARROW_SEGMENTS;
        for (int i = 0; i < ARROW_SEGMENTS; i += 2) 
        { 
            float t0 = i * segLen;
            float t1 = (i + 1) * segLen;
            renderer.drawLine(
                static_cast<int>(m_basePos.x + std::cos(m_aimAngle) * t0),
                static_cast<int>(m_basePos.y + std::sin(m_aimAngle) * t0),
                static_cast<int>(m_basePos.x + std::cos(m_aimAngle) * t1),
                static_cast<int>(m_basePos.y + std::sin(m_aimAngle) * t1),
                core::UI::SHOOTER_LINE);
        }
    }

    /// Draw the currently held bubble.
    void Shooter::drawCurrent(core::Renderer &renderer) const 
    {
        if (m_current)
            m_current->draw(renderer);
    }

    /// Draw the preview (next) bubble.
    void Shooter::drawNext(core::Renderer &renderer) const 
    {
        if (m_next)
            m_next->draw(renderer);
    }

    /// Draw the launcher platform base as a solid rectangle.
    void Shooter::drawPlatform(core::Renderer &renderer) const 
    {
        // Draw simple shooter base platform rectangle.
        renderer.drawRect(static_cast<int>(m_basePos.x - PLATFORM_WIDTH / 2),
                         static_cast<int>(m_basePos.y + PLATFORM_OFFSET_Y),
                         PLATFORM_WIDTH, PLATFORM_HEIGHT,
                         core::UI::SHOOTER_BASE);
    }

} // namespace classes
