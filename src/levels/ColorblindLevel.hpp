#pragma once

#include "Level.hpp"

namespace levels 
{
    /// Colorblind level: challenging mode where bubbles change color when they land.
    /// Tests player prediction and strategy as color matching becomes more difficult.
    class ColorblindLevel final : public Level
    {
        public:
            ColorblindLevel();

            void fillInitialGrid(classes::BubbleGrid &grid) override;
            int shootLimit() const override;
            bool changeColorAtLanding() const override { return true; }

        private:
            int m_shootLimit{INITIAL_SHOOT_LIMIT + 5};
    };

} // namespace levels
