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
        public:
            using Callback = std::function<void()>;

            OptionsScreen(core::GameSettings &settings, Callback onBack,
                            SDL_Rect viewport);

            void handleEvent(const SDL_Event &event,
                            const core::InputHandler &input) override;
            void update(float deltaSeconds) override;
            void render(core::Renderer &renderer) override;

        private:
            core::GameSettings &m_settings; ///< Non-owning ref to Game's settings.
            Callback m_onBack;
            SDL_Rect m_viewport;
            core::UI::Button m_backBtn;
            core::UI::Button m_soundBtn; ///< Toggles sound on/off.

            // TODO: sliders for brightness, volume.

            void drawRow(core::Renderer &renderer, const char *label,
                int x, int y, int w, int h, bool value) const;
    };
} // namespace screens
