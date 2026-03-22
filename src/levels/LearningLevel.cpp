#include "LearningLevel.hpp"

namespace levels 
{
    void LearningLevel::fillInitialGrid(classes::BubbleGrid &grid)
    {
        // Simple pattern for testing: 3 rows of alternating colors.
        for (short r = 0; r < 3; ++r) 
        {
            for (short c = 0; c < grid.cols(); ++c) 
            {
                classes::BubbleColor color = grid.getRandomColor(level_palette);
                grid.addBubble(std::make_unique<classes::Bubble>(color), {r, c});
            }
        }

    }

    bool LearningLevel::isCleared(classes::BubbleGrid &grid)
    {
        // Level is cleared when all bubbles are popped.
        for (short r = 0; r < grid.rows(); ++r) 
        {
            for (short c = 0; c < grid.cols(); ++c) 
            {
                if (grid.at({r, c}))
                    return false;
            }
        }
        return true;
    }

    std::vector<classes::BubbleColor> LearningLevel::getPalette()
    {
        return level_palette;
    }
    
}
