// screens/EntryLevelScreen.hpp
// The stage entry screen: brief intro before actual level gameplay begins.
#pragma once

#include "Screen.hpp"
#include "levels/LevelLoader.hpp"
#include <functional>

namespace screens 
{
    class EntryLevelScreen final : public Screen 
    {
        public:
            using Callback = std::function<void()>;

            /// Initialize entry screen with stage information and callback.
            EntryLevelScreen(Callback onPlay, levels::GameData &gameData, SDL_Rect viewport);

            // ── Lifecycle Callbacks ────────────────────────────────────────
            void handleEvent(const SDL_Event &event, const core::InputHandler &input) override;
            void update(float deltaSeconds) override;
            void render(core::Renderer &renderer) override;

        private:
            static constexpr float AUTO_START_DURATION = 3.f;  ///< Seconds before automatic game start.

            Callback m_onPlay;                                  ///< Callback to start gameplay.
            levels::GameData &m_gd;                             ///< Current stage and score reference.
            SDL_Rect m_viewport;                                ///< Screen display dimensions.
            levels::LevelLoader m_levelLoader;                  ///< Level configuration loader.
            float m_timer{0.f};                                 ///< Elapsed time for auto-start countdown.
    };

} // namespace screens
