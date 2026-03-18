// screens/Screen.hpp
// Pure abstract base for all game screens.
#pragma once

#include "core/InputHandler.hpp"
#include <SDL2/SDL.h>

namespace screens 
{
    class Screen 
    {
        public:
            virtual ~Screen() = default;

            /// Process one SDL event (called once per event per frame).
            virtual void handleEvent(const SDL_Event &event,
                                    const core::InputHandler &input) = 0;

            /// Update game logic for this frame.
            virtual void update(float deltaSeconds) = 0;

            /// Render this screen's content.
            virtual void render(SDL_Renderer *renderer) = 0;

            // Non-copyable.
            Screen(const Screen &) = delete;
            Screen &operator=(const Screen &) = delete;

        protected:
            Screen() = default;
    };
} // namespace screens
