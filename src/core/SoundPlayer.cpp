#include "SoundPlayer.hpp"
#include <iostream>

#ifdef HAS_SDL2_MIXER
namespace core 
{
    void SoundPlayer::init() 
    {
        // Initialize audio: 44.1kHz, stereo, 16-bit, 2048-byte chunks
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) 
        {
            std::cerr << "[Game] failed to initialize audio: " << Mix_GetError() << '\n';
        }
        else 
        {
            std::clog << "[Game] audio initialized successfully\n";
            Mix_AllocateChannels(16);
        }
        return;
    }

    SoundPlayer::SoundPtr SoundPlayer::getSound(const std::string &filePath) 
    {
        Mix_Chunk *chunk = Mix_LoadWAV(filePath.c_str());
        if (!chunk) 
        {
            std::cerr << "[ResourceManager] failed to load sound: " << filePath 
                      << " | " << Mix_GetError() << '\n';
            return nullptr;
        }

        std::clog << "[ResourceManager] loaded sound: " << filePath << '\n';
        return SoundPlayer::SoundPtr(chunk, SoundPlayer::SoundDeleter());;
    }

    int SoundPlayer::playSound(SoundPtr sound, int loops) 
    {
        if (!sound) 
        {
            return -1;
        }
        return Mix_PlayChannel(-1, sound.get(), loops);
    }


    void SoundPlayer::stopAllAudio() 
    {
        Mix_HaltChannel(-1);  // stop all sound effects
        Mix_HaltMusic();      // stop music
    }

    void SoundPlayer::setSoundVolume(float volume) 
    {
        m_soundVolume = volume;
        // Set volume for all channels (0-128 scale)
        Mix_Volume(-1, static_cast<int>(volume * MIX_MAX_VOLUME));
    }

} // namespace core
#endif