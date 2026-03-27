#pragma once

#include "Level.hpp"
#include <memory>
#include <string>
#include <iostream>


namespace levels 
{
    class LevelLoader 
    {
        public:
            LevelLoader(levels::Stage stg = levels::Stage::LEARNING_1) : m_currentLevel(selectStage(stg)) {};
            ~LevelLoader() = default;

            void loadLevel(classes::BubbleGrid &grid);
            bool isStageCleared(classes::BubbleGrid &grid);
            bool exceededShootLimit(int shootCount) const;
            bool exceededShootingTime(float &elapsedTime) const;
            bool changeColor() const;
            
            std::string getStageName(Stage stg) const;
            std::vector<std::string> getStageDescription(Stage stg) const;

            std::unique_ptr<levels::Level> selectStage(Stage stg);
            std::vector<classes::BubbleColor> getStagePalette();

        private:
            std::unique_ptr<levels::Level> m_currentLevel;
    };
} // namespace levels
