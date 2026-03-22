// classes/Roof.hpp
// The Roof is an anchor bar at the top of the game viewport.
// Bubbles that reach it are "attached" and joined to the grid.
#pragma once

#include "Bubble.hpp"
#include <memory>

namespace core {
    class Renderer;
}

namespace classes {

    class Roof 
    {
        public:
            /// @param x, y, w, h  Screen rect of the roof bar.
            Roof(int x, int y, int w, int h);
            ~Roof() = default;

            // ── Collision ─────────────────────────────────────────────────────────
            /// Returns true if bubble's pixel position overlaps the roof rect.
            [[nodiscard]] bool collides(const Bubble &bubble) const noexcept;

            // ── Attachment (stub) ─────────────────────────────────────────────────
            /// Called when an in-flight bubble reaches the roof.
            /// Transfers the bubble into the BubbleGrid at row 0.
            /// The actual grid attachment is handled by GameScreen; this is a trigger
            /// hook.
            void attachBubble(std::shared_ptr<Bubble> bubble);

            // ── Rendering ─────────────────────────────────────────────────────────
            void draw(core::Renderer &renderer) const;

            [[nodiscard]] int x() const noexcept { return m_x; }
            [[nodiscard]] int y() const noexcept { return m_y; }
            [[nodiscard]] int w() const noexcept { return m_w; }
            [[nodiscard]] int h() const noexcept { return m_h; }

        private:
            int m_x, m_y, m_w, m_h;
    };

} // namespace classes
