#include "ColorblindLevel.hpp"

namespace levels 
{
    ColorblindLevel::ColorblindLevel()
    {
        // Initialize palette with limited colors: Red through Blue (excluding higher colors).
        for (classes::BubbleColor color = classes::BubbleColor::Red; color < classes::BubbleColor::Blue;)
        {
            level_palette.push_back(static_cast<classes::BubbleColor>(color));
            color = static_cast<classes::BubbleColor>(static_cast<int>(color) + 1);
        }
    }

    /// Fill initial grid with random bubble patterns and enable color changing.
    void ColorblindLevel::fillInitialGrid(classes::BubbleGrid &grid)
    {
        grid.fillInitialGrid(level_palette, ROWS_TO_FILL); 
        rapidFire = true;
    }

    /// Return the maximum number of shots before increasing rows
    int ColorblindLevel::shootLimit() const
    {
        return m_shootLimit;
    }
}