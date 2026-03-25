#include "ColorblindLevel.hpp"

namespace levels 
{
    void ColorblindLevel::fillInitialGrid(classes::BubbleGrid &grid)
    {
        grid.fillInitialGrid(level_palette, rowsToFill); 
        rapidFire = true;

    }

    bool ColorblindLevel::isCleared(classes::BubbleGrid &grid)
    {
        return grid.isEmpty();
    }

    int ColorblindLevel::shootLimit() const
    {
        return m_shootLimit;
    }

    std::vector<classes::BubbleColor> ColorblindLevel::getPalette()
    {
        return level_palette;
    }

}