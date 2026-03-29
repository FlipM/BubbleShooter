#include "RapidfireLevel.hpp"

namespace levels 
{
    RapidfireLevel::RapidfireLevel()
    {
        // Initialize palette with all available colors.
        for (classes::BubbleColor color = classes::BubbleColor::Red; color < classes::BubbleColor::COUNT;)
        {
            level_palette.push_back(static_cast<classes::BubbleColor>(color));
            color = static_cast<classes::BubbleColor>(static_cast<int>(color) + 1);
        }
    }

    /// Fill initial grid and enable rapid-fire mode for this level.
    void RapidfireLevel::fillInitialGrid(classes::BubbleGrid &grid)
    {
        grid.fillInitialGrid(level_palette, ROWS_TO_FILL); 
        rapidFire = true;
    }

    /// Return the maximum number of shots before increasing rows
    int RapidfireLevel::shootLimit() const
    {
        return m_shootLimit;
    }
} // namespace levels