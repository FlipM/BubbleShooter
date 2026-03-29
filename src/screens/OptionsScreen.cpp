// screens/OptionsScreen.cpp
#include "OptionsScreen.hpp"
#include "core/Renderer.hpp"

namespace screens 
{
    /// Initialize options screen with buttons positioned in viewport.
    OptionsScreen::OptionsScreen(core::GameSettings &settings, Callback onBack, SDL_Rect viewport)
        : m_settings(settings), 
          m_onBack(std::move(onBack)), 
          m_viewport(viewport) 
    {
        const int cx = viewport.x + viewport.w / 2;
        m_backBtn  = core::UI::Button(cx - ROW_HOR_DISPLACEMENT, 
                                      viewport.y + viewport.h - BUTTON_ROWS_FROM_BOTTOM * core::UI::BUTTON_DEFAULT_HEIGHT, 
                                      core::UI::BUTTON_DEFAULT_WIDTH, 
                                      core::UI::BUTTON_DEFAULT_HEIGHT, 
                                      "BACK");
        m_soundBtn = core::UI::Button(cx - ROW_HOR_DISPLACEMENT, 
                                      viewport.y + SOUND_BUTTON_ROW * core::UI::BUTTON_DEFAULT_HEIGHT, 
                                      core::UI::BUTTON_DEFAULT_WIDTH, 
                                      core::UI::BUTTON_DEFAULT_HEIGHT, 
                                      "");
        m_soundBtn.SetLabel(m_settings.soundEnabled ? "SOUND: ON" : "SOUND: OFF");
    }

    /// Handle mouse motion and click events for button interaction.
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

        (void)input; // Kept for interface consistency with virtual base class.
    }

    /// Update options screen logic
    void OptionsScreen::update(float /*deltaSeconds*/) 
    {
        // Placeholder for future option animations or effects.
    }

    /// Render all options screen components.
    void OptionsScreen::render(core::Renderer &renderer) 
    {
        renderer.drawRect(m_viewport.x, m_viewport.y, m_viewport.w, m_viewport.h, 
                         core::UI::DARKEST_BLUE);

        drawRow(renderer, m_soundBtn, m_settings.soundEnabled);

        renderer.drawButton(m_backBtn, core::UI::BLUE, core::UI::LIGHTEST_BLUE, 
                           core::UI::LIGHT_BLUE);
    }

    /// Draw an option row with indicator showing current value.
    void OptionsScreen::drawRow(core::Renderer &renderer, const core::UI::Button &btn, 
                                 bool value) const 
    {
        core::UI::Color indicator = value ? core::UI::SETTINGS_ON : core::UI::SETTINGS_OFF;
        renderer.drawButton(btn, indicator, core::UI::LIGHTEST_BLUE, indicator);
    }

} // namespace screens
