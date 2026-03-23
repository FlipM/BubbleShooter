#include "LevelLoader.hpp"
#include "LearningLevel.hpp"

namespace levels 
{
    void LevelLoader::loadLevel(levels::Stage stg, classes::BubbleGrid &grid)
    {
        m_currentLevel = this->selectStage(stg);
        if(m_currentLevel)
            m_currentLevel->fillInitialGrid(grid);
    }

    bool LevelLoader::isStageCleared(levels::Stage stg, classes::BubbleGrid &grid)
    {
        if(m_currentLevel)
            return m_currentLevel->isCleared(grid);

        return false;
    }

    bool LevelLoader::exceededShootLimit(int shootCount) const
    {
        if(m_currentLevel)
            return shootCount > m_currentLevel->shootLimit();

        return false;
    }

    std::unique_ptr<Level> LevelLoader::selectStage(Stage stg)
    {
        switch(stg)
        {
            case levels::Stage::LEARNING_1:
                return std::make_unique<levels::LearningLevel>();
            default:
            case levels::Stage::COUNT:
                return nullptr;
        }
    }

    std::vector<classes::BubbleColor> LevelLoader::getStagePalette()
    {
        return m_currentLevel->getPalette();   
    }
}