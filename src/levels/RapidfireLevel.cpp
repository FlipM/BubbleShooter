#include "RapidfireLevel.hpp"

namespace levels 
{
    void RapidfireLevel::fillInitialGrid(classes::BubbleGrid &grid)
    {
        grid.fillInitialGrid(level_palette, rowsToFill); 
        rapidFire = true;

    }

    bool RapidfireLevel::isCleared(classes::BubbleGrid &grid)
    {
        return grid.isEmpty();
    }

    int RapidfireLevel::shootLimit() const
    {
        return m_shootLimit;
    }

    std::vector<classes::BubbleColor> RapidfireLevel::getPalette()
    {
        return level_palette;
    }

}