// core/Settings.hpp
// Mutable game settings — persisted to disk (stub).
#pragma once
#include <string>

namespace core 
{

    struct GameSettings 
    {
        bool soundEnabled{true};
        int targetFPS{60};
    };

    class Settings 
    {
        public:
            Settings() = default;

            /// Load settings from a config file 
            void load(const std::string &path);

            /// Save current settings to disk
            void save(const std::string &path) const;

            [[nodiscard]] GameSettings &get() noexcept { return m_settings; }
            [[nodiscard]] const GameSettings &get() const noexcept { return m_settings; }

        private:
            GameSettings m_settings;
    };

} // namespace core
