// screens/GameOverScreen.hpp
#pragma once

#include "Screen.hpp"
#include "core/UI.hpp"
#include "levels/LevelLoader.hpp"
#include <functional>

namespace screens {

    class GameOverScreen final : public Screen 
    {
        const int GAME_OVER_PLATE_HEIGHT = 40;

        public:
            using Callback = std::function<void()>;

            GameOverScreen(Callback onRetry, Callback onHome, levels::GameData &gd, SDL_Rect viewport);

            void handleEvent(const SDL_Event &event,
                            const core::InputHandler &input) override;
            void update(float deltaSeconds) override;
            void render(core::Renderer &renderer) override;

        private:
            Callback m_onRetry;
            Callback m_onHome;
            SDL_Rect m_viewport;
            levels::GameData &m_gd;
            core::UI::Button m_retryBtn;
            core::UI::Button m_homeBtn;
    };

} // namespace screens
