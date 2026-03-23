// screens/HomeScreen.cpp
#include "HomeScreen.hpp"
#include "core/Renderer.hpp"

namespace screens 
{
    HomeScreen::HomeScreen(Callback onStart, Callback onOptions, SDL_Rect viewport)
        : m_onStart(std::move(onStart)), m_onOptions(std::move(onOptions)),
        m_viewport(viewport) 
    {
        const int cx = viewport.x + viewport.w / 2;
        const int bw = 200, bh = 50, gap = 20;
        const int by = viewport.y + viewport.h / 2;

        m_startBtn = core::UI::Button(cx - bw / 2, by - bh - gap / 2, bw, bh, "START");
        m_optionsBtn = core::UI::Button(cx - bw / 2, by + gap / 2, bw, bh, "OPTIONS");
    }

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

        (void)input; // kept for interface consistency
    }

    void HomeScreen::update(float /*deltaSeconds*/) 
    {
        // TODO: animate title, pulsing buttons, particle background, etc.
    }

    void HomeScreen::render(core::Renderer &renderer) 
    {
        drawBackground(renderer);
        drawTitle(renderer);
        drawButton(renderer, m_startBtn);
        drawButton(renderer, m_optionsBtn);
    }

    void HomeScreen::drawBackground(core::Renderer &renderer) const 
    {
        // Gradient simulation: vertical dark-blue to slightly lighter.
        renderer.drawGradientBar(m_viewport.x, m_viewport.y, m_viewport.w, m_viewport.h,
                                core::UI::Color(10, 10, 40, 255),
                                core::UI::Color(30, 40, 80, 255),
                                true);
    }

    void HomeScreen::drawTitle(core::Renderer &renderer) const 
    {
        // TODO: render "BUBBLE SHOOTER" with SDL2_ttf.
        // Placeholder: a simple coloured bar where the title will sit.
        renderer.drawPlate(m_viewport.x + m_viewport.w / 4,
                            m_viewport.y + m_viewport.h / 4, m_viewport.w / 2, 40,
                            "BUBBLE SHOOTER",
                            core::UI::Color(80, 200, 255, 200));
    }

    void HomeScreen::drawButton(core::Renderer &renderer, const core::UI::Button &btn) const 
    {
        renderer.drawButton(btn,
                           core::UI::Color(50, 110, 200, 255),  // fill
                           core::UI::Color(200, 230, 255, 255), // outline
                           core::UI::Color(100, 180, 255, 255)); // hover
    }

} // namespace screens
