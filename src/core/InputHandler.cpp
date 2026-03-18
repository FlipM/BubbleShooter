// core/InputHandler.cpp
#include "InputHandler.hpp"

namespace core {

    bool InputHandler::pollEvents() 
    {
        resetFrameState();

        SDL_Event event;
        while (SDL_PollEvent(&event)) 
        {
            switch (event.type) 
            {
                case SDL_QUIT:
                return false;

                case SDL_KEYDOWN:
                if (!event.key.repeat) 
                {
                    m_keysDown[event.key.keysym.sym] = true;
                    m_keysPressed[event.key.keysym.sym] = true;
                }
                break;

                case SDL_KEYUP:
                m_keysDown[event.key.keysym.sym] = false;
                break;

                case SDL_MOUSEMOTION:
                m_mouse.x = event.motion.x;
                m_mouse.y = event.motion.y;
                break;

                case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) 
                {
                    m_mouse.leftDown = true;
                }
                break;

                case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) 
                {
                    m_mouse.leftDown = false;
                    m_mouse.leftClicked = true; // single-frame pulse
                }
                break;

                default:
                break; // TODO: handle SDL_WINDOWEVENT_RESIZED → notify Renderer
            }
        }
        return true; // still running
    }

    bool InputHandler::isKeyDown(SDL_Keycode key) const 
    {
        auto it = m_keysDown.find(key);
        return (it != m_keysDown.end()) && it->second;
    }

    bool InputHandler::wasKeyPressed(SDL_Keycode key) const 
    {
        auto it = m_keysPressed.find(key);
        return (it != m_keysPressed.end()) && it->second;
    }

    void InputHandler::resetFrameState()
    {
        m_keysPressed.clear();
        m_mouse.leftClicked = false;
    }

} // namespace core
