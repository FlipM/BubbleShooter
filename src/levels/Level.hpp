#pragma once

#include "classes/BubbleGrid.hpp"
#include <memory>
#include <string>

namespace levels 
{

    enum class Stage
    {
        LEARNING_1,
        LEVEL_2,
        LEVEL_3,
        LEVEL_4,
        LEVEL_5,
        LEVEL_6,
        LEVEL_7,
        LEVEL_8,
        LEVEL_9,
        LEVEL_10,
        COUNT
    };
    
    class Level
    {
        public:
            virtual ~Level() = default;
            virtual void fillInitialGrid(classes::BubbleGrid&grid) = 0;
            virtual bool isCleared(classes::BubbleGrid &grid) = 0;
            virtual std::vector<classes::BubbleColor> getPalette() = 0;

             // Non-copyable.
            Level(const Level &) = delete;
            Level &operator=(const Level &) = delete;

        protected:
            std::vector<classes::BubbleColor> level_palette;
            Level() = default;



        
        

    };

} // namespace levels
