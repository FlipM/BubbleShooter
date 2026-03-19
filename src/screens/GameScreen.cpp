// screens/GameScreen.cpp
#include "GameScreen.hpp"
#include <SDL2/SDL.h>
#include <iostream>

namespace screens 
{

    namespace 
    {
        // Grid configuration constants.
        constexpr int GRID_COLS = 10;
        constexpr int GRID_ROWS = 14;
    } // namespace

    GameScreen::GameScreen(Callback onGameOver, SDL_Rect viewport)
        : m_onGameOver(std::move(onGameOver)), m_viewport(viewport),
        m_grid(GRID_COLS, GRID_ROWS, utils::HEX_SIZE,
                {static_cast<float>(viewport.x + 2),
                static_cast<float>(viewport.y + utils::ROOF_HEIGHT + 2)}),
        m_shooter({static_cast<float>(viewport.x + viewport.w / 2),
                    static_cast<float>(viewport.y + viewport.h - 70)}),
        m_roof(viewport.x, viewport.y, viewport.w, utils::ROOF_HEIGHT) 
    {
        // TODO: populate initial grid with random bubbles.
        std::clog << "[GameScreen] constructed, viewport " << viewport.w << 'x'
                    << viewport.h << '\n';
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

        // Update shooter aim toward current mouse position.
        // (SDL_GetMouseState used directly as a simple placeholder.)
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        m_shooter.aimAt({static_cast<float>(mx), static_cast<float>(my)});

        // Move flying bubble.
        if (m_flyingBubble) 
        {
            updateFlight(deltaSeconds);
        }

        // TODO: advance grid down at timed intervals.
        // TODO: update particle effects, animations.

        checkGameOver();
    }

    void GameScreen::render(SDL_Renderer *renderer) 
    {
        drawBackground(renderer);
        drawBorders(renderer);
        m_roof.draw(renderer);
        m_grid.draw(renderer);
        m_shooter.draw(renderer);
        if (m_flyingBubble)
            m_flyingBubble->draw(renderer);
        m_score.draw(renderer, m_viewport.x, m_viewport.y);
    }

    int GameScreen::finalScore() const noexcept { return m_score.current(); }

    void GameScreen::handleShoot() 
    {
        m_flyingBubble = m_shooter.shoot();
        if (m_flyingBubble) 
        {
            std::clog << "[GameScreen] bubble fired\n";
            // TODO: play shoot sound via audio system.
        }
    }

    void GameScreen::updateFlight(float dt) 
    {
        m_flyingBubble->updateMovement(dt);

        const float bx = m_flyingBubble->pixelPos().x;
        const float by = m_flyingBubble->pixelPos().y;
        const int r = m_flyingBubble->radius();

        // Wall bounce (left/right).
        if (bx - r <= m_viewport.x || bx + r >= m_viewport.x + m_viewport.w) 
        {
            m_flyingBubble->onWallBounce();
        }

        // Roof hit.
        if (m_roof.collides(*m_flyingBubble)) 
        {
            m_flyingBubble->onCollisionWithRoof();
            landBubble();
            return;
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
                return;
            }
        }
    }

    void GameScreen::landBubble() 
    {
        if (!m_flyingBubble)
            return;

        auto coord = m_grid.snapToGrid(m_flyingBubble->pixelPos());
        m_grid.addBubble(std::move(m_flyingBubble), coord);

        // Check for matches.
        auto matched = m_grid.findMatches(coord);
        if (matched.size() >= 3) 
        {
            m_score.addPoints(static_cast<int>(matched.size()) * 10);
            for (auto &c : matched) 
            {
                // Needs to be sure that the bubble is still there and active before popping
                if (auto b = m_grid.removeBubble(c)) 
                {
                    b->pop();
                }
            }
            m_grid.dropFloating();
            // TODO: pop animation, sound, combo multiplier.
        }
    }

    void GameScreen::checkGameOver() 
    {
        // Game over if any bubble in the last row reaches the shooter area.
        // TODO: implement proper check against grid rows vs shooter position.
    }

    void GameScreen::drawBackground(SDL_Renderer *renderer) const 
    {
        SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
        SDL_RenderFillRect(renderer, &m_viewport);
    }

    void GameScreen::drawBorders(SDL_Renderer *renderer) const 
    {
        SDL_SetRenderDrawColor(renderer, 80, 80, 130, 255);
        SDL_RenderDrawRect(renderer, &m_viewport);
    }

} // namespace screens
