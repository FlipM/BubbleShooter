#include "EntryLevelScreen.hpp"
#include "core/Renderer.hpp"

namespace screens {

    EntryLevelScreen::EntryLevelScreen(Callback onPlay, levels::Stage initStage, SDL_Rect viewport)
        :   m_onPlay(std::move(onPlay)),
            m_initStage(initStage),
            m_viewport(viewport)
    {
        timer = 0.f;
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

    void EntryLevelScreen::render(core::Renderer &renderer) 
    {
        // Dark overlay.
        renderer.drawRect(m_viewport.x, m_viewport.y, m_viewport.w, m_viewport.h,
                         core::UI::ENTRY_LEVEL_BACKGROUND);

        // Stage name.
        std::string stageName = m_levelLoader.getStageName(m_initStage);
        renderer.drawPlate(m_viewport.x + 50, m_viewport.y + 50, m_viewport.w - 100, 100, stageName,
                          core::UI::LIGHT_BLUE,core::UI::WHITE_COLOR);

        renderer.drawTextVector(m_levelLoader.getStageDescription(m_initStage),
                            m_viewport.x + 50, m_viewport.y + 300,
                            core::UI::WHITE_COLOR, 25);
    }

} // namespace screens
