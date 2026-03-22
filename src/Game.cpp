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

namespace 
{
    constexpr int WINDOW_W = 1080;
    constexpr int WINDOW_H = 844;
} // namespace

Game::Game(const std::string &title)
    : m_renderer(title, WINDOW_W, WINDOW_H), m_resources(m_renderer.raw()) 
{
    m_settings.load("settings.ini");
    m_lastTick = SDL_GetPerformanceCounter();
    changeState(GameState::HOME);
}

Game::~Game() 
{
    m_settings.save("settings.ini");
    SDL_Quit();
}

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

void Game::changeState(GameState newState, int scoreForGameOver) 
{
    m_state = newState;
    m_currentScreen = makeScreen(newState, scoreForGameOver);
}

// ── private ──────────────────────────────────────────────────────────────────

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
}

void Game::update() 
{
    if (m_currentScreen)
        m_currentScreen->update(m_deltaSeconds);
}

void Game::render() 
{
    m_renderer.clear();
    if (m_currentScreen)
        m_currentScreen->render(m_renderer);
    m_renderer.present();
}

void Game::calcDelta() 
{
    const Uint64 now = SDL_GetPerformanceCounter();
    const Uint64 freq = SDL_GetPerformanceFrequency();
    m_deltaSeconds =
        static_cast<float>(now - m_lastTick) / static_cast<float>(freq);
    m_lastTick = now;
    // Clamp delta to avoid spiral-of-death on lag spikes.
    if (m_deltaSeconds > 0.05f)
        m_deltaSeconds = 0.05f;
}

std::unique_ptr<screens::Screen> Game::makeScreen(GameState state,
                                                  int scoreForGameOver) 
{
    const SDL_Rect vp = viewportRect();

    switch (state) 
    {
        case GameState::HOME:
            return std::make_unique<screens::HomeScreen>(
                [this] { changeState(GameState::ENTRY_LEVEL); },
                [this] { changeState(GameState::OPTIONS); }, vp);

        case GameState::ENTRY_LEVEL:
            return std::make_unique<screens::EntryLevelScreen>(
                [this] { changeState(GameState::PLAYING); }, m_currentStage, vp);

        case GameState::OPTIONS:
            return std::make_unique<screens::OptionsScreen>(
                m_settings.get(), [this] { changeState(GameState::HOME); }, vp);

        case GameState::PLAYING: 
        {
            // GameScreen calls back with the score when game is over.
            // We capture 'this' safely — screen lifetime is owned by this Game.
            auto *gs = new screens::GameScreen(
                [this] { changeState(GameState::GAME_OVER, 0); },
                [this] { advanceStage(); },
                m_currentStage,
                vp);
            return std::unique_ptr<screens::GameScreen>(gs);
        }

        case GameState::GAME_OVER:
            return std::make_unique<screens::GameOverScreen>(
                scoreForGameOver, 
                [this] { changeState(GameState::PLAYING); },
                [this] { changeState(GameState::HOME); },
                //[this] { resetStage(); },
                vp);

        case GameState::GAME_ENDING:
            return std::make_unique<screens::EndingScreen>(
                [this] { changeState(GameState::HOME); }, vp);
    }
  return nullptr; // unreachable
}

void Game::advanceStage()
{
    m_currentStage = static_cast<levels::Stage>(static_cast<int>(m_currentStage) + 1);

    if(m_currentStage >= levels::Stage::COUNT)
    {
        m_currentStage = levels::Stage::LEARNING_1;
        changeState(GameState::GAME_ENDING);
    }
    else
        changeState(GameState::ENTRY_LEVEL); 
    
    return;
}

SDL_Rect Game::viewportRect() const noexcept 
{
    const auto &vp = m_renderer.viewport();
    return {vp.x, vp.y, vp.width, vp.height};
}
