// screens/GameOverScreen.hpp
// Game over screen: shown when player loses or completes a stage.
#pragma once

#include "Screen.hpp"
#include "core/UI.hpp"
#include "levels/LevelLoader.hpp"
#include <functional>

namespace screens 
{
    class GameOverScreen final : public Screen 
    {
        public:
            using Callback = std::function<void()>;

            /// Initialize game over screen with callbacks and final score.
            GameOverScreen(Callback onRetry, Callback onHome, levels::GameData &gd, SDL_Rect viewport);

            // ── Lifecycle Callbacks ────────────────────────────────────────
            void handleEvent(const SDL_Event &event, const core::InputHandler &input) override;
            void update(float deltaSeconds) override;
            void render(core::Renderer &renderer) override;

        private:
            static constexpr int BUTTON_Y_OFFSET = 20;     ///< Vertical spacing between buttons.
            static constexpr int BUTTON_Y_OFFSET_2 = 80;   ///< Second button vertical offset.

            Callback m_onRetry;                            ///< Callback when retry button pressed.
            Callback m_onHome;                             ///< Callback when home button pressed.
            levels::GameData &m_gd;                        ///< Reference to game score.
            SDL_Rect m_viewport;                           ///< Screen display dimensions.
            core::UI::Button m_retryBtn;                   ///< Retry current level button.
            core::UI::Button m_homeBtn;                    ///< Return to home button.
    };

} // namespace screens
