// core/Settings.cpp
#include "Settings.hpp"
#include <iostream>

namespace core 
{
    void Settings::load(const std::string &path) 
    {
        std::clog << "[Settings] load() stub called for: " << path << '\n';
    }

    void Settings::save(const std::string &path) const 
    {
        std::clog << "[Settings] save() stub called for: " << path << '\n';
    }

} // namespace core
