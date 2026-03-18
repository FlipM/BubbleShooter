// core/InputHandler.hpp
// Polls SDL events each frame and exposes a clean query API.
#pragma once

#include <SDL2/SDL.h>
#include <unordered_map>

namespace core {

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

            /// Poll the SDL event queue.  Call once per frame.
            /// @returns false when the user requests quit (window-close / Alt-F4).
            bool pollEvents();

            // ── Keyboard ─────────────────────────────────────────────────────────
            /// True while the key is physically held.
            [[nodiscard]] bool isKeyDown(SDL_Keycode key) const;

            /// True for exactly one frame after the key is pressed.
            [[nodiscard]] bool wasKeyPressed(SDL_Keycode key) const;

            // ── Mouse ─────────────────────────────────────────────────────────────
            [[nodiscard]] const MouseState &mouse() const noexcept { return m_mouse; }

            /// True for exactly one frame after LMB is released.
            [[nodiscard]] bool wasMouseClicked() const noexcept { return m_mouse.leftClicked; }

        private:
            MouseState m_mouse;

            std::unordered_map<SDL_Keycode, bool> m_keysDown;
            std::unordered_map<SDL_Keycode, bool> m_keysPressed; ///< single-frame pulse

            void resetFrameState();
    };

} // namespace core
