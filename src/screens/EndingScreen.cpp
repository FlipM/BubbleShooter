#include "EndingScreen.hpp"

namespace screens {

    EndingScreen::EndingScreen(Callback onHome, SDL_Rect viewport)
        :   m_onHome(std::move(onHome)),
            m_viewport(viewport) 
    {
            std::clog << "[EndingScreen] constructed, viewport " << viewport.w << 'x'
                        << viewport.h << '\n';
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

    void EndingScreen::render(SDL_Renderer *renderer) 
    {
        // Dark overlay.
        SDL_SetRenderDrawColor(renderer, 10, 5, 25, 240);
        SDL_RenderFillRect(renderer, &m_viewport);
    }

} // namespace screens
