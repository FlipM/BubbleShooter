// core/Renderer.cpp
#include "Renderer.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

namespace core {

    Renderer::Renderer(const std::string &title, int windowW, int windowH) 
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) 
        {
            throw std::runtime_error(std::string("SDL_Init failed: ") + SDL_GetError());
        }

        SDL_Window *raw_win = SDL_CreateWindow(
            title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowW,
            windowH, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (!raw_win) {
            throw std::runtime_error(std::string("SDL_CreateWindow failed: ") +
                                    SDL_GetError());
        }
        m_window.reset(raw_win);

        SDL_Renderer *raw_ren = SDL_CreateRenderer(
            m_window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!raw_ren) {
            throw std::runtime_error(std::string("SDL_CreateRenderer failed: ") +
                                    SDL_GetError());
        }
        m_renderer.reset(raw_ren);

        recalculateViewport(windowW, windowH);
    }

    void Renderer::clear() 
    {
        // Fill whole window black (letterbox bars).
        SDL_SetRenderDrawColor(m_renderer.get(), 0, 0, 0, 255);
        SDL_RenderClear(m_renderer.get());

        // Fill game viewport with a dark-blue background.
        drawRect(m_viewport.x, m_viewport.y, m_viewport.width, m_viewport.height,
                UI::Color(20, 20, 40, 255));
    }

    void Renderer::present() { SDL_RenderPresent(m_renderer.get()); }

    void Renderer::drawPlate(int x, int y, int w, int h, const std::string text, UI::Color color, UI::Color textColor)
    {
        // Draw a filled rectangle with a white outline to simulate a plate.
        drawRect(x, y, w, h, color);
        //drawRectOutline(x, y, w, h, textColor);
        auto textCoords = measureText(x, y, w, h, text);
        drawText(text, textCoords.first, textCoords.second, textColor);
    }

    void Renderer::drawRect(int x, int y, int w, int h, UI::Color color)
    {
        SDL_SetRenderDrawColor(m_renderer.get(), color.r, color.g, color.b, color.a);
        SDL_Rect rect{x, y, w, h};
        SDL_RenderFillRect(m_renderer.get(), &rect);
    }

    void Renderer::drawRectOutline(int x, int y, int w, int h, UI::Color color) 
    {
        SDL_SetRenderDrawColor(m_renderer.get(), color.r, color.g, color.b, color.a);
        SDL_Rect rect{x, y, w, h};
        SDL_RenderDrawRect(m_renderer.get(), &rect);
    }

    void Renderer::drawCircle(int cx, int cy, int radius, UI::Color color) 
    {
        // TODO: replace with textured bubble sprite; for now draw a rough circle.
        SDL_SetRenderDrawColor(m_renderer.get(), color.r, color.g, color.b, color.a);
        for (int dy = -radius; dy <= radius; ++dy) 
        {
            int dx = static_cast<int>(std::sqrt(radius * radius - dy * dy));
            SDL_RenderDrawLine(m_renderer.get(), cx - dx, cy + dy, cx + dx, cy + dy);
        }
    }

    void Renderer::drawLine(int x1, int y1, int x2, int y2, UI::Color color)
    {
        SDL_SetRenderDrawColor(m_renderer.get(), color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(m_renderer.get(), x1, y1, x2, y2);
    }

    void Renderer::drawTextVector(const std::vector<std::string> &lines, int x, int y, UI::Color color, int lineSpacing) 
    {
        for (size_t i = 0; i < lines.size(); ++i) 
        {
            drawText(lines[i], x, y + static_cast<int>(i * lineSpacing), color);
        }
    }

    void Renderer::drawText(const std::string &text, int x, int y, UI::Color color) 
    {
        if (text.empty()) return;

        // Lazy-load font on first use.
        ensureFontLoaded();

        if (!m_font) {
            // Font loading failed; silently skip text rendering.
            std::clog << "[Renderer] Font not available, skipping drawText\n";
            return;
        }

        // Render text to surface, create texture, render, and clean up.
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

    void Renderer::recalculateViewport(int windowW, int windowH) 
    {
        // Keep the game area portrait (9:19.5 ≈ phone ratio). Scale to fit height.
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

    void Renderer::drawButton(const UI::Button &btn, UI::Color fillColor, UI::Color outlineColor, UI::Color hoverColor)
    {
        // Use hover color if button is hovered.
        UI::Color actualFill = btn.hovered ? hoverColor : fillColor;
        
        // Draw fill.
        drawRect(btn.x, btn.y, btn.width, btn.height, actualFill);
        
        // Draw outline.
        drawRectOutline(btn.x, btn.y, btn.width, btn.height, outlineColor);
        
        // Draw label text centered in button.
        auto textCoordinates = measureText(btn.x, btn.y, btn.width, btn.height, btn.label);
        drawText(btn.label, textCoordinates.first, textCoordinates.second, UI::Color(255, 255, 255, 255));
    }

    void Renderer::drawBackground(UI::Color color)
    {
        drawRect(m_viewport.x, m_viewport.y, m_viewport.width, m_viewport.height, color);
    }

    void Renderer::drawGradientBar(int x, int y, int w, int h, UI::Color colorStart, UI::Color colorEnd, bool vertical)
    {
        if (vertical) {
            // Vertical gradient (top to bottom)
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
            // Horizontal gradient (left to right)
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

    std::pair< int, int > Renderer::measureText(int x, int y, int width, int height, const std::string &text)
    {
        int textWidth, textHeight;
        TTF_SizeUTF8(m_font.get(), text.c_str(), &textWidth, &textHeight);
        
        // Calculate center position
        int textX = x + (width - textWidth) / 2;
        int textY = y + (height - textHeight) / 2;
        return {textX, textY};
    }

    void Renderer::ensureFontLoaded()
    {
        // Already loaded.
        if (m_font) 
        {
            return;
        }

        // Ensure SDL_ttf is initialized.
        if (TTF_WasInit() == 0) {
            if (TTF_Init() == -1) {
                std::cerr << "[Renderer] TTF_Init failed: " << TTF_GetError() << '\n';
                return;
            }
        }

        // Try a few common font locations (platform-specific).
        const char *fontPaths[] = 
        {
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", // Linux
            "/Library/Fonts/Arial.ttf",                        // macOS
            "C:\\Windows\\Fonts\\arial.ttf",                   // Windows
            nullptr
        };

        TTF_Font *font = nullptr;
        for (int i = 0; fontPaths[i] != nullptr; ++i) 
        {
            font = TTF_OpenFont(fontPaths[i], 20);
            if (font) {
                std::clog << "[Renderer] Font loaded from: " << fontPaths[i] << '\n';
                break;
            }
        }

        if (!font) {
            std::cerr << "[Renderer] Could not load any font\n";
            return;
        }

        m_font.reset(font);
    }

} // namespace core
