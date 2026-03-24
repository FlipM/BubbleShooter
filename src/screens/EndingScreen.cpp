#include "EndingScreen.hpp"
#include "core/Renderer.hpp"

namespace screens {

    EndingScreen::EndingScreen(Callback onHome, SDL_Rect viewport)
        :   m_onHome(std::move(onHome)),
            m_viewport(viewport) 
    {
            std::clog << "[EndingScreen] constructed, viewport " << viewport.w << 'x'
                        << viewport.h << '\n';
            timer = 0.f;
    }

    void EndingScreen::handleEvent(const SDL_Event &event, const core::InputHandler &input) 
    {
        (void)input;
    }

    void EndingScreen::update(float deltaSeconds) 
    {
        // TODO: fade-in animation, score tally counter.
        timer += deltaSeconds;
        if(timer > ENDING_DURATION) // auto-start after 3 seconds
            m_onHome();
    }

    void EndingScreen::render(core::Renderer &renderer) 
    {
        // Dark overlay.
        renderer.drawRect(m_viewport.x, m_viewport.y, m_viewport.w, m_viewport.h,
                         core::UI::Color(10, 5, 25, 240));
    }

} // namespace screens
