// screens/GameOverScreen.hpp
#pragma once

#include "Screen.hpp"
#include "core/UI.hpp"
#include <functional>

namespace screens {

    class GameOverScreen final : public Screen 
    {
        public:
            using Callback = std::function<void()>;

            GameOverScreen(int finalScore, Callback onRetry, Callback onHome, SDL_Rect viewport);

            void handleEvent(const SDL_Event &event,
                            const core::InputHandler &input) override;
            void update(float deltaSeconds) override;
            void render(core::Renderer &renderer) override;

        private:
            int m_finalScore;
            Callback m_onRetry;
            Callback m_onHome;
            SDL_Rect m_viewport;
            core::UI::Button m_retryBtn;
            core::UI::Button m_homeBtn;
    };

} // namespace screens
