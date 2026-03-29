// classes/Score.hpp
// Tracks current and high score during gameplay.
#pragma once

#include "core/Renderer.hpp"

namespace classes 
{
    class Score 
    {
        public:
            Score() = default;
            ~Score() = default;

            // Non-copyable, movable.
            Score(const Score &) = delete;
            Score &operator=(const Score &) = delete;
            Score(Score &&) = default;
            Score &operator=(Score &&) = default;

            // ── Getters ────────────────────────────────────────────────────────────
            [[nodiscard]] int current() const noexcept { return m_current; }
            [[nodiscard]] int highScore() const noexcept { return m_high; }

            // ── Utility ────────────────────────────────────────────────────────────
            void addPoints(int pts);
            void reset();
            void draw(core::Renderer &renderer, int viewX, int viewY, int viewWidth) const;

        private:
            int m_current{0}; ///< Current game score.
            int m_high{0};    ///< Highest score achieved.
    };

} // namespace classes
