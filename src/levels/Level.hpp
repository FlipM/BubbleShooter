#pragma once

#include "classes/BubbleGrid.hpp"
#include "classes/Score.hpp"
#include <memory>
#include <string>

static constexpr int INITIAL_SHOOT_LIMIT = 10;  ///< Maximum shots allowed in learning level.
static constexpr int PATTERN_ROW_STRIDE = 2;    ///< Rows spacing for triplet patterns.
static constexpr int PATTERN_COL_STRIDE = 3;    ///< Column spacing for color variation.
static constexpr int ROWS_TO_FILL = 3;          ///< Number of rows to fill initially in certain levels.

namespace levels 
{
    enum class Stage
    {
        LEARNING_1,   ///< Tutorial level with simple bubble patterns.
        COUNT,         ///< Total number of stages.
        CASCADE_2,    ///< Vertical drop challenge with cascading bubbles.
        RAPIDFIRE_3,  ///< Fast-paced level with time limit and rapid shooting.
        COLORBLIND_4 ///< Color-changing bubbles for difficulty variation.
    };

    struct GameData
    {
        classes::Score score;                      ///< Current game score tracking.
        Stage currentStage{Stage::LEARNING_1};     ///< Current active stage.
    };
    
    class Level
    {
        public:
            virtual ~Level() = default;

            // Non-copyable.
            Level(const Level &) = delete;
            Level &operator=(const Level &) = delete;

            // ── Grid and Game Logic ────────────────────────────────────────
            virtual void fillInitialGrid(classes::BubbleGrid &grid) = 0;
            virtual bool isCleared(const classes::BubbleGrid &grid) { return grid.isEmpty(); };
            virtual int shootLimit() const = 0;
            virtual float shootingTimeLimit() const { return -1.f; }
            virtual bool changeColorAtLanding() const { return false; }
            virtual std::vector<classes::BubbleColor> getPalette() { return level_palette; };

        protected:
            std::vector<classes::BubbleColor> level_palette; ///< Available colors for this level.
            bool rapidFire{false};                           ///< Whether level enables rapid-fire mode.
            Level() = default;
    };

} // namespace levels
