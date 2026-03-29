// screens/EndingScreen.hpp
// Game ending screen: shown when player completes all stages.
#pragma once

#include "Screen.hpp"
#include <functional>
#include <iostream>
#include "core/Renderer.hpp"

namespace screens 
{
    class EndingScreen final : public Screen 
    {
        public:
            using Callback = std::function<void()>;

            /// Initialize ending screen with callback and display area.
            EndingScreen(Callback onHome, int finalScore, SDL_Rect viewport);

            // ── Lifecycle Callbacks ────────────────────────────────────────
            void handleEvent(const SDL_Event &event, const core::InputHandler &input) override;
            void update(float deltaSeconds) override;
            void render(core::Renderer &renderer) override;

        private:
            static constexpr float AUTO_END_DURATION = 3.f;  ///< Seconds before automatic return to home.

            Callback m_onHome;                               ///< Callback to return to home screen.
            int m_finalScore{0};                             ///< Final score to display on ending screen.
            SDL_Rect m_viewport;                             ///< Screen display dimensions.
            float m_timer{0.f};                              ///< Elapsed time for auto-return countdown.
    };

} // namespace screens
