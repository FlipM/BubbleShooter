// screens/OptionsScreen.cpp
#include "OptionsScreen.hpp"
#include "core/Renderer.hpp"

namespace screens 
{
    OptionsScreen::OptionsScreen(core::GameSettings &settings, Callback onBack, SDL_Rect viewport)
        : m_settings(settings), 
          m_onBack(std::move(onBack)), 
          m_viewport(viewport) 
    {
        const int cx = viewport.x + viewport.w / 2;
        m_backBtn  = core::UI::Button(cx - ROW_HOR_DISPLACEMENT, viewport.y + viewport.h - 2*ROW_HEIGHT, ROW_WIDTH, ROW_HEIGHT, "BACK");
        m_soundBtn = core::UI::Button(cx - ROW_HOR_DISPLACEMENT, viewport.y + 4*ROW_HEIGHT, ROW_WIDTH, ROW_HEIGHT, "");
        m_soundBtn.SetLabel(m_settings.soundEnabled ? "SOUND: ON" : "SOUND: OFF");
    }

    void OptionsScreen::handleEvent(const SDL_Event &event, const core::InputHandler &input) 
    {
        if (event.type == SDL_MOUSEMOTION) 
        {
            m_backBtn.hovered = m_backBtn.contains(event.motion.x, event.motion.y);
            m_soundBtn.hovered = m_soundBtn.contains(event.motion.x, event.motion.y);
        }

        if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) 
        {
            if (m_backBtn.contains(event.button.x, event.button.y) && m_onBack)
                m_onBack();

            if (m_soundBtn.contains(event.button.x, event.button.y))
            {
                m_settings.soundEnabled = !m_settings.soundEnabled;
                m_soundBtn.SetLabel(m_settings.soundEnabled ? "SOUND: ON" : "SOUND: OFF");
            }
        }

        (void)input;
    }

    void OptionsScreen::update(float deltaSeconds) 
    {
        return;
    }

    void OptionsScreen::render(core::Renderer &renderer) 
    {
        // Background.
        renderer.drawRect(m_viewport.x, m_viewport.y, m_viewport.w, m_viewport.h, core::UI::DARKEST_BLUE);

        // Sound toggle row.
        drawRow(renderer, m_soundBtn, m_settings.soundEnabled);

        // Back button.
        renderer.drawButton(m_backBtn,  core::UI::BLUE, core::UI::LIGHTEST_BLUE, core::UI::LIGHT_BLUE);
    }

    void OptionsScreen::drawRow(core::Renderer &renderer, const core::UI::Button &btn, bool value) const 
    {
        // Indicator box.
        core::UI::Color indicator = value ? core::UI::SETTINGS_ON : core::UI::SETTINGS_OFF;
        renderer.drawButton(btn, indicator, core::UI::LIGHTEST_BLUE, indicator);
    }

} // namespace screens
