#pragma once

#include "Level.hpp"

namespace levels 
{
    /// Learning level: tutorial stage with simple bubble patterns for beginners.
    /// Introduces basic matching mechanics with limited color palette.
    class LearningLevel final : public Level
    {
        public:
            LearningLevel();

            void fillInitialGrid(classes::BubbleGrid &grid) override;
            int shootLimit() const override;

        private:
            int m_shootLimit{INITIAL_SHOOT_LIMIT};

            void makeTriplet(classes::BubbleGrid &grid, utils::HexCoord origin, 
                           classes::BubbleColor color, bool inverse);
    };

} // namespace levels
