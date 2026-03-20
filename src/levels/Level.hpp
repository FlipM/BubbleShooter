#pragma once

#include "classes/BubbleGrid.hpp"
#include <memory>
#include <string>

namespace levels 
{
    class Level
    {
        public:
            virtual ~Level() = default;
            virtual void fillInitialGrid(std::shared_ptr<classes::BubbleGrid> &grid);
            virtual std::vector<short> getPalette();
            virtual bool isCleared(std::shared_ptr<classes::BubbleGrid> &grid);

        private:
            std::vector<short> level_Palette;

    };

} // namespace levels
