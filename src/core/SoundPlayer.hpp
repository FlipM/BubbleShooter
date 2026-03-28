#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#ifdef HAS_SDL2_MIXER
#include <SDL2/SDL_mixer.h>
#endif

namespace core 
{
    class SoundPlayer
    {
        public:
            SoundPlayer() = default;
            ~SoundPlayer() = default;

#ifdef HAS_SDL2_MIXER
            void init();

            struct SoundDeleter 
            {
                void operator()(Mix_Chunk *s) const noexcept { Mix_FreeChunk(s); }
            };

            using SoundPtr = std::shared_ptr<Mix_Chunk>;

            /// Returns a sound pointer or nullptr if file is missing
            [[nodiscard]] SoundPtr getSound(const std::string &filePath);
                
            /// Play a sound effect on a free channel (respects volume).
            /// Returns the channel used (-1 if no channels available).
            int playSound(SoundPtr sound, int loops = 0);

            /// Stop all sounds and music.
            void stopAllAudio();

            /// Set master volume for sound effects (0.0 – 1.0).
            void setSoundVolume(float volume);

#endif
          
        private:
            float m_soundVolume{1.0f};  ///< 0.0 – 1.0      
    };

} // namespace core  
