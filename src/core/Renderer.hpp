// core/Renderer.hpp
// RAII wrapper around SDL_Window + SDL_Renderer.
// All draw methods are stubs — implement rendering logic here.
#pragma once

#include "UI.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <string>
#include <vector>

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

    struct FontDeleter 
    {
        void operator()(TTF_Font *f) const noexcept { if (f) TTF_CloseFont(f); }
    };

    using WindowPtr = std::unique_ptr<SDL_Window, WindowDeleter>;
    using RendererPtr = std::unique_ptr<SDL_Renderer, RendererDeleter>;
    using FontPtr = std::unique_ptr<TTF_Font, FontDeleter>;

    /// Viewport geometry — keeps the game portrait-locked inside letterboxed bars.
    struct Viewport 
    {
        int x{0}, y{0};  ///< top-left offset inside the OS window
        int width{390};  ///< logical game width  (phone portrait, e.g. 390 px)
        int height{844}; ///< logical game height (phone portrait, e.g. 844 px)
    };

    class Renderer 
    {
        const char *FONT_PATH = "assets/fonts/DejaVuSans.ttf";

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

            void initFont();

            // ── Frame lifecycle ───────────────────────────────────────────────────
            void clear();   ///< Fill with background (black bars + game area).
            void present(); ///< Flip to screen.

            // ── Primitive draw stubs ──────────────────────────────────────────────
            /// Draw a filled rectangle (in logical game coordinates).
            void drawRect(int x, int y, int w, int h, UI::Color color);
            void drawPlate(int x, int y, int w, int h, const std::string text, UI::Color color, UI::Color textColor = UI::WHITE_COLOR);

            /// Draw the outline of a rectangle.
            void drawRectOutline(int x, int y, int w, int h, UI::Color color);

            /// Draw a circle approximation using points.
            void drawCircle(int cx, int cy, int radius, UI::Color color);

            /// Draw a line between two points.
            void drawLine(int x1, int y1, int x2, int y2, UI::Color color);

            /// Draw a UTF-8 text string at (x,y).  Stub — requires SDL2_ttf.
            void drawText(const std::string &text, int x, int y, UI::Color color);
            void drawTextVector(const std::vector<std::string> &lines, int x, int y, UI::Color color, int lineSpacing);

            // ── UI elements ───────────────────────────────────────────────────────
            /// Draw a button with optional hover effect.
            void drawButton(const UI::Button &btn, UI::Color fillColor, UI::Color outlineColor, UI::Color hoverColor);

            /// Draw a filled rectangle background (used for screens).
            void drawBackground(UI::Color color);

            /// Draw a horizontal gradient bar (for title areas, score bars, etc).
            void drawGradientBar(int x, int y, int w, int h, UI::Color colorStart, UI::Color colorEnd, bool vertical = false);

            // ── Accessors ─────────────────────────────────────────────────────────
            [[nodiscard]] SDL_Renderer *raw() const noexcept { return m_renderer.get(); }
            [[nodiscard]] const Viewport &viewport() const noexcept { return m_viewport; }

        private:
            WindowPtr m_window;
            RendererPtr m_renderer;
            Viewport m_viewport;
            FontPtr m_font; 

            /// Recalculate viewport so game area is centred, preserving aspect ratio.
            void recalculateViewport(int windowW, int windowH);


            /// Measure the dimensions of a UTF-8 text string.
            std::pair<int, int> measureText(int x, int y, int width, int height, const std::string &text);
    };

} // namespace core
