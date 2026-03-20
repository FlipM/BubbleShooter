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

    class LevelLoader 
    {
        public:
            void loadLevel(Stage stg, std::shared_ptr<classes::BubbleGrid> &grid);
            bool isStageCleared(Stage stg, std::shared_ptr<classes::BubbleGrid> &grid);
            std::unique_ptr<Level> selectStage(Stage stg);
            int[] getStagePalette();

        private:
            levels::Level m_currentLevel;
    };
} // namespace levels
