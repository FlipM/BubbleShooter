// core/InputHandler.hpp
// Polls SDL events and provides a query API for input state.
#pragma once

#include <SDL2/SDL.h>
#include <unordered_map>

namespace core 
{

    struct MouseState 
    {
        int x{0}, y{0};          ///< Current cursor position (window coords).
        bool leftDown{false};    ///< True while LMB is held.
        bool leftClicked{false}; ///< True for one frame after LMB release.
    };

    class InputHandler 
    {
        public:
            InputHandler() = default;
            ~InputHandler() = default;

            // Non-copyable, movable.
            InputHandler(const InputHandler &) = delete;
            InputHandler &operator=(const InputHandler &) = delete;
            InputHandler(InputHandler &&) = default;
            InputHandler &operator=(InputHandler &&) = default;

            bool pollEvents();

            // ── Keyboard State ────────────────────────────────────────────────────
            [[nodiscard]] bool isKeyDown(SDL_Keycode key) const;
            [[nodiscard]] bool wasKeyPressed(SDL_Keycode key) const;

            // ── Mouse State ────────────────────────────────────────────────────────
            [[nodiscard]] const MouseState &mouse() const noexcept { return m_mouse; }
            [[nodiscard]] bool wasMouseClicked() const noexcept { return m_mouse.leftClicked; }

        private:
            MouseState m_mouse;
            std::unordered_map<SDL_Keycode, bool> m_keysDown;       ///< Persistent key state.
            std::unordered_map<SDL_Keycode, bool> m_keysPressed;    ///< Single-frame pulse.

            void resetFrameState();
    };

} // namespace core
