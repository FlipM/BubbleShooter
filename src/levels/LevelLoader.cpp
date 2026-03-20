#include "LevelLoader.hpp"

void LevelLoader::loadLevel(Levels level)
{
    std::unique_ptr<Level> lvl = this->selectStage(level);
    if(lvl)
        lvl->fillInitialGrid(grid);

}

std::unique_ptr<Level> LevelLoader::selectStage(Stage stg)
{
    switch(stg)
    {
        case Stage::LEARNING_1:
            return std::make_unique<LearningLevel>();
        default:
            return nullptr;
    }
}

int[] LevelLoader::getStagePalette()
{
    return m_currentLevel.getPalette();   
}