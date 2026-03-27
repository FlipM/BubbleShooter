// screens/GameOverScreen.hpp
#pragma once

#include "Screen.hpp"
#include "levels/LevelLoader.hpp"
#include <functional>

namespace screens 
{

    constexpr float ENTRY_LEVEL_DURATION = 3.f; // seconds before auto-start

    class EntryLevelScreen final : public Screen 
    {
        public:
            using Callback = std::function<void()>;

            EntryLevelScreen(Callback onPlay, levels::GameData &gameData, SDL_Rect viewport);

            void handleEvent(const SDL_Event &event,
                            const core::InputHandler &input) override;
            void update(float deltaSeconds) override;
            void render(core::Renderer &renderer) override;

        private:
            Callback m_onPlay;
            levels::GameData &m_gd;
            SDL_Rect m_viewport;
            levels::LevelLoader m_levelLoader;
            
            float timer;
    };

} // namespace screens
