#include "CascadeLevel.hpp"

namespace levels 
{
    void CascadeLevel::fillInitialGrid(classes::BubbleGrid &grid)
    {
        // Simple pattern for testing: 3 rows of alternating colors.
        classes::BubbleColor color = classes::BubbleColor::Red;
        for (int r = 0; r < 9; r+=4) 
        {
            fillLine(grid, r, color);
            color = classes::getNextColor(color);
        }

        semiFillColumn(grid, 1, 1, 3, classes::BubbleColor::Yellow);
        semiFillColumn(grid, 5, 8, 3, classes::BubbleColor::Red);
        semiFillColumn(grid, 9, 5, 3, classes::BubbleColor::Orange);

    }

    bool CascadeLevel::isCleared(classes::BubbleGrid &grid)
    {
        return grid.isEmpty();
    }

    int CascadeLevel::shootLimit() const
    {
        return m_shootLimit;
    }

    std::vector<classes::BubbleColor> CascadeLevel::getPalette()
    {
        return level_palette;
    }

    void CascadeLevel::fillLine(classes::BubbleGrid &grid, int r, classes::BubbleColor color)
    {
        for (int c = 0; c < grid.cols(); c++)
        {
            grid.addBubble(std::make_unique<classes::Bubble>(color), {r, c});
        }
    }

    void CascadeLevel::semiFillColumn(classes::BubbleGrid &grid, int r, int c, int count, classes::BubbleColor color)
    {
        for (int i = 0; i < count; i++)
        {
            grid.addBubble(std::make_unique<classes::Bubble>(color), {r + i, c});
        }
    }

}