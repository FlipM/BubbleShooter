#include "EntryLevelScreen.hpp"
#include "core/Renderer.hpp"

namespace screens 
{

    EntryLevelScreen::EntryLevelScreen(Callback onPlay, levels::GameData &gameData, SDL_Rect viewport)
        :   m_onPlay(std::move(onPlay)),
            m_gd(gameData),
            m_viewport(viewport)
    {
        timer = 0.f;
        if (m_gd.currentStage == levels::Stage::LEARNING_1)
            m_gd.score.reset();
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
        int mx = m_viewport.x;
        int my = m_viewport.y;
        int mw = m_viewport.w;
        int mh = m_viewport.h;

        // Dark overlay.
        renderer.drawRect(mx, my, mw, mh, core::UI::ENTRY_LEVEL_BACKGROUND);

        // Stage name.
        std::string stageName = m_levelLoader.getStageName(m_gd.currentStage);
        renderer.drawPlate(mx + mw * core::UI::PLATE_WIDTH_DISP, 
            my + mh * core::UI::PLATE_HEIGHT_DISP, 
            mw * core::UI::PLATE_WIDTH, 
            core::UI::PLATE_DEFAULT_HEIGHT, stageName, core::UI::LIGHT_BLUE,core::UI::WHITE_COLOR);

        renderer.drawTextVector(m_levelLoader.getStageDescription(m_gd.currentStage),
            mx + mw * core::UI::PLATE_WIDTH_DISP, 
            my + (mh * core::UI::PLATE_HEIGHT_DISP * 3),
            core::UI::WHITE_COLOR, 25);
    }

} // namespace screens
