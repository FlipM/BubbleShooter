#include "LearningLevel.hpp"

namespace levels 
{
    void LearningLevel::fillInitialGrid(classes::BubbleGrid &grid)
    {
        // Simple pattern for testing: 3 rows of alternating colors.
        classes::BubbleColor color = classes::BubbleColor::Red;
        for (int r = 0; r < 5; r+=2) 
        {
            for (int c = 0; c < grid.cols() - 2; c += 3) 
            {
                makeTriplet(grid, {r, c}, color, false);
                color = classes::getNextColor(color);
                makeTriplet(grid, {r+1, c+1}, color, true);
                color = classes::getNextColor(color);
            }
        }

        color = classes::BubbleColor::Purple;
        for (int r = 0; r < 5; r++) 
        {
            grid.addBubble(std::make_unique<classes::Bubble>(color), {r, grid.cols() - 1});
        }
    }

    bool LearningLevel::isCleared(classes::BubbleGrid &grid)
    {
        // Level is cleared when all bubbles are popped.
        for (int r = 0; r < grid.rows(); ++r) 
        {
            for (int c = 0; c < grid.cols(); ++c) 
            {
                if (grid.at({r, c}))
                    return false;
            }
        }
        return true;
    }

    int LearningLevel::shootLimit() const
    {
        return m_shootLimit;
    }

    std::vector<classes::BubbleColor> LearningLevel::getPalette()
    {
        return level_palette;
    }

    void LearningLevel::makeTriplet(classes::BubbleGrid &grid, utils::HexCoord origin, classes::BubbleColor color, bool inverse)
    {
        int nextC = origin.c + 1;
        int otherR = origin.r + (inverse ? -1 : 1);

        // Place 3 bubbles of the same color in a cluster for testing matches.
        grid.addBubble(std::make_unique<classes::Bubble>(color), origin);
        grid.addBubble(std::make_unique<classes::Bubble>(color), {origin.r, nextC});
        grid.addBubble(std::make_unique<classes::Bubble>(color), {otherR, (inverse ? nextC : origin.c)});
        
    }
    
}
