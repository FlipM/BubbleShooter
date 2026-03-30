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
#include <optional>
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
        core::Renderer m_renderer;                        ///< SDL window and rendering context.
        core::InputHandler m_input;                       ///< Keyboard and mouse event polling.
        core::Settings m_settings;                        ///< Game configuration (sound, etc).
        core::SoundPlayer m_soundPlayer;                  ///< Audio playback system.
        core::ResourceManager m_resources;                ///< Sound effect cache and management.
        levels::GameData m_gameData;                      ///< Persistent game state (score, stage).

        std::unique_ptr<screens::Screen> m_currentScreen; ///< Active screen (home, gameplay, etc).
        GameState m_state{GameState::HOME};               ///< Current game state.
        bool m_running{true};                             ///< Whether main loop should continue.
        std::optional<GameState> m_pendingState;          ///< Deferred state change applied at safe loop points.

        Uint64 m_lastTick{0};                             ///< Last SDL performance counter tick.
        float m_deltaSeconds{0.f};                        ///< Frame delta time, clamped to max.

        // ── Frame Update and screen management ─────────────────────────────────
        void processEvents();
        void processPendingStateChange();

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
