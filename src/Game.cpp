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

namespace 
{
    constexpr int WINDOW_W = 1080;
    constexpr int WINDOW_H = 844;
} // namespace

Game::Game(const std::string &title)
    : m_renderer(title, WINDOW_W, WINDOW_H), 
      m_resources(m_renderer, m_soundPlayer) 
{
    //m_settings.load("settings.ini");
#ifdef HAS_SDL2_MIXER
    m_soundPlayer.init();
#endif
    m_lastTick = SDL_GetPerformanceCounter();
    changeState(GameState::HOME);
}

Game::~Game() 
{
    //m_settings.save("settings.ini");
#ifdef HAS_SDL2_MIXER
    Mix_CloseAudio();
#endif
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

void Game::changeState(GameState newState) 
{


    m_state = newState;
    m_currentScreen = makeScreen(newState);
}

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

std::unique_ptr<screens::Screen> Game::makeScreen(GameState state) 
{
    const SDL_Rect vp = viewportRect();

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
                vp);
    }
    return nullptr; // unreachable
}

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
    return;
}

void Game::gameOver()
{
    m_gameData.currentStage = levels::Stage::LEARNING_1;
    changeState(GameState::GAME_OVER);
}

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

SDL_Rect Game::viewportRect() const noexcept 
{
    const auto &vp = m_renderer.viewport();
    return {vp.x, vp.y, vp.width, vp.height};
}
