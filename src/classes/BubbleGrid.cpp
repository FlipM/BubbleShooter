// classes/BubbleGrid.cpp
#include "BubbleGrid.hpp"
#include <cmath>
#include <iostream>
#include <queue>
#include <unordered_set>

namespace classes 
{

    BubbleGrid::BubbleGrid(int cols, int rows, float hexSize, utils::Vec2f origin)
        : m_cols(cols), m_rows(rows), m_hexSize(hexSize), m_origin(origin) 
    {
        m_grid.assign(rows, std::vector<std::shared_ptr<Bubble>>(cols, nullptr));
        visited.assign(rows, std::vector<short>(cols, 0));
    }

    void BubbleGrid::addBubble(std::unique_ptr<Bubble> bubble,
                            utils::HexCoord pos) 
    {
        if (pos.r < 0 || pos.r >= m_rows || pos.c < 0 || pos.c >= m_cols)
            return;

        // Special case: 

        bubble->setGridPos(pos);
        bubble->setPixelPos(cellCenter(pos));
        m_grid[pos.r][pos.c] = std::move(bubble);
    }

    std::shared_ptr<Bubble> BubbleGrid::removeBubble(utils::HexCoord pos) 
    {
        return std::exchange(m_grid[pos.r][pos.c], nullptr);
    }

    std::shared_ptr<Bubble> BubbleGrid::at(utils::HexCoord pos) const 
    {
        if (pos.r < 0 || pos.r >= m_rows || pos.c < 0 || pos.c >= m_cols)
            return nullptr;
        return m_grid[pos.r][pos.c];
    }

    bool BubbleGrid::isValidBubble(std::shared_ptr<Bubble> bubblePtr) const
    {
        return bubblePtr && bubblePtr->isActive();
    }

    classes::BubbleColor BubbleGrid::getRandomColor(const std::vector<classes::BubbleColor> &palette) const
    {        
        int randomIndex = rand() % palette.size();
        return static_cast<classes::BubbleColor>(palette[randomIndex]);
    }

    std::vector<utils::HexCoord> BubbleGrid::findMatches(utils::HexCoord origin) 
    {
        // TODO: BFS collecting same-colour connected bubbles.
        // Returns coords of matched group (≥3 → pop them).
        auto src = at(origin);
        if (!src)
            return {};

        std::vector<utils::HexCoord> matched;

        classes::BubbleColor color = src->color();
        visited[origin.r][origin.c] = 1;
        matched = recMatches(origin, color);
        clearVisited();
        return matched;
    }

    std::vector<utils::HexCoord> BubbleGrid::recMatches(utils::HexCoord origin, BubbleColor &color)
    {
        std::vector<utils::HexCoord> matched;
        matched.push_back(origin);
        visited[origin.r][origin.c] = 1;

        std::vector<utils::HexCoord> neighbours = this->neighbours(origin);
        for(const auto& neighborCoord : neighbours) 
        {
            if (isVisited(neighborCoord))
                continue;

            auto neighborBubble = this->at(neighborCoord);
            if(isValidBubble(neighborBubble) && neighborBubble->matches(color))
            {
                auto subMatches = recMatches(neighborCoord, color);
                matched.insert(matched.end(), subMatches.begin(), subMatches.end());
            }
        }
        return matched;
    }

    void BubbleGrid::dropFloating() 
    {
        for(short c = 0; c < m_cols; c++)
        {
            if(!isVisited({0, c}))
            {
                auto bubble = at({0, c});
                if(isValidBubble(bubble))
                {
                    dfsVisited(bubble->gridPos());
                }
            }

        }

        for(short r = 0; r < m_rows; r++)
        {
            for(short c = 0; c < m_cols; c++)
            {
                if(!isVisited({r, c}) && at({r, c}))
                    removeBubble({r, c})->pop();
            }
        }
        clearVisited();
    }

