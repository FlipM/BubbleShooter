// screens/GameScreen.hpp
// The main in-game level screen.
// Owns the BubbleGrid, Shooter, Roof, Score, and in-flight bubble.
#pragma once

#include "Screen.hpp"
#include "classes/BubbleGrid.hpp"
#include "classes/Roof.hpp"
#include "classes/Score.hpp"
#include "classes/Shooter.hpp"
#include "levels/LevelLoader.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <functional>
#include <memory>

namespace screens 
{
    class GameScreen final : public Screen 
    {
        public:
            using Callback = std::function<void()>;

            /// @param onGameOver  Called when a game-over condition is met.
            /// @param advanceStage Called when the stage should be advanced.
            /// @param viewport    Pixel rect of the game area (portrait, centred).
            GameScreen(Callback onGameOver, Callback onAdvanceStage, levels::Stage stage, SDL_Rect viewport);

            void handleEvent(const SDL_Event &event, const core::InputHandler &input) override;
            void update(float deltaSeconds) override;
            void render(SDL_Renderer *renderer) override;

            [[nodiscard]] int finalScore() const noexcept;

        private:
            Callback m_onGameOver;
            Callback m_onAdvanceStage;
            Callback m_advanceStage;
            SDL_Rect m_viewport;

            // ── Game objects ───────────────────────────────────────────────────────
            classes::BubbleGrid m_grid;
            classes::Shooter m_shooter;
            classes::Roof m_roof;
            classes::Score m_score;
            levels::LevelLoader m_levelLoader;
            levels::Stage m_currentStage;
            std::unique_ptr<classes::Bubble>
                m_flyingBubble; ///< Bubble currently in flight.

            // ── State ──────────────────────────────────────────────────────────────
            bool m_paused{false};

            // ── Helpers ────────────────────────────────────────────────────────────
            void handleShoot();          ///< Trigger: fire the shooter.
            void updateFlight(float dt); ///< Move flying bubble, check collisions.
            void landBubble();           ///< Attach flying bubble to grid, check matches.
            void checkNextLevel();       ///< Advance stage if current level objectives are met.
            void checkGameOver();        ///< Game-over when bubbles reach shooter row.

            void drawBackground(SDL_Renderer *renderer) const;
            void drawBorders(SDL_Renderer *renderer) const;
    };
} // namespace screens
