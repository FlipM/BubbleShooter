// classes/Shooter.hpp
// The launcher at the bottom of the screen.
// Holds current + next bubble, tracks aim direction, fires on demand.
#pragma once

#include "Bubble.hpp"
#include "utils/MathUtils.hpp"
#include <functional>
#include <memory>
#include <set>

namespace classes 
{

    class Shooter 
    {
        public:
            /// @param basePos  Centre of the shooter (screen-space pixels).
            explicit Shooter(utils::Vec2f basePos) : m_basePos(basePos) {};
            ~Shooter() = default;

            void initiate(std::vector<classes::BubbleColor> palette);

            // ── Update ─────────────────────────────────────────────────────────────
            /// Update aim direction toward the mouse cursor position.
            /// Should be called every frame.
            void aimAt(utils::Vec2f mousePos) noexcept;

            // ── Fire ───────────────────────────────────────────────────────────────
            /// Launch the current bubble in the aimed direction.
            /// Moves `next` → `current`, generates a new `next` bubble.
            /// Returns the launched bubble (caller takes ownership to simulate flight).
            /// Returns nullptr if already a bubble in flight.
            std::unique_ptr<Bubble> shoot();

            // ── Bubble management ──────────────────────────────────────────────────
            /// Callback: request a new random bubble colour from the game.
            classes::BubbleColor randomColor();
            void removeColor(BubbleColor color) { m_upcomingColors.erase(color); }

            // ── Rendering ──────────────────────────────────────────────────────────
            void draw(SDL_Renderer *renderer) const;

            // ── Accessors ──────────────────────────────────────────────────────────
            [[nodiscard]] float aimAngle() const noexcept { return m_aimAngle; }
            [[nodiscard]] utils::Vec2f basePos() const noexcept { return m_basePos; }
            [[nodiscard]] const Bubble *current() const noexcept { return m_current.get(); }       
            [[nodiscard]] const Bubble *next() const noexcept { return m_next.get(); }

        private:
            utils::Vec2f m_basePos;
            float m_aimAngle{-1.5708f}; ///< Radians (default: straight up).

            std::unique_ptr<Bubble> m_current;
            std::unique_ptr<Bubble> m_next;
            std::set<BubbleColor> m_upcomingColors;

            int m_arrowLength{80}; ///< Length of the aim arrow in pixels.

            /// Advance: next → current, generate new next.
            void advance();

            void drawArrow(SDL_Renderer *renderer) const;
            void drawCurrent(SDL_Renderer *renderer) const;
            void drawNext(SDL_Renderer *renderer) const;
            void drawPlatform(SDL_Renderer *renderer) const;
    };

} // namespace classes
