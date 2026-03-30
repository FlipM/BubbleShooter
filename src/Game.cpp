// Game.cpp
#include "Game.hpp"
#include "screens/GameOverScreen.hpp"
#include "screens/GameScreen.hpp"
#include "screens/HomeScreen.hpp"
#include "screens/OptionsScreen.hpp"
#include "screens/EntryLevelScreen.hpp"
#include "screens/EndingScreen.hpp"
#include <SDL2/SDL.h>
#include <iostream>

#ifdef HAS_SDL2_MIXER
#include <SDL2/SDL_mixer.h>
#endif

constexpr int WINDOW_W = 1080;
constexpr int WINDOW_H = 844;
constexpr float MAX_FRAME_TIME = 0.05f;  // Prevent spiral-of-death on lag spikes.

/// Initialize the game with SDL and transition to home screen.
Game::Game(const std::string &title)
    : m_renderer(title, WINDOW_W, WINDOW_H), 
      m_resources(m_renderer, m_soundPlayer) 
{
    m_settings.load("settings.ini");
#ifdef HAS_SDL2_MIXER
    m_soundPlayer.init();
#endif
    m_lastTick = SDL_GetPerformanceCounter();

    changeState(GameState::HOME);
    processPendingStateChange();
}

/// Clean up SDL and audio resources.
Game::~Game() 
{
    m_settings.save("settings.ini");
#ifdef HAS_SDL2_MIXER
    Mix_CloseAudio();
#endif
    SDL_Quit();
}

/// Main game loop: process input, update, render.
void Game::run() 
{
    while (m_running) 
    {
        calcDelta();
        processEvents();
        update();
        render();
    }
}

/// Queue a state change to be applied at a safe point in the main loop.
void Game::changeState(GameState newState) 
{
    m_pendingState = newState;
}

/// Apply any queued state change. Only happens at handling and udpate points.
void Game::processPendingStateChange()
{
    if (!m_pendingState)
        return;

    m_state = *m_pendingState;
    m_currentScreen = makeScreen(*m_pendingState);
    m_pendingState.reset();
}

/// Handle SDL events and delegate to current screen.
void Game::processEvents() 
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) 
    {
        if (event.type == SDL_QUIT) 
        {
            m_running = false;
            return;
        }
        if (m_currentScreen)
            m_currentScreen->handleEvent(event, m_input);
    }
    processPendingStateChange();

}

/// Update the current screen with delta time.
void Game::update() 
{
    if (m_currentScreen)
        m_currentScreen->update(m_deltaSeconds);
    processPendingStateChange();

}

/// Render the current screen.
void Game::render() 
{
    m_renderer.clear();
    if (m_currentScreen)
        m_currentScreen->render(m_renderer);
    m_renderer.present();

}

/// Calculate elapsed time since last frame.
void Game::calcDelta() 
{
    const Uint64 now = SDL_GetPerformanceCounter();
    const Uint64 freq = SDL_GetPerformanceFrequency();
    m_deltaSeconds =
        static_cast<float>(now - m_lastTick) / static_cast<float>(freq);
    m_lastTick = now;
    
    if (m_deltaSeconds > MAX_FRAME_TIME)
        m_deltaSeconds = MAX_FRAME_TIME;
}

/// Create and return the appropriate screen for the given state.
std::unique_ptr<screens::Screen> Game::makeScreen(GameState state) 
{
    const SDL_Rect vp = viewportRect();
    std::clog << "[Game] transitioning to state: " << static_cast<int>(state) << "\n";

    switch (state) 
    {
        case GameState::HOME:
            return std::make_unique<screens::HomeScreen>(
                [this] { changeState(GameState::ENTRY_LEVEL); },
                [this] { changeState(GameState::OPTIONS); }, 
                vp);

        case GameState::ENTRY_LEVEL:
            return std::make_unique<screens::EntryLevelScreen>(
                [this] { changeState(GameState::PLAYING); }, 
                m_gameData, 
                vp);

        case GameState::OPTIONS:
            return std::make_unique<screens::OptionsScreen>(
                m_settings.get(), 
                [this] { processSettings(); }, 
                vp);

        case GameState::PLAYING: 
        {
            return std::make_unique<screens::GameScreen>(
                [this] { gameOver(); },
                [this] { advanceStage(); },
                m_gameData,
                m_resources,
                vp);
        }

        case GameState::GAME_OVER:
            return std::make_unique<screens::GameOverScreen>(
                [this] { changeState(GameState::ENTRY_LEVEL); },
                [this] { changeState(GameState::HOME); },
                m_gameData,
                vp);

        case GameState::GAME_ENDING:
            return std::make_unique<screens::EndingScreen>(
                [this] { changeState(GameState::HOME); }, 
                m_gameData.score.current(),
                vp);
    }
    return nullptr;
}

/// Advance to the next game stage or end sequence.
void Game::advanceStage()
{
    m_gameData.currentStage = static_cast<levels::Stage>(static_cast<int>(m_gameData.currentStage) + 1);

    if(m_gameData.currentStage >= levels::Stage::COUNT)
    {
        m_gameData.currentStage = levels::Stage::LEARNING_1;
        m_resources.play("gameWin");
        changeState(GameState::GAME_ENDING);
    }
    else
    {
        m_resources.play("stageClear");
        changeState(GameState::ENTRY_LEVEL); 
    }
}

/// Return to start on game over.
void Game::gameOver()
{
    m_gameData.currentStage = levels::Stage::LEARNING_1;
    changeState(GameState::GAME_OVER);
}

/// Apply sound settings and return to home.
void Game::processSettings()
{
    changeState(GameState::HOME);
#ifdef HAS_SDL2_MIXER
    if(m_settings.get().soundEnabled)
    {
        m_soundPlayer.setSoundVolume(1.0f);
    }
    else
    {
        m_soundPlayer.setSoundVolume(0.0f);
    }
#endif
}

/// Reset current stage to first learning level.
void Game::resetStage()
{
    m_gameData.currentStage = levels::Stage::LEARNING_1;
}

/// Get viewport rect from renderer.
SDL_Rect Game::viewportRect() const noexcept 
{
    const auto &vp = m_renderer.viewport();
    return {vp.x, vp.y, vp.width, vp.height};
}

