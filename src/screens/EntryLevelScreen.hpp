// screens/GameOverScreen.hpp
#pragma once

#include "Screen.hpp"
#include <functional>

namespace screens {

    class EntryLevelScreen final : public Screen 
    {
        public:
            using Callback = std::function<void()>;

            EntryLevelScreen(Callback onPlay, levels::Stage initStage, SDL_Rect viewport);

            void handleEvent(const SDL_Event &event,
                            const core::InputHandler &input) override;
            void update(float deltaSeconds) override;
            void render(SDL_Renderer *renderer) override;

        private:
            Callback m_onPlay;
            levels::Stage m_initStage;
            SDL_Rect m_viewport;
            int m_finalScore;
    };

} // namespace screens
