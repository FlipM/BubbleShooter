#pragma once

#include "Level.hpp"
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
                for(classes::BubbleColor color = classes::BubbleColor::Red; color < classes::BubbleColor::Blue;)
                {
                    level_palette.push_back(static_cast<classes::BubbleColor>(color));
                    color = static_cast<classes::BubbleColor>(static_cast<int>(color) + 1);
                }
            };

            void fillInitialGrid(classes::BubbleGrid &grid) override;
            bool isCleared(classes::BubbleGrid &grid) override;
            std::vector<classes::BubbleColor> getPalette() override;


        private:


    };

} // namespace levels
