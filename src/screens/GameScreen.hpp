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
#include "core/ResourceManager.hpp"
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
            /// @param resourceMgr ResourceManager for audio playback.
            GameScreen(Callback onGameOver, Callback onAdvanceStage, levels::GameData &gameData, 
                       core::ResourceManager &resourceMgr, SDL_Rect viewport);

            void handleEvent(const SDL_Event &event, const core::InputHandler &input) override;
            void update(float deltaSeconds) override;
            void render(core::Renderer &renderer) override;

            [[nodiscard]] int finalScore() const noexcept;

        private:
            Callback m_onGameOver;
            Callback m_onAdvanceStage;
            SDL_Rect m_viewport;
            core::ResourceManager* m_resources{nullptr};

            // ── Game objects ───────────────────────────────────────────────────────
            classes::BubbleGrid m_grid;
            classes::Shooter m_shooter;
            classes::Roof m_roof;
            levels::LevelLoader m_levelLoader;            
            levels::GameData &m_gd;
            std::unique_ptr<classes::Bubble> m_flyingBubble; 

            // ── Game values ────────────────────────────────────────────────────────
            int m_shootcount{0};
            float nonShootTime{0.f}; // Time elapsed since last shot, for rapid-fire levels.

            // ── State ──────────────────────────────────────────────────────────────
            bool m_paused{false};

            // ── Helpers ────────────────────────────────────────────────────────────
            void handleShoot();          ///< Trigger: fire the shooter.
            bool updateFlight(float dt); ///< Move flying bubble, check collisions. True if bubble landed.
            void landBubble();           ///< Attach flying bubble to grid, check matches.
            void checkNextLevel();       ///< Advance stage if current level objectives are met.
            bool checkGameOver();        ///< Game-over when bubbles reach shooter row.

            void drawBackground(core::Renderer &renderer) const;
            void drawBorders(core::Renderer &renderer) const;
    };
} // namespace screens