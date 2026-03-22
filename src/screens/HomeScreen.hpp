// screens/HomeScreen.hpp
// The initial title screen with Start and Options buttons.
#pragma once

#include "Screen.hpp"
#include <core/Renderer.hpp>
#include <functional>
#include <string>

namespace screens 
{

    struct Button 
    {
        SDL_Rect rect;
        std::string label;
        bool hovered{false};

        [[nodiscard]] bool contains(int mx, int my) const noexcept 
        {
            return mx >= rect.x && mx <= rect.x + rect.w && my >= rect.y &&
                my <= rect.y + rect.h;
        }
    };

    class HomeScreen final : public Screen 
    {
        public:
            using Callback = std::function<void()>;

            /// @param onStart    Called when user clicks Start.
            /// @param onOptions  Called when user clicks Options.
            HomeScreen(Callback onStart, Callback onOptions, SDL_Rect viewport);

            void handleEvent(const SDL_Event &event,
                            const core::InputHandler &input) override;
            void update(float deltaSeconds) override;
            void render(SDL_Renderer *renderer) override;

        private:
            Callback m_onStart;
            Callback m_onOptions;
            SDL_Rect m_viewport;
            Button m_startBtn;
            Button m_optionsBtn;

            void drawBackground(SDL_Renderer *renderer) const;
            void drawTitle(SDL_Renderer *renderer) const;
            void drawButton(SDL_Renderer *renderer, const Button &btn) const;
    };

} // namespace screens