    void BubbleGrid::dfsVisited(utils::HexCoord pos)
    {
        visited[pos.r][pos.c] = 1;

        std::vector<utils::HexCoord> neighbours = this->neighbours(pos);
        for(const auto& neighborCoord : neighbours) 
        {
            if (isVisited(neighborCoord))
                continue;

            auto neighborBubble = this->at(neighborCoord);
            if(isValidBubble(neighborBubble))
            {
                dfsVisited(neighborCoord);
            }
        }
        return;
    }

    utils::HexCoord BubbleGrid::snapToGrid(utils::Vec2f pixelPos) const 
    {
        // TODO: find nearest empty hex cell to pixelPos.
        return utils::pixelToHex(pixelPos, m_origin);
    }

    void BubbleGrid::advanceDown() 
    {
        // TODO: shift all rows down by one, add a new row at the top.
        std::clog << "[BubbleGrid] advanceDown() stub\n";
    }

    void BubbleGrid::draw(SDL_Renderer *renderer) const 
    {
        // Draw hex lines. Odd rows has one less slot. Last row is ommited, since a ball there would end the game.
        for (short r = 0; r < m_rows - 1; ++r) 
        {
            for (short c = 0; c < m_cols; ++c) 
            {
                drawHexOutline(renderer, cellCenter({r, c}));
            }
        }

        // Draw all active bubbles.
        for (auto &row : m_grid) 
        {
            for (auto &bubble : row) 
            {
                if (bubble)
                    bubble->draw(renderer);
            }
        }
    }

    utils::Vec2f BubbleGrid::cellCenter(utils::HexCoord pos) const noexcept 
    {
        return utils::hexToPixel(pos, m_origin);
    }

    void BubbleGrid::clearVisited()
    {
        for(short r = 0; r < m_rows; r++)
        {
            std::fill(visited[r].begin(), visited[r].end(), 0);
        }
    }

    bool BubbleGrid::isVisited(utils::HexCoord pos) const
    {
        return visited[pos.r][pos.c] == 1;
    }

    std::vector<utils::HexCoord> BubbleGrid::neighbours(utils::HexCoord pos) const 
    {
        short offset_r = pos.r % 2;

        static constexpr utils::HexCoord dirs[6] = {{ 0, 1},   { 0, -1},  // samle line
                                                    {-1, 0},   {-1,  1},  // prev line
                                                    { 1, 0},   { 1,  1}}; // next line
        std::vector<utils::HexCoord> result;
        result.reserve(6);
        for (auto &d : dirs) 
        {
            utils::HexCoord n{pos.r + d.r, pos.c + d.c};
            // If odd row, subtract 1 in the indexes of top and bottom neighbors.
            if(offset_r == 0 && (d.r == 1 || d.r == -1)) 
                n.c -= 1; 
            if (n.c >= 0 && n.c < m_cols && n.r >= 0 && n.r < m_rows)
            result.push_back(n);
        }
        return result;
    }

    void BubbleGrid::drawHexOutline(SDL_Renderer *renderer,
                                    utils::Vec2f center) const 
    {
        // Draw six-sided polygon outline.
        SDL_SetRenderDrawColor(renderer, 60, 60, 80, 100); // subtle dark colour
        constexpr int sides = 6;
        for (int i = 0; i < sides; ++i) 
        {
            float a0 = static_cast<float>(i) / sides * 2.f * utils::PI;
            float a1 = static_cast<float>(i + 1) / sides * 2.f * utils::PI;
            int x0 = static_cast<int>(center.x + m_hexSize * std::cos(a0));
            int y0 = static_cast<int>(center.y + m_hexSize * std::sin(a0));
            int x1 = static_cast<int>(center.x + m_hexSize * std::cos(a1));
            int y1 = static_cast<int>(center.y + m_hexSize * std::sin(a1));
            SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
        }
    }

    bool BubbleGrid::colorExists(classes::BubbleColor color) const
    {
        for (const auto &row : m_grid) 
        {
            for (const auto &bubble : row) 
            {
                if (bubble && bubble->isActive() && bubble->color() == color)
                    return true;
            }
        }
        return false;
    }

} // namespace classes
