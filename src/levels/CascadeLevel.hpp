#pragma once

#include "Level.hpp"

namespace levels 
{
    /// Cascade level: challenging stage with vertical bubble drops and patterns.
    /// Tests player skill with cascading gravity and tactical bubble placement.
    class CascadeLevel final : public Level
    {
        public:
            CascadeLevel();

            void fillInitialGrid(classes::BubbleGrid &grid) override;
            int shootLimit() const override;

        private:
            static constexpr int FIRST_FILL_ROW = 0;        ///< First row to fill completely.
            static constexpr int SECOND_FILL_ROW = 4;       ///< Second row to fill completely.
            static constexpr int THIRD_FILL_ROW = 8;        ///< Third row to fill completely.
            static constexpr int ROW_STRIDE = 4;            ///< Spacing between filled rows.
            static constexpr int COLUMN_FILL_COUNT = 3;     ///< Number of bubbles in semi-filled columns.

            int m_shootLimit{INITIAL_SHOOT_LIMIT};

            void fillLine(classes::BubbleGrid &grid, int r, classes::BubbleColor color);
            void semiFillColumn(classes::BubbleGrid &grid, int r, int c, int count, 
                               classes::BubbleColor color);
    };

} // namespace levels
