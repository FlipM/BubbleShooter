#include "LevelLoader.hpp"
#include "LearningLevel.hpp"
#include "CascadeLevel.hpp"
#include "RapidfireLevel.hpp"
#include "ColorblindLevel.hpp"

namespace levels 
{
    void LevelLoader::loadLevel( classes::BubbleGrid &grid)
    {
        if(m_currentLevel)
            m_currentLevel->fillInitialGrid(grid);
    }

    bool LevelLoader::isStageCleared(classes::BubbleGrid &grid)
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

    std::string LevelLoader::getStageName(Stage stg) const
    {
        switch(stg)
        {
            case levels::Stage::LEARNING_1:
                return "Learning";
                
            case levels::Stage::CASCADE_2:
                return "Cascade";
                
            case levels::Stage::RAPIDFIRE_3:
                return "Rapidfire";
                
            case levels::Stage::COLORBLIND_4:
                return "Colorblind";

            default:
            case levels::Stage::COUNT:
                std::clog << "[LevelLoader] invalid stage for name\n";
                return "Unknown Stage";
        }
    }

    std::vector<std::string> LevelLoader::getStageDescription(Stage stg) const
    {
        switch(stg)
        {
            case levels::Stage::LEARNING_1:
                return {"Left-click to shoot bubbles.", "Match 3 or more to pop them."};
                
            case levels::Stage::CASCADE_2:
                return {"Gray bubbles do not pop,", "but they fall without support!"};
                
            case levels::Stage::RAPIDFIRE_3:
                return {"Be quick!"};
                
            case levels::Stage::COLORBLIND_4:
                return {"Do not trust the colors!"};

            default:
            case levels::Stage::COUNT:
                std::clog << "[LevelLoader] invalid stage for description\n";
                return {"No description available."};
        }
    }

    std::vector<classes::BubbleColor> LevelLoader::getStagePalette()
    {
        return m_currentLevel->getPalette();   
    }


}