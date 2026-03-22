#include "EntryLevelScreen.hpp"

namespace screens {

    EntryLevelScreen::EntryLevelScreen(Callback onPlay, levels::Stage initStage, SDL_Rect viewport)
        :   m_onPlay(std::move(onPlay)),
            m_initStage(initStage),
            m_viewport(viewport) 
    {

    }

    void EntryLevelScreen::handleEvent(const SDL_Event &event,
                                      const core::InputHandler &input) 
    {
        
        (void)input;
    }

    void EntryLevelScreen::update(float deltaSeconds) 
    {
        // TODO: fade-in animation, score tally counter.
        timer += deltaSeconds;
        if(timer > ENTRY_LEVEL_DURATION) // auto-start after 3 seconds
            m_onPlay();
    }

    void EntryLevelScreen::render(SDL_Renderer *renderer) 
    {
        // Dark overlay.
        SDL_SetRenderDrawColor(renderer, 10, 5, 25, 240);
        SDL_RenderFillRect(renderer, &m_viewport);
    }

} // namespace screens
