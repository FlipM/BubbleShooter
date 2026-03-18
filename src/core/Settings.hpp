// core/Settings.hpp
// Mutable game settings — persisted to disk (stub).
#pragma once
#include <string>

namespace core 
{

    struct GameSettings 
    {
        bool soundEnabled{true};
        float masterVolume{1.0f}; ///< 0.0 – 1.0
        float brightness{1.0f};   ///< 0.0 – 2.0  (1.0 = default)
        bool fullscreen{false};
        int targetFPS{60};
        // TODO: add more options (language, control scheme, …)
    };

    class Settings 
    {
        public:
            Settings() = default;

            /// Load settings from a config file (INI/JSON — stub).
            void load(const std::string &path);

            /// Save current settings to disk (stub).
            void save(const std::string &path) const;

            [[nodiscard]] GameSettings &get() noexcept { return m_settings; }
            [[nodiscard]] const GameSettings &get() const noexcept { return m_settings; }

        private:
            GameSettings m_settings;
            // TODO: file path stored here once serialization is implemented.
    };

} // namespace core
