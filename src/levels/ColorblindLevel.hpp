#pragma once

#include "Level.hpp"
#include <memory>
#include <string>

namespace levels 
{
    class ColorblindLevel final : public Level
    {
        const int rowsToFill = 3;

        public:
            ColorblindLevel()
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
            int shootLimit() const override;
            bool changeColorAtLanding() const override { return true; }
            std::vector<classes::BubbleColor> getPalette() override;


        private:
            const int m_shootLimit{15};

    };

} // namespace levels
