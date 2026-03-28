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
        int mx = m_viewport.x;
        int my = m_viewport.y;
        int mw = m_viewport.w;
        int mh = m_viewport.h;
        // Dark overlay.
        renderer.drawRect(mx, my, mw, mh, core::UI::DARKEST_BLUE);

        renderer.drawPlate(mx + mw * core::UI::PLATE_WIDTH_DISP, 
            my + mh * core::UI::PLATE_HEIGHT_DISP,   
            mw * core::UI::PLATE_WIDTH, 
            core::UI::PLATE_DEFAULT_HEIGHT, 
            "CONGRATULATIONS!", core::UI::RED);

        const std::vector<std::string> finalMessages = 
        {
            "Thank you for playing!",
            "I hope you enjoyed as much as",
            "I enjoyed making it."  
        };

        renderer.drawTextVector(finalMessages, 
            mx + mw * core::UI::PLATE_WIDTH_DISP,
            my + (mh * core::UI::PLATE_HEIGHT_DISP * 3), 
            core::UI::LIGHT_BLUE, 24);
    }

} // namespace screens
