// screens/HomeScreen.hpp
// The initial title screen with Start and Options buttons.
#pragma once

#include "Screen.hpp"
#include "core/UI.hpp"
#include <functional>
#include <string>

namespace screens 
{
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
            void render(core::Renderer &renderer) override;

        private:
            Callback m_onStart;
            Callback m_onOptions;
            SDL_Rect m_viewport;
            core::UI::Button m_startBtn;
            core::UI::Button m_optionsBtn;

            void drawBackground(core::Renderer &renderer) const;
            void drawTitle(core::Renderer &renderer) const;
            void drawButton(core::Renderer &renderer, const core::UI::Button &btn) const;
    };

} // namespace screens
