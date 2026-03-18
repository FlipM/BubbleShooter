// core/Renderer.cpp
#include "Renderer.hpp"
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

    void Renderer::drawText(const std::string & /*text*/, int /*x*/, int /*y*/,
                            SDL_Color /*color*/) 
    {
        // TODO: integrate SDL2_ttf — load font via ResourceManager,
        // TTF_RenderUTF8_Blended, etc. Placeholder: does nothing until SDL2_ttf is
        // wired in.
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
