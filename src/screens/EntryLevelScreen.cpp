#include "EntryLevelScreen.hpp"
#include "core/Renderer.hpp"

namespace screens 
{
    /// Initialize entry screen and reset score if starting fresh level sequence.
    EntryLevelScreen::EntryLevelScreen(Callback onPlay, levels::GameData &gameData, SDL_Rect viewport)
        :   m_onPlay(std::move(onPlay)),
            m_gd(gameData),
            m_viewport(viewport)
    {
        if (m_gd.currentStage == levels::Stage::LEARNING_1)
            m_gd.score.reset();
    }

    /// Handle input events (placeholder for skip functionality).
    void EntryLevelScreen::handleEvent(const SDL_Event &event,
                                      const core::InputHandler &input) 
    {
        (void)event;  
        (void)input;   
    }

    /// Update countdown timer and auto-start gameplay when duration expires.
    void EntryLevelScreen::update(float deltaSeconds) 
    {
        m_timer += deltaSeconds;
        if (m_timer > AUTO_START_DURATION)
            m_onPlay();
    }

    /// Render stage introduction with name and description.
    void EntryLevelScreen::render(core::Renderer &renderer) 
    {
        int mx = m_viewport.x;
        int my = m_viewport.y;
        int mw = m_viewport.w;
        int mh = m_viewport.h;

        renderer.drawRect(mx, my, mw, mh, core::UI::ENTRY_LEVEL_BACKGROUND);

        std::string stageName = m_levelLoader.getStageName(m_gd.currentStage);
        renderer.drawPlate(mx + mw * core::UI::PLATE_WIDTH_DISP, 
                          my + mh * core::UI::PLATE_HEIGHT_DISP, 
                          mw * core::UI::PLATE_WIDTH, 
                          core::UI::PLATE_DEFAULT_HEIGHT, stageName, core::UI::LIGHT_BLUE,
                          core::UI::WHITE_COLOR);

        renderer.drawTextVector(m_levelLoader.getStageDescription(m_gd.currentStage),
                               mx + mw * core::UI::PLATE_WIDTH_DISP, 
                               my + (mh * core::UI::PLATE_HEIGHT_DISP * 3),
                               core::UI::WHITE_COLOR, 25);
    }

} // namespace screens
