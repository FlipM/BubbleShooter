#include "LearningLevel.hpp"

namespace levels 
{

    LearningLevel::LearningLevel()
    {
        // Initialize palette with all available colors for maximum flexibility.
        for (classes::BubbleColor color = classes::BubbleColor::Red; color < classes::BubbleColor::COUNT;)
        {
            level_palette.push_back(static_cast<classes::BubbleColor>(color));
            color = static_cast<classes::BubbleColor>(static_cast<int>(color) + 1);
        }
    }

    /// Fill initial grid with a simple alternating triplet pattern.
    void LearningLevel::fillInitialGrid(classes::BubbleGrid &grid)
    {
        const int rows = 5;  ///< Number of rows to fill initially.
        classes::BubbleColor color = classes::BubbleColor::Red;
        for (int r = 0; r < rows; r += PATTERN_ROW_STRIDE) 
        {
            for (int c = 0; c < grid.cols() - 2; c += PATTERN_COL_STRIDE) 
            {
                makeTriplet(grid, {r, c}, color, false);
                color = classes::getNextColor(color);
                makeTriplet(grid, {r + 1, c + 1}, color, true);
                color = classes::getNextColor(color);
            }
        }

        // Fill rightmost column with purple bubbles.
        color = classes::BubbleColor::Purple;
        for (int r = 0; r < rows; r++) 
        {
            grid.addBubble(std::make_unique<classes::Bubble>(color), {r, grid.cols() - 1});
        }
    }

    /// Return the maximum number of shots before increasing rows
    int LearningLevel::shootLimit() const
    {
        return m_shootLimit;
    }

    /// Create a triplet of same-colored bubbles in hex formation.
    void LearningLevel::makeTriplet(classes::BubbleGrid &grid, utils::HexCoord origin, 
                                     classes::BubbleColor color, bool inverse)
    {
        int nextC = origin.c + 1;
        int otherR = origin.r + (inverse ? -1 : 1);

        grid.addBubble(std::make_unique<classes::Bubble>(color), origin);
        grid.addBubble(std::make_unique<classes::Bubble>(color), {origin.r, nextC});
        grid.addBubble(std::make_unique<classes::Bubble>(color), {otherR, (inverse ? nextC : origin.c)});
    }
}
