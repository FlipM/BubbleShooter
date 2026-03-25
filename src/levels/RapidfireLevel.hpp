#pragma once

#include "Level.hpp"
#include <memory>
#include <string>

namespace levels 
{
    class RapidfireLevel final : public Level
    {
        const int rowsToFill = 3;

        public:
            RapidfireLevel()
            {
                //All colors
                for(classes::BubbleColor color = classes::BubbleColor::Red; color < classes::BubbleColor::COUNT;)
                {
                    level_palette.push_back(static_cast<classes::BubbleColor>(color));
                    color = static_cast<classes::BubbleColor>(static_cast<int>(color) + 1);
                }
            };

            void fillInitialGrid(classes::BubbleGrid &grid) override;
            bool isCleared(classes::BubbleGrid &grid) override;
            int shootLimit() const override;
            bool exceededShootingTime(float &elapsedTime) const override;
            std::vector<classes::BubbleColor> getPalette() override;


        private:
            const int m_shootLimit{20};
            const float timeLimit{1.f};

            


    };

} // namespace levels
