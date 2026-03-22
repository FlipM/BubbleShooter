// core/Renderer.cpp
#include "Renderer.hpp"
#include <cmath>
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
                {20, 20, 40, 255});
    }

    void Renderer::present() { SDL_RenderPresent(m_renderer.get()); }

    void Renderer::drawRect(int x, int y, int w, int h, SDL_Color color)
    {
        SDL_SetRenderDrawColor(m_renderer.get(), color.r, color.g, color.b, color.a);
        SDL_Rect rect{x, y, w, h};
        SDL_RenderFillRect(m_renderer.get(), &rect);
    }

    void Renderer::drawRectOutline(int x, int y, int w, int h, SDL_Color color) 
    {
        SDL_SetRenderDrawColor(m_renderer.get(), color.r, color.g, color.b, color.a);
        SDL_Rect rect{x, y, w, h};
        SDL_RenderDrawRect(m_renderer.get(), &rect);
    }

    void Renderer::drawCircle(int cx, int cy, int radius, SDL_Color color) 
    {
        // TODO: replace with textured bubble sprite; for now draw a rough circle.
        SDL_SetRenderDrawColor(m_renderer.get(), color.r, color.g, color.b, color.a);
        for (int dy = -radius; dy <= radius; ++dy) 
        {
            int dx = static_cast<int>(std::sqrt(radius * radius - dy * dy));
            SDL_RenderDrawLine(m_renderer.get(), cx - dx, cy + dy, cx + dx, cy + dy);
        }
    }

    void Renderer::drawLine(int x1, int y1, int x2, int y2, SDL_Color color)
    {
        SDL_SetRenderDrawColor(m_renderer.get(), color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(m_renderer.get(), x1, y1, x2, y2);
    }

    void Renderer::drawText(const std::string &text, int x, int y, SDL_Color color) 
    {
        if (text.empty()) return;

        // Ensure SDL_ttf is initialized (lazy init).
        if (TTF_WasInit() == 0) {
            if (TTF_Init() == -1) {
                throw std::runtime_error(std::string("TTF_Init failed: ") + TTF_GetError());
            }
        }

        // Try a few common font locations. Adjust path or load a font once elsewhere for better control.
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
            font = TTF_OpenFont(fontPaths[i], 24);
            if (font) break;
        }
        if (!font) {
            throw std::runtime_error(std::string("TTF_OpenFont failed: ") + TTF_GetError());
        }

        // Render text to surface, create texture, render, and clean up.
        SDL_Surface *surf = TTF_RenderUTF8_Blended(font, text.c_str(), color);
        if (!surf) {
            TTF_CloseFont(font);
            throw std::runtime_error(std::string("TTF_RenderUTF8_Blended failed: ") + SDL_GetError());
        }

        SDL_Texture *tex = SDL_CreateTextureFromSurface(m_renderer.get(), surf);
        if (!tex) {
            SDL_FreeSurface(surf);
            TTF_CloseFont(font);
            throw std::runtime_error(std::string("SDL_CreateTextureFromSurface failed: ") + SDL_GetError());
        }

        SDL_Rect dst{ x, y, surf->w, surf->h };
        SDL_FreeSurface(surf);

        SDL_RenderCopy(m_renderer.get(), tex, nullptr, &dst);

        SDL_DestroyTexture(tex);
        TTF_CloseFont(font);
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

} // namespace core
