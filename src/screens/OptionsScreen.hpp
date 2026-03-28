// screens/OptionsScreen.hpp
// Settings screen: sound toggle, brightness, etc.
#pragma once

#include "Screen.hpp"
#include "core/UI.hpp"
#include "core/Settings.hpp"
#include <functional>

namespace screens 
{
    class OptionsScreen final : public Screen 
    {
        const int ROW_HOR_DISPLACEMENT = 100;
        const int ROW_WIDTH  = 200;
        const int ROW_HEIGHT = 45;

        public:
            using Callback = std::function<void()>;

            OptionsScreen(core::GameSettings &settings, Callback onBack, SDL_Rect viewport);

            void handleEvent(const SDL_Event &event, const core::InputHandler &input) override;
            void update(float deltaSeconds) override;
            void render(core::Renderer &renderer) override;

        private:
            core::GameSettings &m_settings; ///< Non-owning ref to Game's settings.
            Callback m_onBack;
            SDL_Rect m_viewport;
            core::UI::Button m_backBtn;
            core::UI::Button m_soundBtn; 

            void drawRow(core::Renderer &renderer, const core::UI::Button &btn, bool value) const;
    };
} // namespace screens
