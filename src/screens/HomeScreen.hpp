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

            HomeScreen(Callback onStart, Callback onOptions, SDL_Rect viewport);

            // ── Lifecycle Callbacks ────────────────────────────────────────
            void handleEvent(const SDL_Event &event, const core::InputHandler &input) override;
            void update(float deltaSeconds) override;
            void render(core::Renderer &renderer) override;

        private:
            static constexpr int BUTTON_WIDTH = 200;       ///< Width of menu buttons.
            static constexpr int BUTTON_HEIGHT = 50;       ///< Height of menu buttons.
            static constexpr int BUTTON_GAP = 20;          ///< Vertical spacing between buttons.
            static constexpr int TITLE_OFFSET_X = 4;       ///< Title horizontal position divisor.
            static constexpr int TITLE_OFFSET_Y = 4;       ///< Title vertical position divisor.
            static constexpr int TITLE_HEIGHT = 40;        ///< Height of title bar.

            Callback m_onStart;                            ///< Callback when start button pressed.
            Callback m_onOptions;                          ///< Callback when options button pressed.
            SDL_Rect m_viewport;                           ///< Screen display dimensions.
            core::UI::Button m_startBtn;                   ///< Start game button.
            core::UI::Button m_optionsBtn;                 ///< Options menu button.

            void drawBackground(core::Renderer &renderer) const;
            void drawTitle(core::Renderer &renderer) const;
            void drawButton(core::Renderer &renderer, const core::UI::Button &btn) const;
    };

} // namespace screens
