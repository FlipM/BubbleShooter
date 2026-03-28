// screens/GameScreen.cpp
#include "GameScreen.hpp"
#include "core/Renderer.hpp"
#include "core/UI.hpp"

namespace screens 
{

    namespace 
    {
        // Grid configuration constants.
        constexpr int GRID_COLS = 10;
        constexpr int GRID_ROWS = 14;
        constexpr int   ORIGIN_OFFSET_X = 2; // pixels from viewport edge to grid start
    } // namespace

    GameScreen::GameScreen(Callback onGameOver, Callback onAdvanceStage, levels::GameData &gameData, 
                           core::Renderer &renderer, SDL_Rect viewport)
        :   m_onGameOver(std::move(onGameOver)), 
            m_onAdvanceStage(std::move(onAdvanceStage)),
            m_gd(gameData),
            m_renderer(&renderer),
            m_viewport(viewport),
            m_grid(GRID_COLS, GRID_ROWS, utils::HEX_SIZE,  {static_cast<float>(viewport.x + ORIGIN_OFFSET_X), 
                    static_cast<float>(viewport.y + utils::ROOF_HEIGHT + 2)}),
            m_shooter({static_cast<float>(viewport.x + viewport.w / 2), static_cast<float>(viewport.y + viewport.h - 70)}),
            m_roof(viewport.x, viewport.y, viewport.w, utils::ROOF_HEIGHT),
            m_levelLoader(gameData.currentStage)
    {
        std::clog << "[GameScreen] constructed, viewport " << viewport.w << 'x' << viewport.h << '\n';
        m_levelLoader.loadLevel(m_grid);
        m_shooter.initiate(m_levelLoader.getStagePalette());
    }

    void GameScreen::handleEvent(const SDL_Event &event,
                                const core::InputHandler &input) 
    {
        if (event.type == SDL_MOUSEBUTTONUP &&
            event.button.button == SDL_BUTTON_LEFT) 
        {
            if (!m_flyingBubble) 
            { // only shoot when no bubble is in flight
                handleShoot();
            }
        }

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) 
        {
            // TODO: push a pause menu, for now toggle pause.
            m_paused = !m_paused;
        }

