#pragma once

#include "classes/BubbleGrid.hpp"
#include "classes/Score.hpp"
#include <memory>
#include <string>

namespace levels 
{
    enum class Stage
    {
        LEARNING_1,
        CASCADE_2,
        RAPIDFIRE_3,
        COLORBLIND_4,
        COUNT
    };

    struct GameData
    {
        classes::Score score;
        Stage currentStage{Stage::LEARNING_1};
    };
    
    class Level
    {
        public:
            virtual ~Level() = default;
            virtual void fillInitialGrid(classes::BubbleGrid&grid) = 0;
            virtual bool isCleared(classes::BubbleGrid &grid) = 0;
            virtual int shootLimit() const = 0;
            virtual float shootingTimeLimit() const { return -1.f; }
            virtual bool changeColorAtLanding() const { return false; }
            virtual std::vector<classes::BubbleColor> getPalette() = 0;

             // Non-copyable.
            Level(const Level &) = delete;
            Level &operator=(const Level &) = delete;

        protected:
            std::vector<classes::BubbleColor> level_palette;
            bool rapidFire{false};
            Level() = default;



        
        

    };

} // namespace levels
