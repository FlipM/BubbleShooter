// screens/HomeScreen.cpp
#include "HomeScreen.hpp"
#include "core/Renderer.hpp"

namespace screens 
{
    /// Initialize home screen with callbacks and buttons centered in viewport.
    HomeScreen::HomeScreen(Callback onStart, Callback onOptions, SDL_Rect viewport)
        : m_onStart(std::move(onStart)), m_onOptions(std::move(onOptions)),
        m_viewport(viewport) 
    {
        const int cx = viewport.x + viewport.w / 2;
        const int by = viewport.y + viewport.h / 2;

        m_startBtn = core::UI::Button(cx - core::UI::BUTTON_DEFAULT_WIDTH / 2, 
                                      by - core::UI::BUTTON_DEFAULT_HEIGHT, 
                                      core::UI::BUTTON_DEFAULT_WIDTH, 
                                      core::UI::BUTTON_DEFAULT_HEIGHT, "START");
        m_optionsBtn = core::UI::Button(cx - core::UI::BUTTON_DEFAULT_WIDTH / 2, 
                                        by + core::UI::BUTTON_DEFAULT_HEIGHT / 2, 
                                        core::UI::BUTTON_DEFAULT_WIDTH, 
                                        core::UI::BUTTON_DEFAULT_HEIGHT, "OPTIONS");
    }

    /// Handle mouse motion and click events for button interaction.
    void HomeScreen::handleEvent(const SDL_Event &event,
                                const core::InputHandler &input) 
    {
        if (event.type == SDL_MOUSEMOTION) 
        {
            int mx = event.motion.x, my = event.motion.y;
            m_startBtn.hovered = m_startBtn.contains(mx, my);
            m_optionsBtn.hovered = m_optionsBtn.contains(mx, my);
        }

        if (event.type == SDL_MOUSEBUTTONUP &&
            event.button.button == SDL_BUTTON_LEFT) 
        {
            int mx = event.button.x, my = event.button.y;
            if (m_startBtn.contains(mx, my) && m_onStart)
                m_onStart();
            if (m_optionsBtn.contains(mx, my) && m_onOptions)
                m_onOptions();
        }

        (void)input;
    }

    /// Update home screen logic 
    void HomeScreen::update(float /*deltaSeconds*/) 
    {
        // Placeholder for future animations or effects.
    }

    /// Render all home screen components.
    void HomeScreen::render(core::Renderer &renderer) 
    {
        drawBackground(renderer);
        drawTitle(renderer);
        drawButton(renderer, m_startBtn);
        drawButton(renderer, m_optionsBtn);
    }

    /// Draw the vertical gradient background.
    void HomeScreen::drawBackground(core::Renderer &renderer) const 
    {
        renderer.drawGradientBar(m_viewport.x, m_viewport.y, m_viewport.w, m_viewport.h,
                                core::UI::DARKEST_BLUE,
                                core::UI::DARK_BLUE,
                                true);
    }

    /// Draw the game title text on a colored plate.
    void HomeScreen::drawTitle(core::Renderer &renderer) const 
    {
        // Centered title at 1/4 from top.
        renderer.drawPlate(m_viewport.x + m_viewport.w / TITLE_OFFSET_X,
                            m_viewport.y + m_viewport.h / TITLE_OFFSET_Y, m_viewport.w / 2, TITLE_HEIGHT,
                            "BUBBLE SHOOTER",
                            core::UI::LIGHT_BLUE);
    }

    /// Draw a button with color change on hover.
    void HomeScreen::drawButton(core::Renderer &renderer, const core::UI::Button &btn) const 
    {
        renderer.drawButton(btn,
                           core::UI::BLUE,           // normal fill
                           core::UI::LIGHTEST_BLUE,  // outline
                           core::UI::LIGHT_BLUE);    // hover fill
    }

} // namespace screens
