// core/Settings.cpp
#include "Settings.hpp"
#include <iostream>

namespace core {

void Settings::load(const std::string &path) {
  // TODO: open file at `path`, parse INI/JSON, fill m_settings.
  std::clog << "[Settings] load() stub called for: " << path << '\n';
}

void Settings::save(const std::string &path) const {
  // TODO: serialise m_settings to file at `path`.
  std::clog << "[Settings] save() stub called for: " << path << '\n';
}

} // namespace core
