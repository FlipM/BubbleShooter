// classes/Bubble.hpp
// A single bubble that lives on the hex grid or is in-flight.
#pragma once

#include "utils/MathUtils.hpp"
#include "core/Renderer.hpp"

namespace classes 
{

    /// Colour palette for bubbles.
    enum class BubbleColor : std::uint8_t 
    {
        Red = 0,
        Orange,
        Yellow,
        Green,
        Blue,
        Purple,
        COUNT, ///< Sentinel — number of colours.
        Gray // Special color for bubbles that do not pop
    };

    /// Maps BubbleColor to an RGB triplet for rendering.
    struct BubbleColorRGB 
    {
        std::uint8_t r, g, b;
    };

    [[nodiscard]] BubbleColorRGB bubbleColorToRGB(BubbleColor c) noexcept;
    [[nodiscard]] BubbleColor getNextColor(BubbleColor c) noexcept;

    class Bubble 
    {
        public:
            explicit Bubble(BubbleColor color, utils::HexCoord gridPos = {-1, -1});
            ~Bubble() = default;

            // Non-copyable — bubbles are owned via unique_ptr inside BubbleGrid.
            Bubble(const Bubble &) = delete;
            Bubble &operator=(const Bubble &) = delete;
            Bubble(Bubble &&) = default;
            Bubble &operator=(Bubble &&) = default;

            // ── Getters and Setters ─────────────────────────────────────────

            [[nodiscard]] BubbleColor color() const noexcept { return m_color; }
            [[nodiscard]] utils::HexCoord gridPos() const noexcept { return m_gridPos; }
            [[nodiscard]] utils::Vec2f pixelPos() const noexcept { return m_pixelPos; }
            [[nodiscard]] bool isActive() const noexcept { return m_active; }
            [[nodiscard]] int radius() const noexcept { return m_radius; }

            void setGridPos(utils::HexCoord p) noexcept { m_gridPos = p; }
            void setPixelPos(utils::Vec2f p) noexcept { m_pixelPos = p; }
            void setColor(BubbleColor c) noexcept { m_color = c; }
            void setVelocity(utils::Vec2f vel) noexcept { m_velocity = vel; }

            // ── Functionality ────────────────────────────────────────────
            

            [[nodiscard]] bool matches(const BubbleColor &color) const noexcept;
            void pop();
            void updateMovement(float deltaSeconds);
            bool collides(const Bubble &other) const noexcept;
            void draw(core::Renderer &renderer) const;
            
            // ── Interaction triggers ──────────────────────────────────────
            
            void onCollisionWithBubble();
            void onCollisionWithRoof();
            void onWallBounce();

        private:
            BubbleColor m_color;
            utils::HexCoord m_gridPos; ///< Position in the hex grid (-1,-1 = in-flight).
            utils::Vec2f m_pixelPos;   ///< Screen-space centre.
            utils::Vec2f m_velocity;   ///< Pixels/second while in-flight.
            int m_radius{static_cast<int>(utils::HEX_SIZE)}; ///< Radius of the bubble in pixels (for collision).
            bool m_active{true};

    };

} // namespace classes
