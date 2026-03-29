// screens/Screen.hpp
// Pure abstract base for all game screens.
#pragma once

#include "core/InputHandler.hpp"
#include "core/Renderer.hpp"
#include <SDL2/SDL.h>

namespace screens 
{
    /// Abstract base class for game screens (menu, gameplay, etc).
    class Screen 
    {
        public:
            virtual ~Screen() = default;

            // Non-copyable and non-movable.
            Screen(const Screen &) = delete;
            Screen &operator=(const Screen &) = delete;

            // ── Lifecycle Callbacks ────────────────────────────────────────
            virtual void handleEvent(const SDL_Event &event, const core::InputHandler &input) = 0;
            virtual void update(float deltaSeconds) = 0;
            virtual void render(core::Renderer &renderer) = 0;

        protected:
            Screen() = default;
    };
} // namespace screens
