// classes/Roof.hpp
// The Roof is an anchor bar at the top of the game viewport.
// Bubbles that reach it are "attached" and joined to the grid.
#pragma once

#include "Bubble.hpp"

namespace classes 
{

    class Roof 
    {
        public:
            /// @param x, y, w, h  Screen rect of the roof bar.
            explicit Roof(int x, int y, int w, int h);
            ~Roof() = default;

            // Non-copyable, movable.
            Roof(const Roof &) = delete;
            Roof &operator=(const Roof &) = delete;
            Roof(Roof &&) = default;
            Roof &operator=(Roof &&) = default;

            // ── Getters ────────────────────────────────────────────────────────────
            [[nodiscard]] int x() const noexcept { return m_x; }
            [[nodiscard]] int y() const noexcept { return m_y; }
            [[nodiscard]] int w() const noexcept { return m_w; }
            [[nodiscard]] int h() const noexcept { return m_h; }

            // ── Utility ─────────────────────────────────────────────────────────
            [[nodiscard]] bool collides(const Bubble &bubble) const noexcept;
            void draw(core::Renderer &renderer) const;
            void attachBubble(std::shared_ptr<Bubble> bubble);

        private:
            int m_x;  ///< Top-left X coordinate.
            int m_y;  ///< Top-left Y coordinate.
            int m_w;  ///< Width in pixels.
            int m_h;  ///< Height in pixels.
    };

} // namespace classes
