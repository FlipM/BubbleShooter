#pragma once

#include "Level.hpp"
#include <memory>
#include <string>


namespace levels 
{
    class LevelLoader 
    {
        public:
            LevelLoader(levels::Stage stg = levels::Stage::LEARNING_1) : m_currentLevel(selectStage(stg)) {};
            ~LevelLoader() = default;

            void loadLevel(Stage stg, classes::BubbleGrid &grid);
            bool isStageCleared(Stage stg, classes::BubbleGrid &grid);
            bool exceededShootLimit(int shootCount) const;
            std::unique_ptr<levels::Level> selectStage(Stage stg);
            std::vector<classes::BubbleColor> getStagePalette();

        private:
            std::unique_ptr<levels::Level> m_currentLevel;
    };
} // namespace levels
