// core/Renderer.hpp
// RAII wrapper around SDL_Window + SDL_Renderer.
// All draw methods are stubs — implement rendering logic here.
#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <string>

namespace core 
{

    /// Custom deleters for SDL handle types.
    struct WindowDeleter 
    {
        void operator()(SDL_Window *w) const noexcept { SDL_DestroyWindow(w); }
    };
    struct RendererDeleter 
    {
        void operator()(SDL_Renderer *r) const noexcept { SDL_DestroyRenderer(r); }
    };

    using WindowPtr = std::unique_ptr<SDL_Window, WindowDeleter>;
    using RendererPtr = std::unique_ptr<SDL_Renderer, RendererDeleter>;

    /// Viewport geometry — keeps the game portrait-locked inside letterboxed bars.
    struct Viewport 
    {
        int x{0}, y{0};  ///< top-left offset inside the OS window
        int width{390};  ///< logical game width  (phone portrait, e.g. 390 px)
        int height{844}; ///< logical game height (phone portrait, e.g. 844 px)
    };

    class Renderer 
    {
        public:
            /// Creates OS window + SDL renderer.  Throws std::runtime_error on failure.
            explicit Renderer(const std::string &title, int windowW = 1080,
                                int windowH = 844);

            ~Renderer() = default;

            // Non-copyable, movable.
            Renderer(const Renderer &) = delete;
            Renderer &operator=(const Renderer &) = delete;
            Renderer(Renderer &&) = default;
            Renderer &operator=(Renderer &&) = default;

            // ── Frame lifecycle ───────────────────────────────────────────────────
            void clear();   ///< Fill with background (black bars + game area).
            void present(); ///< Flip to screen.

            // ── Primitive draw stubs ──────────────────────────────────────────────
            /// Draw a filled rectangle (in logical game coordinates).
            void drawRect(int x, int y, int w, int h, SDL_Color color);

            /// Draw the outline of a rectangle.
            void drawRectOutline(int x, int y, int w, int h, SDL_Color color);

            /// Draw a circle approximation using points.
            void drawCircle(int cx, int cy, int radius, SDL_Color color);

            /// Draw a line between two points.
            void drawLine(int x1, int y1, int x2, int y2, SDL_Color color);

            /// Draw a UTF-8 text string at (x,y).  Stub — requires SDL2_ttf.
            void drawText(const std::string &text, int x, int y, SDL_Color color);

            // ── Accessors ─────────────────────────────────────────────────────────
            [[nodiscard]] SDL_Renderer *raw() const noexcept { return m_renderer.get(); }
            [[nodiscard]] const Viewport &viewport() const noexcept { return m_viewport; }

        private:
            WindowPtr m_window;
            RendererPtr m_renderer;
            Viewport m_viewport;

            /// Recalculate viewport so game area is centred, preserving aspect ratio.
            void recalculateViewport(int windowW, int windowH);
    };

} // namespace core
