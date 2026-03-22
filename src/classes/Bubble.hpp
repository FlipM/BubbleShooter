// classes/Bubble.hpp
// A single bubble that lives on the hex grid or is in-flight.
#pragma once

#include "utils/MathUtils.hpp"
#include <memory>

namespace core {
    class Renderer;
}

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
        COUNT ///< Sentinel — number of colours.
    };

    /// Maps BubbleColor to an RGB triplet for rendering.
    struct BubbleColorRGB {
        std::uint8_t r, g, b;
    };

    [[nodiscard]] BubbleColorRGB bubbleColorToRGB(BubbleColor c) noexcept;

    /// ── Bubble
    /// ───────────────────────────────────────────────────────────────────
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

            // ── State ─────────────────────────────────────────────────────────────
            [[nodiscard]] BubbleColor color() const noexcept { return m_color; }
            [[nodiscard]] utils::HexCoord gridPos() const noexcept { return m_gridPos; }
            [[nodiscard]] utils::Vec2f pixelPos() const noexcept { return m_pixelPos; }
            [[nodiscard]] bool isActive() const noexcept { return m_active; }
            [[nodiscard]] int radius() const noexcept { return m_radius; }

            void setGridPos(utils::HexCoord p) noexcept { m_gridPos = p; }
            void setPixelPos(utils::Vec2f p) noexcept { m_pixelPos = p; }

            // ── Lifecycle ─────────────────────────────────────────────────────────
            /// Mark bubble for removal and play pop animation (stub).
            void pop();

            /// True if this bubble matches the colour of another (for chain detection).
            [[nodiscard]] bool matches(const BubbleColor &color) const noexcept;

            // ── Rendering ─────────────────────────────────────────────────────────
            /// Draw the bubble at its current pixel position.
            /// @param renderer  Renderer reference for SDL abstraction.
            void draw(core::Renderer &renderer) const;

            // ── Physics / movement (in-flight) ───────────────────────────────────
            /// Move the bubble by its velocity vector (called while in-flight).
            void updateMovement(float deltaSeconds);

            /// Set launch velocity (px/s).
            void setVelocity(utils::Vec2f vel) noexcept { m_velocity = vel; }


            bool collides(const Bubble &other) const noexcept;
            
            // ── Interaction triggers (stubs) ──────────────────────────────────────
            /// Called when this bubble collides with another bubble on the grid.
            void onCollisionWithBubble(Bubble &other);

            /// Called when this bubble hits the roof.
            void onCollisionWithRoof();

            /// Called when this bubble hits a side wall (reflect).
            void onWallBounce();

        private:
            BubbleColor m_color;
            utils::HexCoord m_gridPos; ///< Position in the hex grid (-1,-1 = in-flight).
            utils::Vec2f m_pixelPos;   ///< Screen-space centre.
            utils::Vec2f m_velocity;   ///< Pixels/second while in-flight.
            int m_radius{static_cast<int>(utils::HEX_SIZE)}; ///< Radius of the bubble in pixels (for collision).
            bool m_active{true};

            // TODO: animation state (pop timer, scale, alpha)
    };

} // namespace classes
