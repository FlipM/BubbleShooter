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

            /// Initialize gameplay screen with level configuration and resources.
            GameScreen(Callback onGameOver, Callback onAdvanceStage, levels::GameData &gameData, 
                       core::ResourceManager &resourceMgr, SDL_Rect viewport);

            // ── Lifecycle Callbacks ────────────────────────────────────────
            void handleEvent(const SDL_Event &event, const core::InputHandler &input) override;
            void update(float deltaSeconds) override;
            void render(core::Renderer &renderer) override;

            // ── Accessors ──────────────────────────────────────────────────
            [[nodiscard]] int finalScore() const noexcept { return m_gd.score.current(); }

        private:
            Callback m_onGameOver;                        ///< Triggered on game over condition.
            Callback m_onAdvanceStage;                    ///< Triggered when stage is complete.
            SDL_Rect m_viewport;                          ///< Playable area dimensions.
            core::ResourceManager* m_resources{nullptr};  ///< Audio resource cache.

            // ── Game Objects ───────────────────────────────────────────────
            classes::BubbleGrid m_grid;                   ///< Hex grid of bubbles.
            classes::Shooter m_shooter;                   ///< Launcher at bottom.
            classes::Roof m_roof;                         ///< Anchor barrier at top.
            levels::LevelLoader m_levelLoader;            ///< Level configuration loader.
            levels::GameData &m_gd;                       ///< Persistent game state.
            std::unique_ptr<classes::Bubble> m_flyingBubble;  ///< Currently in-flight bubble.

            // ── Game State ─────────────────────────────────────────────────
            int m_shootcount{0};                          ///< Number of shots fired in current level.
            float nonShootTime{0.f};                      ///< Time since last shot (for rapid-fire).
            bool m_paused{false};                         ///< Whether gameplay is paused.

            void handleShoot();
            bool updateFlight(float dt);
            void landBubble();
            void checkNextLevel();
            bool checkGameOver();
            
            void drawBackground(core::Renderer &renderer) const;
            void drawBorders(core::Renderer &renderer) const;
    };
} // namespace screens