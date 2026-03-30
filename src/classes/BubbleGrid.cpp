// classes/BubbleGrid.cpp
#include "BubbleGrid.hpp"
#include "core/Renderer.hpp"
#include "core/UI.hpp"
#include <iostream>
#include <memory>
#include <vector>

namespace classes 
{
    constexpr int HEX_SIDES = 6;              // Sides in a hexagon.
    constexpr int ROWS_TO_ADVANCE = 2;        // Rows to advance bubbles downward.
    constexpr int MIN_MATCH_SIZE = 3;         // Minimum bubbles for a match.

    BubbleGrid::BubbleGrid(int cols, int rows, float hexSize, utils::Vec2f origin)
        : m_cols(cols), m_rows(rows), m_hexSize(hexSize), m_origin(origin) 
    {
        m_grid.assign(rows, std::vector<std::shared_ptr<Bubble>>(cols, nullptr));
        visited.assign(rows, std::vector<int>(cols, 0));
    }

    /// Place a bubble at the given hex coordinate.
    void BubbleGrid::addBubble(std::unique_ptr<Bubble> bubble, utils::HexCoord pos) 
    {
        if (pos.r < 0 || pos.r >= m_rows || pos.c < 0 || pos.c >= m_cols)
            return;

        bubble->setGridPos(pos);
        bubble->setPixelPos(cellCenter(pos));
        m_grid[pos.r][pos.c] = std::move(bubble);
    }

    /// Remove and return the bubble at the given position.
    std::shared_ptr<Bubble> BubbleGrid::removeBubble(utils::HexCoord pos) 
    {
        if (pos.r < 0 || pos.r >= m_rows || pos.c < 0 || pos.c >= m_cols)
            return nullptr;

        return std::exchange(m_grid[pos.r][pos.c], nullptr);
    }

    /// Get the bubble at the given position, or nullptr if out of bounds.
    std::shared_ptr<Bubble> BubbleGrid::at(utils::HexCoord pos) const 
    {
        if (pos.r < 0 || pos.r >= m_rows || pos.c < 0 || pos.c >= m_cols)
            return nullptr;
        return m_grid[pos.r][pos.c];
    }

    /// Check if bubble pointer is valid and active.
    bool BubbleGrid::isValidBubble(std::shared_ptr<Bubble> bubblePtr) const
    {
        return bubblePtr && bubblePtr->isActive();
    }

    /// Select a random color from the provided palette.
    classes::BubbleColor BubbleGrid::getRandomColor(const std::vector<classes::BubbleColor> &palette) const
    {        
        if (palette.empty())
            return classes::BubbleColor::Gray; //Protection, but should not happen

        int randomIndex = rand() % palette.size();
        return static_cast<classes::BubbleColor>(palette[randomIndex]);
    }

