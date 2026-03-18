// screens/GameScreen.hpp
// The main in-game level screen.
// Owns the BubbleGrid, Shooter, Roof, Score, and in-flight bubble.
#pragma once

#include "Screen.hpp"
#include "classes/BubbleGrid.hpp"
#include "classes/Roof.hpp"
#include "classes/Score.hpp"
#include "classes/Shooter.hpp"
#include <functional>
#include <memory>

namespace screens 
{
    class GameScreen final : public Screen 
    {
        public:
            using Callback = std::function<void()>;

            /// @param onGameOver  Called when a game-over condition is met.
            /// @param viewport    Pixel rect of the game area (portrait, centred).
            GameScreen(Callback onGameOver, SDL_Rect viewport);

            void handleEvent(const SDL_Event &event,
                            const core::InputHandler &input) override;
            void update(float deltaSeconds) override;
            void render(SDL_Renderer *renderer) override;

            [[nodiscard]] int finalScore() const noexcept;

        private:
            Callback m_onGameOver;
            SDL_Rect m_viewport;

            // ── Game objects ───────────────────────────────────────────────────────
            classes::BubbleGrid m_grid;
            classes::Shooter m_shooter;
            classes::Roof m_roof;
            classes::Score m_score;
            std::unique_ptr<classes::Bubble>
                m_flyingBubble; ///< Bubble currently in flight.

            // ── State ──────────────────────────────────────────────────────────────
            bool m_paused{false};

            // ── Helpers ────────────────────────────────────────────────────────────
            void handleShoot();          ///< Trigger: fire the shooter.
            void updateFlight(float dt); ///< Move flying bubble, check collisions.
            void landBubble();           ///< Attach flying bubble to grid, check matches.
            void checkGameOver();        ///< Game-over when bubbles reach shooter row.

            void drawBackground(SDL_Renderer *renderer) const;
            void drawBorders(SDL_Renderer *renderer) const;
    };
} // namespace screens
