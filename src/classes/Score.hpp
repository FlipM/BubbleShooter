// classes/Score.hpp
// Tracks current and high score during gameplay.
#pragma once

namespace core {
    class Renderer;
}

namespace classes {

    class Score 
    {
        public:
            Score() = default;

            void addPoints(int pts);
            void reset();

            [[nodiscard]] int current() const noexcept { return m_current; }
            [[nodiscard]] int highScore() const noexcept { return m_high; }

            /// Draw score overlay at top of game viewport.
            void draw(core::Renderer &renderer, int viewX, int viewY) const;

        private:
            int m_current{0};
            int m_high{0};

            // TODO: load/save highscore to file via Settings or ResourceManager.
    };

} // namespace classes
