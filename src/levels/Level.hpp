#pragma once

#include "classes/BubbleGrid.hpp"
#include <memory>
#include <string>

namespace levels 
{

    enum class Stage
    {
        LEARNING_1,
        CASCADE_2,
        COUNT
    };
    
    class Level
    {
        public:
            virtual ~Level() = default;
            virtual void fillInitialGrid(classes::BubbleGrid&grid) = 0;
            virtual bool isCleared(classes::BubbleGrid &grid) = 0;
            virtual int shootLimit() const = 0;
            virtual std::vector<classes::BubbleColor> getPalette() = 0;

             // Non-copyable.
            Level(const Level &) = delete;
            Level &operator=(const Level &) = delete;

        protected:
            std::vector<classes::BubbleColor> level_palette;
            Level() = default;



        
        

    };

} // namespace levels
