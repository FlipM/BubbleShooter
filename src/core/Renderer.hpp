// core/Renderer.hpp
// RAII wrapper around SDL_Window + SDL_Renderer.
#pragma once

#include "UI.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

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
        int x{0}, y{0};      ///< top-left offset inside the OS window.
        int width{390};      ///< logical game width.
        int height{844};     ///< logical game height.
    };

    class Renderer 
    {
        public:
            explicit Renderer(const std::string &title, int windowW = 1080,
                                int windowH = 844);
            ~Renderer() = default;

            // Non-copyable, movable.
            Renderer(const Renderer &) = delete;
            Renderer &operator=(const Renderer &) = delete;
            Renderer(Renderer &&) = default;
            Renderer &operator=(Renderer &&) = default;

            // ── Utility ───────────────────────────────────────────────────────────
            void initFont();
            void clear();
            void present();

            // ── Primitives ────────────────────────────────────────────────────────
            void drawRect(int x, int y, int w, int h, UI::Color color);
            void drawRectOutline(int x, int y, int w, int h, UI::Color color);
            void drawCircle(int cx, int cy, int radius, UI::Color color);
            void drawLine(int x1, int y1, int x2, int y2, UI::Color color);

            // ── Text ───────────────────────────────────────────────────────────────
            void drawText(const std::string &text, int x, int y, UI::Color color);
            void drawTextVector(const std::vector<std::string> &lines, int x, int y, UI::Color color, int lineSpacing);

            // ── UI Elements ────────────────────────────────────────────────────────
            void drawPlate(int x, int y, int w, int h, const std::string text, UI::Color color, UI::Color textColor = UI::WHITE_COLOR);
            void drawButton(const UI::Button &btn, UI::Color fillColor, UI::Color outlineColor, UI::Color hoverColor);
            void drawBackground(UI::Color color);
            void drawGradientBar(int x, int y, int w, int h, UI::Color colorStart, UI::Color colorEnd, bool vertical = false);

            // ── Accessors ──────────────────────────────────────────────────────────
            [[nodiscard]] SDL_Renderer *raw() const noexcept { return m_renderer.get(); }
            [[nodiscard]] const Viewport &viewport() const noexcept { return m_viewport; }

        private:
            WindowPtr m_window;
            RendererPtr m_renderer;
            Viewport m_viewport;
            FontPtr m_font;

            void recalculateViewport(int windowW, int windowH);
            std::pair<int, int> measureText(int x, int y, int width, int height, const std::string &text);
    };

} // namespace core
