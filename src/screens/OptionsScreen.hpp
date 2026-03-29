// screens/OptionsScreen.hpp
// Settings screen: sound toggle
#pragma once

#include "Screen.hpp"
#include "core/UI.hpp"
#include "core/Settings.hpp"
#include <functional>

namespace screens 
{
    class OptionsScreen final : public Screen 
    {
        public:
            using Callback = std::function<void()>;

            OptionsScreen(core::GameSettings &settings, Callback onBack, SDL_Rect viewport);

            // ── Lifecycle Callbacks ────────────────────────────────────────
            void handleEvent(const SDL_Event &event, const core::InputHandler &input) override;
            void update(float deltaSeconds) override;
            void render(core::Renderer &renderer) override;

        private:
            static constexpr int ROW_HOR_DISPLACEMENT = 100;  ///< Horizontal button centering offset.
            static constexpr int BUTTON_ROWS_FROM_BOTTOM = 2; ///< Back button position (rows from bottom).
            static constexpr int SOUND_BUTTON_ROW = 4;        ///< Sound button vertical position (row multiplier).

            core::GameSettings &m_settings;                    ///< Non-owning reference to game settings.
            Callback m_onBack;                                 ///< Callback when back button pressed.
            SDL_Rect m_viewport;                               ///< Screen display dimensions.
            core::UI::Button m_backBtn;                        ///< Back button.
            core::UI::Button m_soundBtn;                       ///< Sound toggle button.

            void drawRow(core::Renderer &renderer, const core::UI::Button &btn, bool value) const;
    };
} // namespace screens
