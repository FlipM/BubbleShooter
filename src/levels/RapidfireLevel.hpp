#pragma once

#include "Level.hpp"

namespace levels 
{
    /// Rapidfire level: intense time-based challenge requiring fast decision-making.
    /// Players must clear bubbles within a strict time limit with unlimited shots.
    class RapidfireLevel final : public Level
    {
        public:
            RapidfireLevel();

            void fillInitialGrid(classes::BubbleGrid &grid) override;
            int shootLimit() const override;
            float shootingTimeLimit() const override { return TIME_LIMIT; }

        private:
            static constexpr float TIME_LIMIT = 1.f;        ///< Time limit in seconds for completion.

            int m_shootLimit{INITIAL_SHOOT_LIMIT + 10};
    };

} // namespace levels
