// Game.hpp
// Central game object: owns the SDL context, runs the main loop,
// and manages screen transitions via a state machine.
#pragma once

#include "core/InputHandler.hpp"
#include "core/Renderer.hpp"
#include "core/ResourceManager.hpp"
#include "core/Settings.hpp"
#include "levels/LevelLoader.hpp"
#include "screens/Screen.hpp"
#include <memory>
#include <string>

enum class GameState 
{ 
    HOME, 
    OPTIONS, 
    ENTRY_LEVEL,
    PLAYING, 
    GAME_OVER 
};

class Game 
{
    public:
        explicit Game(const std::string &title = "Bubble Shooter");
        ~Game();

        // Non-copyable.
        Game(const Game &) = delete;
        Game &operator=(const Game &) = delete;

        /// Enters the main loop. Returns when the user quits.
        void run();

        /// Transition to a new game state, constructing the appropriate Screen.
        void changeState(GameState newState, int scoreForGameOver = 0);
        void advanceStage();
        void resetStage() { m_currentStage = levels::Stage::LEARNING_1; }


    private:
        core::Renderer m_renderer;
        core::InputHandler m_input;
        core::Settings m_settings;
        core::ResourceManager m_resources;

        std::unique_ptr<screens::Screen> m_currentScreen;
        GameState m_state{GameState::HOME};
        levels::Stage m_currentStage{levels::Stage::LEARNING_1};
        bool m_running{true};

        // ── Frame timing ──────────────────────────────────────────────────────
        Uint64 m_lastTick{0};
        float m_deltaSeconds{0.f};

        void processEvents();
        void update();
        void render();
        void calcDelta();

        /// Build the screen object matching `state`.
        [[nodiscard]] std::unique_ptr<screens::Screen>
        makeScreen(GameState state, int scoreForGameOver = 0);

        [[nodiscard]] SDL_Rect viewportRect() const noexcept;
};
