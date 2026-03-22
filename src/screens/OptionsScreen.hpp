// screens/OptionsScreen.hpp
// Settings screen: sound toggle, brightness, etc.
#pragma once

#include "HomeScreen.hpp" // for Button struct reuse
#include "Screen.hpp"
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
            void render(SDL_Renderer *renderer) override;

        private:
            core::GameSettings &m_settings; ///< Non-owning ref to Game's settings.
            Callback m_onBack;
            SDL_Rect m_viewport;
            Button m_backBtn;
            Button m_soundBtn; ///< Toggles sound on/off.

            // TODO: sliders for brightness, volume.

            void drawRow(SDL_Renderer *renderer, const char *label,
                const SDL_Rect &rowRect, bool value) const;
    };
} // namespace screens
