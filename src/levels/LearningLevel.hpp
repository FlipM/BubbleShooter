#pragma once

#include "classes/BubbleGrid.hpp"
#include <memory>
#include <string>

namespace levels 
{
    class LearningLevel final : public Level
    {
        public:
            LearningLevel()
            {
                //All colors
                for(auto color = 0; color < BubbleColor::COUNT; color++)
                {
                    level_pallete.push_back(color);
                }
            };

            void fillInitialGrid(std::shared_ptr<classes::BubbleGrid> &grid) override


        private:
            void fillInitialGrid(std::shared_ptr<classes::BubbleGrid> &grid) override;      
            bool isCleared(std::shared_ptr<classes::BubbleGrid> &grid) override;
            std::vector<short> getPalette() override;


    };

} // namespace levels
