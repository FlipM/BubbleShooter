// screens/GameOverScreen.hpp
#pragma once

#include "Screen.hpp"
#include <functional>
#include <iostream>

namespace screens 
{

    constexpr float ENDING_DURATION = 3.f; // seconds before auto-start

    class EndingScreen final : public Screen 
    {
        public:
            using Callback = std::function<void()>;

            EndingScreen(Callback onHome, SDL_Rect viewport);

            void handleEvent(const SDL_Event &event, const core::InputHandler &input) override;
            void update(float deltaSeconds) override;
            void render(SDL_Renderer *renderer) override;

        private:
            Callback m_onHome;
            SDL_Rect m_viewport;
            float timer;
    };

} // namespace screens
