#pragma once

#include "Level.hpp"
#include <memory>
#include <string>

namespace levels 
{
    class CascadeLevel final : public Level
    {
        public:
            CascadeLevel()
            {
                //All colors
                for(classes::BubbleColor color = classes::BubbleColor::Red; color < classes::BubbleColor::Green;)
                {
                    level_palette.push_back(static_cast<classes::BubbleColor>(color));
                    color = static_cast<classes::BubbleColor>(static_cast<int>(color) + 1);
                }
            };

            void fillInitialGrid(classes::BubbleGrid &grid) override;
            bool isCleared(classes::BubbleGrid &grid) override;
            int shootLimit() const override;
            std::vector<classes::BubbleColor> getPalette() override;


        private:
            void fillLine(classes::BubbleGrid &grid, int r, classes::BubbleColor color);
            void semiFillColumn(classes::BubbleGrid &grid, int r, int c, int count, classes::BubbleColor color);
            int m_shootLimit{10};


    };

} // namespace levels
