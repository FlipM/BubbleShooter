// core/Renderer.cpp
#include "Renderer.hpp"
#include <iostream>


namespace core 
{
    constexpr const char *FONT_PATH = "assets/fonts/DejaVuSans.ttf";
    constexpr int FONT_SIZE = 20;

    /// Initialize the SDL renderer with window.
    Renderer::Renderer(const std::string &title, int windowW, int windowH) 
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) 
        {
            throw std::runtime_error(std::string("SDL_Init failed: ") + SDL_GetError());
        }
        initFont();

        SDL_Window *raw_win = SDL_CreateWindow(
            title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowW,
            windowH, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (!raw_win) {
            throw std::runtime_error(std::string("SDL_CreateWindow failed: ") +
                                    SDL_GetError());
        }
        m_window.reset(raw_win);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

        SDL_Renderer *raw_ren = SDL_CreateRenderer(
            m_window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!raw_ren) {
            throw std::runtime_error(std::string("SDL_CreateRenderer failed: ") +
                                    SDL_GetError());
        }
        m_renderer.reset(raw_ren);
        SDL_RenderSetLogicalSize(m_renderer.get(), windowW, windowH);

        recalculateViewport(windowW, windowH);
    }

    /// Clear frame buffer with background colors.
    void Renderer::clear() 
    {
        SDL_SetRenderDrawColor(m_renderer.get(), 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(m_renderer.get());
        drawRect(m_viewport.x, m_viewport.y, m_viewport.width, m_viewport.height, core::UI::GAME_BACKGROUND);
    }

    /// Present the rendered frame to screen.
    void Renderer::present() 
    { 
        SDL_RenderPresent(m_renderer.get()); 
    }

    /// Draw a filled rectangle with text overlay.
    void Renderer::drawPlate(int x, int y, int w, int h, const std::string text, UI::Color color, UI::Color textColor)
    {
        drawRect(x, y, w, h, color);
        auto textCoords = measureText(x, y, w, h, text);
        drawText(text, textCoords.first, textCoords.second, textColor);
    }

    /// Draw a filled rectangle.
    void Renderer::drawRect(int x, int y, int w, int h, UI::Color color)
    {
        SDL_SetRenderDrawColor(m_renderer.get(), color.r, color.g, color.b, color.a);
        SDL_Rect rect{x, y, w, h};
        SDL_RenderFillRect(m_renderer.get(), &rect);
    }

    /// Draw a rectangle outline.
    void Renderer::drawRectOutline(int x, int y, int w, int h, UI::Color color) 
    {
        SDL_SetRenderDrawColor(m_renderer.get(), color.r, color.g, color.b, color.a);
        SDL_Rect rect{x, y, w, h};
        SDL_RenderDrawRect(m_renderer.get(), &rect);
    }

    /// Draw a filled circle using line rasterization.
    void Renderer::drawCircle(int cx, int cy, int radius, UI::Color color) 
    {
        SDL_SetRenderDrawColor(m_renderer.get(), color.r, color.g, color.b, color.a);
        for (int dy = -radius; dy <= radius; ++dy) 
        {
            int dx = static_cast<int>(std::sqrt(radius * radius - dy * dy));
            SDL_RenderDrawLine(m_renderer.get(), cx - dx, cy + dy, cx + dx, cy + dy);
        }
    }

    /// Draw a line between two points.
    void Renderer::drawLine(int x1, int y1, int x2, int y2, UI::Color color)
    {
        SDL_SetRenderDrawColor(m_renderer.get(), color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(m_renderer.get(), x1, y1, x2, y2);
    }

    /// Render multiple text lines with line spacing.
    void Renderer::drawTextVector(const std::vector<std::string> &lines, int x, int y, UI::Color color, int lineSpacing) 
    {
        for (size_t i = 0; i < lines.size(); ++i) 
        {
            drawText(lines[i], x, y + static_cast<int>(i * lineSpacing), color);
        }
    }

    /// Render a button with label and optional hover effect.
    void Renderer::drawButton(const UI::Button &btn, UI::Color fillColor, UI::Color outlineColor, UI::Color hoverColor)
    {
        UI::Color actualFill = btn.hovered ? hoverColor : fillColor;
        
        drawRect(btn.x, btn.y, btn.width, btn.height, actualFill);
        drawRectOutline(btn.x, btn.y, btn.width, btn.height, outlineColor);
        
        auto textCoordinates = measureText(btn.x, btn.y, btn.width, btn.height, btn.label);
        drawText(btn.label, textCoordinates.first, textCoordinates.second, UI::WHITE_COLOR);
    }

    /// Fill viewport background with specified color.
    void Renderer::drawBackground(UI::Color color)
    {
        drawRect(m_viewport.x, m_viewport.y, m_viewport.width, m_viewport.height, color);
    }

    /// Draw a gradient bar (horizontal or vertical).
    void Renderer::drawGradientBar(int x, int y, int w, int h, UI::Color colorStart, UI::Color colorEnd, bool vertical)
    {
        if (vertical) {
            for (int dy = 0; dy < h; ++dy) {
                float t = static_cast<float>(dy) / h;
                UI::Color color(
                    static_cast<std::uint8_t>(colorStart.r + (colorEnd.r - colorStart.r) * t),
                    static_cast<std::uint8_t>(colorStart.g + (colorEnd.g - colorStart.g) * t),
                    static_cast<std::uint8_t>(colorStart.b + (colorEnd.b - colorStart.b) * t),
                    static_cast<std::uint8_t>(colorStart.a + (colorEnd.a - colorStart.a) * t)
                );
                drawLine(x, y + dy, x + w, y + dy, color);
            }
        } else {
            for (int dx = 0; dx < w; ++dx) {
                float t = static_cast<float>(dx) / w;
                UI::Color color(
                    static_cast<std::uint8_t>(colorStart.r + (colorEnd.r - colorStart.r) * t),
                    static_cast<std::uint8_t>(colorStart.g + (colorEnd.g - colorStart.g) * t),
                    static_cast<std::uint8_t>(colorStart.b + (colorEnd.b - colorStart.b) * t),
                    static_cast<std::uint8_t>(colorStart.a + (colorEnd.a - colorStart.a) * t)
                );
                drawLine(x + dx, y, x + dx, y + h, color);
            }
        }
    }

    /// Render UTF-8 text string at screen position.
    void Renderer::drawText(const std::string &text, int x, int y, UI::Color color) 
    {
        if (text.empty()) return;

        if (!m_font) {
            std::clog << "[Renderer] Font not available, skipping drawText\n";
            return;
        }

        SDL_Color sdlColor{color.r, color.g, color.b, color.a};
        SDL_Surface *surf = TTF_RenderUTF8_Blended(m_font.get(), text.c_str(), sdlColor);
        if (!surf) {
            std::cerr << "[Renderer] TTF_RenderUTF8_Blended failed: " << SDL_GetError() << '\n';
            return;
        }

        SDL_Texture *tex = SDL_CreateTextureFromSurface(m_renderer.get(), surf);
        if (!tex) {
            std::cerr << "[Renderer] SDL_CreateTextureFromSurface failed: " << SDL_GetError() << '\n';
            SDL_FreeSurface(surf);
            return;
        }

        SDL_Rect dst{ x, y, surf->w, surf->h };
        SDL_FreeSurface(surf);
        SDL_RenderCopy(m_renderer.get(), tex, nullptr, &dst);
        SDL_DestroyTexture(tex);
    }

    /// Recalculate viewport maintaining aspect ratio and centering.
    void Renderer::recalculateViewport(int windowW, int windowH) 
    {
        const float gameAspect = static_cast<float>(m_viewport.width) /
                                static_cast<float>(m_viewport.height);
        int gameH = windowH;
        int gameW = static_cast<int>(gameH * gameAspect);

        if (gameW > windowW) {
            gameW = windowW;
            gameH = static_cast<int>(gameW / gameAspect);
        }

        m_viewport.width = gameW;
        m_viewport.height = gameH;
        m_viewport.x = (windowW - gameW) / 2;
        m_viewport.y = (windowH - gameH) / 2;
    }

    /// Calculate center position for text within a bounding box.
    std::pair<int, int> Renderer::measureText(int x, int y, int width, int height, const std::string &text)
    {
        int textWidth, textHeight;
        TTF_SizeUTF8(m_font.get(), text.c_str(), &textWidth, &textHeight);
        
        int textX = x + (width - textWidth) / 2;
        int textY = y + (height - textHeight) / 2;
        return {textX, textY};
    }

    /// Load and initialize TTF font.
    void Renderer::initFont()
    {
        if (m_font) 
            return;

        if (TTF_WasInit() == 0) 
        {
            if (TTF_Init() == -1) 
            {
                std::cerr << "[Renderer] TTF_Init failed: " << TTF_GetError() << '\n';
                return;
            }
        }
        
        TTF_Font *font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
        if (font) 
        {
            std::clog << "[Renderer] Font loaded from: " << FONT_PATH << '\n';
        }
        else
        {
            std::cerr << "[Renderer] Could not load any font. Aborting.\n";
            std::abort();
        }

        m_font.reset(font);
    }

} // namespace core