    /// Find all connected bubbles matching the color at origin (BFS/DFS).
    /// Returns a list of coordinates that form a match group.
    std::vector<utils::HexCoord> BubbleGrid::findMatches(utils::HexCoord origin) 
    {
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

    /// Recursively collect connected bubbles of the same color.
    std::vector<utils::HexCoord> BubbleGrid::recMatches(utils::HexCoord origin, BubbleColor &color)
    {
        std::vector<utils::HexCoord> matched;
        matched.push_back(origin);
        visited[origin.r][origin.c] = 1;

        std::vector<utils::HexCoord> neighbours_list = this->neighbours(origin);
        for(const auto& neighborCoord : neighbours_list) 
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

    /// Remove all floating bubbles (disconnected from the roof).
    void BubbleGrid::dropFloating() 
    {
        for(int c = 0; c < m_cols; c++)
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

        for(int r = 0; r < m_rows; r++)
        {
            for(int c = 0; c < m_cols; c++)
            {
                if(!isVisited({r, c}) && at({r, c}))
                    removeBubble({r, c})->pop();
            }
        }
        clearVisited();
    }

    /// Mark all bubbles reachable from position as visited (DFS).
    void BubbleGrid::dfsVisited(utils::HexCoord pos)
    {
        visited[pos.r][pos.c] = 1;

        std::vector<utils::HexCoord> neighbours_list = this->neighbours(pos);
        for(const auto& neighborCoord : neighbours_list) 
        {
            if (isVisited(neighborCoord))
                continue;

            auto neighborBubble = this->at(neighborCoord);
            if(isValidBubble(neighborBubble))
            {
                dfsVisited(neighborCoord);
            }
        }
    }

    /// Snap an in-flight bubble to the nearest empty hex cell.
    utils::HexCoord BubbleGrid::snapToGrid(utils::Vec2f pixelPos) const 
    {
        return utils::pixelToHex(pixelPos, m_origin);
    }

    /// Advance the grid downward and fill top rows with new random bubbles.
    void BubbleGrid::advanceDown(const std::set<classes::BubbleColor> &shooterColors) 
    {
        // Move bubbles down from second-to-last row upward.
        for(int r = m_rows - 3; r >= 0; --r) 
        {
            for (int c = 0; c < m_cols; ++c) 
            {
                auto bubble = at({r, c});
                if (isValidBubble(bubble)) 
                {
                    bubble->setGridPos({r + ROWS_TO_ADVANCE, c});
                    bubble->setPixelPos(cellCenter({r + ROWS_TO_ADVANCE, c}));
                    m_grid[r + ROWS_TO_ADVANCE][c] = std::move(m_grid[r][c]);
                    m_grid[r][c] = nullptr;
                }
            }
        }
        if(shooterColors.empty())
            return;

        // Fill top rows with new random bubbles.
        std::vector<classes::BubbleColor> colorVect(shooterColors.begin(), shooterColors.end());
        for (int c = 0; c < m_cols; ++c) 
        {
            for (int r = 0; r < ROWS_TO_ADVANCE; ++r) 
            {
                classes::BubbleColor color = getRandomColor(colorVect);
                this->addBubble(std::make_unique<classes::Bubble>(color), {r, c});
            }
        }
    }

    /// Fill the initial grid with random bubbles from the given palette.
    void BubbleGrid::fillInitialGrid(const std::vector<classes::BubbleColor> &palette, int rowsToFill) 
    {
        if (rowsToFill >= m_rows)
            rowsToFill = m_rows - 1; 
            
        for (int r = 0; r < rowsToFill; ++r) 
        {
            for (int c = 0; c < m_cols; ++c) 
            {
                classes::BubbleColor color = getRandomColor(palette);
                addBubble(std::make_unique<classes::Bubble>(color), {r, c});
            }
        }
    }

    /// Render all active bubbles in the grid.
    void BubbleGrid::draw(core::Renderer &renderer) const 
    {
        //drawOutlines(renderer);
        for (auto &row : m_grid) 
        {
            for (auto &bubble : row) 
            {
                if (bubble)
                    bubble->draw(renderer);
            }
        }
    }

    /// Render hex outlines for all cells (debugging aid).
    void BubbleGrid::drawOutlines(core::Renderer &renderer) const 
    {
        for (int r = 0; r < m_rows - 1; ++r) 
        {
            for (int c = 0; c < m_cols; ++c) 
            {
                drawHexOutline(renderer, cellCenter({r, c}));
            }
        }
    }

    /// Convert hex coordinates to screen pixel position.
    utils::Vec2f BubbleGrid::cellCenter(utils::HexCoord pos) const noexcept 
    {
        return utils::hexToPixel(pos, m_origin);
    }

    /// Clear the visited tracking array.
    void BubbleGrid::clearVisited()
    {
        for(int r = 0; r < m_rows; r++)
        {
            std::fill(visited[r].begin(), visited[r].end(), 0);
        }
    }

    /// Check if a cell has been visited in the current algorithm pass.
    bool BubbleGrid::isVisited(utils::HexCoord pos) const
    {
        if (pos.r < 0 || pos.r >= m_rows || pos.c < 0 || pos.c >= m_cols)
            return false;

        return visited[pos.r][pos.c] == 1;
    }

    /// Get all valid neighbors of a hex cell using flat-top layout.
    std::vector<utils::HexCoord> BubbleGrid::neighbours(utils::HexCoord pos) const 
    {
        int offset_r = pos.r % 2;

        static constexpr utils::HexCoord dirs[6] = {{ 0, 1},   { 0, -1},  // same row
                                                    {-1, 0},   {-1,  1},  // prev row
                                                    { 1, 0},   { 1,  1}}; // next row
        std::vector<utils::HexCoord> result;
        result.reserve(6);
        for (auto &d : dirs) 
        {
            utils::HexCoord n{pos.r + d.r, pos.c + d.c};
            // Adjust for odd rows in flat-top hex layout.
            if(offset_r == 0 && (d.r == 1 || d.r == -1)) 
                n.c -= 1; 
            if (n.c >= 0 && n.c < m_cols && n.r >= 0 && n.r < m_rows)
                result.push_back(n);
        }
        return result;
    }

    /// Render a single hex cell outline.
    void BubbleGrid::drawHexOutline(core::Renderer &renderer,
                                    utils::Vec2f center) const 
    {
        core::UI::Color core::UI::HEX_OUTLINE;
        for (int i = 0; i < HEX_SIDES; ++i) 
        {
            float a0 = static_cast<float>(i) / HEX_SIDES * 2.f * utils::PI;
            float a1 = static_cast<float>(i + 1) / HEX_SIDES * 2.f * utils::PI;
            int x0 = static_cast<int>(center.x + m_hexSize * std::cos(a0));
            int y0 = static_cast<int>(center.y + m_hexSize * std::sin(a0));
            int x1 = static_cast<int>(center.x + m_hexSize * std::cos(a1));
            int y1 = static_cast<int>(center.y + m_hexSize * std::sin(a1));
            renderer.drawLine(x0, y0, x1, y1, color);
        }
    }

    /// Check if a given color exists anywhere in the grid.
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

    /// Check if the grid is completely empty of active bubbles.
    bool BubbleGrid::isEmpty() const
    {
        for (const auto &row : m_grid) 
        {
            for (const auto &bubble : row) 
            {
                // Gray bubbles do not count as active for game-over conditions
                if (bubble && bubble->isActive() && bubble->color() != classes::BubbleColor::Gray)
                    return false;
            }
        }
        return true;
    }

} // namespace classes
