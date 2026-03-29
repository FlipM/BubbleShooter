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
            ~Settings() = default;

            // Non-copyable, movable.
            Settings(const Settings &) = delete;
            Settings &operator=(const Settings &) = delete;
            Settings(Settings &&) = default;
            Settings &operator=(Settings &&) = default;

            // ── Getters ────────────────────────────────────────────────────────────
            [[nodiscard]] GameSettings &get() noexcept { return m_settings; }
            [[nodiscard]] const GameSettings &get() const noexcept { return m_settings; }

            // ── File I/O ───────────────────────────────────────────────────────────
            void load(const std::string &path);
            void save(const std::string &path) const;

        private:
            GameSettings m_settings; ///< Current settings state.
    };

} // namespace core
