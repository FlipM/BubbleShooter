// classes/Shooter.hpp
// The launcher at the bottom of the screen.
// Holds current + next bubble, tracks aim direction, fires on demand.
#pragma once

#include "Bubble.hpp"
#include "utils/MathUtils.hpp"
#include "core/Renderer.hpp"
#include <set>

namespace core 
{
    class Renderer;
}

namespace classes 
{

    class Shooter 
    {
        public:
            /// @param basePos  Centre of the shooter (screen-space pixels).
            explicit Shooter(utils::Vec2f basePos);
            ~Shooter() = default;

            // Non-copyable, movable.
            Shooter(const Shooter &) = delete;
            Shooter &operator=(const Shooter &) = delete;
            Shooter(Shooter &&) = default;
            Shooter &operator=(Shooter &&) = default;

            // ── Getters ────────────────────────────────────────────────────────────
            [[nodiscard]] float aimAngle() const noexcept { return m_aimAngle; }
            [[nodiscard]] utils::Vec2f basePos() const noexcept { return m_basePos; }
            [[nodiscard]] const Bubble *current() const noexcept { return m_current.get(); }       
            [[nodiscard]] const Bubble *next() const noexcept { return m_next.get(); }
            [[nodiscard]] std::set<classes::BubbleColor> remainingColors() const;

            // ── Aiming and Shooting ────────────────────────────────────────────────
            void aimAt(utils::Vec2f mousePos) noexcept;
            std::unique_ptr<Bubble> shoot();

            // ── Utility ────────────────────────────────────────────────────────────
            void initiate(std::vector<classes::BubbleColor> palette);
            classes::BubbleColor randomColor();
            void removeColor(BubbleColor color);
            void draw(core::Renderer &renderer) const;

        private:
            utils::Vec2f m_basePos;                    ///< Screen-space position of launcher base.
            float m_aimAngle;                          ///< Aim direction in radians.
            std::unique_ptr<Bubble> m_current;         ///< Currently held bubble.
            std::unique_ptr<Bubble> m_next;            ///< Preview of next bubble to shoot.
            std::set<BubbleColor> m_upcomingColors;    ///< Available colors for new bubbles.
            int m_arrowLength;                         ///< Length of aim guide in pixels.

            void advance();
            void drawArrow(core::Renderer &renderer) const;
            void drawCurrent(core::Renderer &renderer) const;
            void drawNext(core::Renderer &renderer) const;
            void drawPlatform(core::Renderer &renderer) const;
    };

} // namespace classes
