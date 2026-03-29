// Game.hpp
// Central game object: manages SDL context, main loop, and screen state machine.
#pragma once

#include "core/InputHandler.hpp"
#include "core/Renderer.hpp"
#include "core/ResourceManager.hpp"
#include "core/Settings.hpp"
#include "core/SoundPlayer.hpp"
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
    GAME_OVER,
    GAME_ENDING
};

class Game 
{
    public:
        explicit Game(const std::string &title = "Bubble Shooter");
        ~Game();

        // Non-copyable.
        Game(const Game &) = delete;
        Game &operator=(const Game &) = delete;

        // ── Game Management ────────────────────────────────────────────────────
        void run();
        void changeState(GameState newState);

        // ── Getters ────────────────────────────────────────────────────────────
        [[nodiscard]] core::ResourceManager &getResourceManager() noexcept   { return m_resources; }

    private:
        core::Renderer m_renderer;
        core::InputHandler m_input;
        core::Settings m_settings;
        core::SoundPlayer m_soundPlayer;
        core::ResourceManager m_resources;
        levels::GameData m_gameData;

        std::unique_ptr<screens::Screen> m_currentScreen;
        GameState m_state{GameState::HOME};
        bool m_running{true};

        Uint64 m_lastTick{0};
        float m_deltaSeconds{0.f};

        // ── Frame Update and screen management ─────────────────────────────────
        void processEvents();
        void update();
        void render();
        void calcDelta();
        [[nodiscard]] std::unique_ptr<screens::Screen> makeScreen(GameState state);

        // ── Game Logic ─────────────────────────────────────────────────────────
        void gameOver();
        void advanceStage();
        void resetStage();
        void processSettings();

        [[nodiscard]] SDL_Rect viewportRect() const noexcept;
};