        (void)input; // mouse position is polled in update() via InputHandler
    }

    void GameScreen::update(float deltaSeconds) 
    {
        if (m_paused)
            return;

        if (!m_renderer)
            return;  // Renderer not yet initialized

        int mx, my;
        SDL_GetMouseState(&mx, &my);
        float logicalX, logicalY;
        SDL_RenderWindowToLogical(m_renderer->raw(), mx, my, &logicalX, &logicalY);
        m_shooter.aimAt({static_cast<float>(logicalX), static_cast<float>(logicalY)});

        // Move flying bubble.
        if (m_flyingBubble) 
        {
            if(updateFlight(deltaSeconds))
            {
                if (m_levelLoader.exceededShootLimit(m_shootcount))
                {
                    m_grid.advanceDown(m_shooter.remainingColors());
                    m_shootcount = 0; // reset shoot count after advancing grid
                }
            }
        }
        // Handle rapid-fire time limit (if applicable).
        else 
        {
            nonShootTime += deltaSeconds;
            if(m_levelLoader.exceededTimeLimit(nonShootTime))
            {
                handleShoot();
            }
        }

        // TODO: update particle effects, animations.

        if(!checkGameOver())
            checkNextLevel();
    }

    void GameScreen::render(core::Renderer &renderer) 
    {
        drawBackground(renderer);
        drawBorders(renderer);
        m_roof.draw(renderer);
        m_grid.draw(renderer);
        m_shooter.draw(renderer);
        if (m_flyingBubble)
            m_flyingBubble->draw(renderer);
        m_gd.score.draw(renderer, m_viewport.x, m_viewport.y, m_viewport.w);
    }

    int GameScreen::finalScore() const noexcept { return m_gd.score.current(); }

    void GameScreen::handleShoot() 
    {
        m_flyingBubble = m_shooter.shoot();
        m_shootcount++;
        nonShootTime = 0.f;
        if (m_flyingBubble) 
        {
            std::clog << "[GameScreen] bubble fired\n";
            // TODO: play shoot sound via audio system.
        }
    }

    bool GameScreen::updateFlight(float dt) 
    {
        m_flyingBubble->updateMovement(dt);

        const float bx = m_flyingBubble->pixelPos().x;
        const float by = m_flyingBubble->pixelPos().y;
        const int r = m_flyingBubble->radius();

        // Calculate grid boundaries (grid has +2 pixel offset from viewport)
        // Add 1 pixel buffer to prevent bubbles from landing at invalid grid cells
        const float grid_left = static_cast<float>(m_viewport.x + ORIGIN_OFFSET_X + 1);
        const float grid_right = static_cast<float>(m_viewport.x + m_viewport.w - ORIGIN_OFFSET_X - 1);

        // Wall bounce (left/right).
        if (bx - r <= grid_left || bx + r >= grid_right) 
        {
            m_flyingBubble->onWallBounce();
            // Clamp bubble position to stay within grid bounds
            float clamped_x = utils::clamp(bx, 
                                          grid_left + r,
                                          grid_right - r);
            m_flyingBubble->setPixelPos({clamped_x, by});
        }

        // Roof hit.
        if (m_roof.collides(*m_flyingBubble)) 
        {
            m_flyingBubble->onCollisionWithRoof();
            landBubble();
            return true;
        }

        utils::HexCoord flyingCoord = m_grid.snapToGrid(m_flyingBubble->pixelPos());
        std::vector<utils::HexCoord> neighbours = m_grid.neighbours(flyingCoord);
        for(const auto& neighborCoord : neighbours) 
        {
            auto neighborBubble = m_grid.at(neighborCoord);
            if (neighborBubble != nullptr && m_flyingBubble->collides(*neighborBubble)) 
            {
                m_flyingBubble->onCollisionWithBubble(*neighborBubble);
                landBubble();
                return true;
            }
        }
        return false; // still flying
    }

    void GameScreen::landBubble() 
    {
        if (!m_flyingBubble)
            return;

        auto coord = m_grid.snapToGrid(m_flyingBubble->pixelPos());
        m_grid.addBubble(std::move(m_flyingBubble), coord);

        // Check for matches.
        auto matched = m_grid.findMatches(coord);
        auto src = m_grid.at(coord);
        auto savedColor = src->color();

        if (matched.size() >= 3) 
        {
            m_gd.score.addPoints(static_cast<int>(matched.size()) * 100);
            for (auto &c : matched) 
            {
                // Needs to be sure that the bubble is still there and active before popping
                if (auto b = m_grid.removeBubble(c)) 
                {
                    b->pop();
                }
            }
            m_grid.dropFloating();
        
            
            if(!m_grid.colorExists(savedColor))
            {
                m_shooter.removeColor(savedColor);
            }
            // TODO: pop animation, sound, combo multiplier.
        }
        else
        {
            if(m_levelLoader.changeColor())
            {
                src->setColor(m_shooter.randomColor());
            }
        }

    }

    bool GameScreen::checkGameOver() 
    {
        for (int col = 0; col < GRID_COLS; ++col) 
        {
            if(m_grid.at({GRID_ROWS - 1, col})) 
            {
                std::clog << "[GameScreen] game over condition met\n";
                m_onGameOver();
                return true;
            }
        }
        return false;
    }

    void GameScreen::checkNextLevel() 
    {  
        if (m_levelLoader.isStageCleared(m_grid)) 
        {
            std::clog << "[GameScreen] stage cleared, advancing\n";
            m_onAdvanceStage();
        }
    }

    void GameScreen::drawBackground(core::Renderer &renderer) const 
    {
        renderer.drawRect(m_viewport.x, m_viewport.y, m_viewport.w, m_viewport.h,
                         core::UI::GAME_BACKGROUND);
    }

    void GameScreen::drawBorders(core::Renderer &renderer) const 
    {
        renderer.drawRectOutline(m_viewport.x, m_viewport.y, m_viewport.w, m_viewport.h,
                                core::UI::GAME_BORDER);
    }

} // namespace screens
