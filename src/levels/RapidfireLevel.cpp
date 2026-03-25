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

    bool RapidfireLevel::exceededShootingTime(float &elapsedTime) const
    {
    
        if (elapsedTime > timeLimit)
        {
            elapsedTime = 0.0f; // Reset timer for next shot.
            return true;
        }
        return false;
    }

    std::vector<classes::BubbleColor> RapidfireLevel::getPalette()
    {
        return level_palette;
    }

}