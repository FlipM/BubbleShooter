// core/ResourceManager.cpp
#include "ResourceManager.hpp"
#include <iostream>

namespace core 
{

    ResourceManager::ResourceManager(SDL_Renderer *renderer)
        : m_renderer(renderer) {}

    TexturePtr ResourceManager::getTexture(const std::string &id,
                                const std::string &filePath) 
    {
        if (auto it = m_textures.find(id); it != m_textures.end()) 
        {
            return it->second; // cache hit
        }

        // TODO: use SDL_image (IMG_LoadTexture) when HAS_SDL2_IMAGE is defined.
        std::clog << "[ResourceManager] stub load texture: " << filePath << '\n';
        // Return nullptr placeholder — callers must null-check before use.
        m_textures[id] = nullptr;
        return nullptr;
    }

    void ResourceManager::unload(const std::string &id) { m_textures.erase(id); }

    void ResourceManager::clear() { m_textures.clear(); }

} // namespace core
