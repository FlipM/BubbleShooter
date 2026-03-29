// core/ResourceManager.cpp
#include "ResourceManager.hpp"
#include <iostream>

namespace core 
{

    constexpr const char *SOUND_PATH = "assets/sounds/"; ///< Path to sound files.
    constexpr const char *SOUND_EXT = ".wav";            ///< Sound file extension.

    /// Initialize with references to renderer and sound player.
    ResourceManager::ResourceManager(Renderer &renderer, SoundPlayer &soundPlayer) 
        : m_renderer(renderer), m_soundPlayer(soundPlayer) {}

    /// Clear all cached sound effects.
    void ResourceManager::clear() 
    {
#ifdef HAS_SDL2_MIXER
        m_sounds.clear();
#endif
    }

    /// Load and play a sound effect by ID.
    /// Caches sound on first access.
    void ResourceManager::play(const std::string &id, int loops) 
    {

#ifdef HAS_SDL2_MIXER
            
            SoundPlayer::SoundPtr sound = nullptr;
            auto it = m_sounds.find(id);
            if(it != m_sounds.end())
            {
                sound = it->second;
            }
            else 
            {
                std::string filePath = SOUND_PATH + id + SOUND_EXT;
                sound = m_soundPlayer.getSound(filePath);
                if(sound)
                {
                    m_sounds[id] = sound;
                }
            }
            m_soundPlayer.playSound(sound, loops);
#endif
    }

} // namespace core