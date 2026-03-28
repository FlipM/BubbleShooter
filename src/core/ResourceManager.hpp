// core/ResourceManager.hpp
// Centralize the loading and caching of textures, fonts, and audio resources. 
// Uses shared_ptr to allow multiple users to share ownership of the same resource
#pragma once

#include "Renderer.hpp"
#include "SoundPlayer.hpp"
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <unordered_map>


namespace core 
{
    struct TextureDeleter 
    {
        void operator()(SDL_Texture *t) const noexcept { SDL_DestroyTexture(t); }
    };

    class ResourceManager 
    {
        const std::string FONT_PATH = "assets/fonts/";
        const std::string SOUND_PATH = "assets/sounds/";
        const std::string SOUND_EXT = ".wav";

        public:
            explicit ResourceManager(Renderer &renderer, SoundPlayer &soundPlayer);
            ~ResourceManager() = default;

            // Non-copyable.
            ResourceManager(const ResourceManager &) = delete;
            ResourceManager &operator=(const ResourceManager &) = delete;

            Renderer *getRenderer() { return &m_renderer; } 
            
            

            /// Purge all cached resources.
            void clear();

            /// Play a sound effect by ID, loading from file on first access.
            void play(const std::string &id, int loops = 0);


        private:
            Renderer &m_renderer;
            SoundPlayer &m_soundPlayer;
        #ifdef HAS_SDL2_MIXER
            std::unordered_map<std::string, SoundPlayer::SoundPtr> m_sounds;
        #endif
    };

} // namespace core