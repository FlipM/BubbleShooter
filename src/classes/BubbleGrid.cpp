// classes/BubbleGrid.cpp
#include "BubbleGrid.hpp"
#include <cmath>
#include <iostream>
#include <queue>
#include <unordered_set>

namespace classes {

    BubbleGrid::BubbleGrid(int cols, int rows, float hexSize, utils::Vec2f origin)
        : m_cols(cols), m_rows(rows), m_hexSize(hexSize), m_origin(origin) 
    {
        m_grid.assign(rows, std::vector<std::shared_ptr<Bubble>>(cols, nullptr));
    }

    void BubbleGrid::addBubble(std::unique_ptr<Bubble> bubble,
                            utils::HexCoord pos) 
    {
        if (pos.r < 0 || pos.r >= m_rows || pos.c < 0 || pos.c >= m_cols)
            return;
        bubble->setGridPos(pos);
        bubble->setPixelPos(cellCenter(pos));
        m_grid[pos.r][pos.c] = std::move(bubble);
    }

    std::shared_ptr<Bubble> BubbleGrid::removeBubble(utils::HexCoord pos) 
    {
        if (pos.r < 0 || pos.r >= m_rows || pos.c < 0 || pos.c >= m_cols)
            return nullptr;
        return std::exchange(m_grid[pos.r][pos.c], nullptr);
    }

    std::shared_ptr<Bubble> BubbleGrid::at(utils::HexCoord pos) const 
    {
        if (pos.r < 0 || pos.r >= m_rows || pos.c < 0 || pos.c >= m_cols)
            return nullptr;
        return m_grid[pos.r][pos.c];
    }

    std::vector<utils::HexCoord>
    BubbleGrid::findMatches(utils::HexCoord origin) const 
    {
        // TODO: BFS collecting same-colour connected bubbles.
        // Returns coords of matched group (≥3 → pop them).
        auto src = at(origin);
        if (!src)
            return {};

        std::vector<utils::HexCoord> matched;
        // Placeholder: return only the origin cell.
        matched.push_back(origin);
        std::clog << "[BubbleGrid] findMatches() stub at (" << origin.c << ','
                    << origin.r << ")\n";
        return matched;
    }

    void BubbleGrid::dropFloating() 
    {
        // TODO: BFS from roof row; mark unreachable cells; remove them + add score.
        std::clog << "[BubbleGrid] dropFloating() stub\n";
    }

    utils::HexCoord BubbleGrid::snapToGrid(utils::Vec2f pixelPos) const 
    {
        // TODO: find nearest empty hex cell to pixelPos.
        return utils::pixelToHex(pixelPos, m_hexSize, m_origin);
    }

    void BubbleGrid::advanceDown() 
    {
        // TODO: shift all rows down by one, add a new row at the top.
        std::clog << "[BubbleGrid] advanceDown() stub\n";
    }

    void BubbleGrid::draw(SDL_Renderer *renderer) const 
    {
        // Draw hex lines. Odd rows has one less slot.
        for (int r = 0; r < m_rows; ++r) 
        {
            int n_cols = m_cols - (r % 2);
            for (int c = 0; c < n_cols; ++c) 
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
        return utils::hexToPixel(pos, m_hexSize, m_origin);
    }

    std::vector<utils::HexCoord> BubbleGrid::neighbours(utils::HexCoord pos) const 
    {
        int offset_r = pos.r % 2;

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
            float a0 = static_cast<float>(i) / sides * 2.f * 3.14159265f;
            float a1 = static_cast<float>(i + 1) / sides * 2.f * 3.14159265f;
            int x0 = static_cast<int>(center.x + m_hexSize * std::cos(a0));
            int y0 = static_cast<int>(center.y + m_hexSize * std::sin(a0));
            int x1 = static_cast<int>(center.x + m_hexSize * std::cos(a1));
            int y1 = static_cast<int>(center.y + m_hexSize * std::sin(a1));
            SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
        }
    }

} // namespace classes
