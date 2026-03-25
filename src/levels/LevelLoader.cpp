#include "LevelLoader.hpp"
#include "LearningLevel.hpp"
#include "CascadeLevel.hpp"
#include "RapidfireLevel.hpp"
#include "ColorblindLevel.hpp"

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

    bool LevelLoader::exceededShootingTime(float &elapsedTime) const
    {
        if(m_currentLevel && m_currentLevel->exceededShootingTime(elapsedTime))
        {
            elapsedTime = 0.0f; // Reset timer for next shot.
            return true;
        }

        return false;
    }

    bool LevelLoader::changeColor() const
    {
        // Only Rapidfire level changes colors.
        return m_currentLevel && m_currentLevel->changeColorAtLanding();
    }

    std::unique_ptr<Level> LevelLoader::selectStage(Stage stg)
    {
        switch(stg)
        {
            case levels::Stage::LEARNING_1:
                return std::make_unique<levels::LearningLevel>();
                
            case levels::Stage::CASCADE_2:
                return std::make_unique<levels::CascadeLevel>();
                
            case levels::Stage::RAPIDFIRE_3:
                return std::make_unique<levels::RapidfireLevel>();
                
            case levels::Stage::COLORBLIND_4:
                return std::make_unique<levels::ColorblindLevel>();

            default:
            case levels::Stage::COUNT:
                std::clog << "[LevelLoader] invalid stage selected\n";
                return nullptr;
        }
    }

    std::vector<classes::BubbleColor> LevelLoader::getStagePalette()
    {
        return m_currentLevel->getPalette();   
    }
}