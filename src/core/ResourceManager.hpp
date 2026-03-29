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
    class ResourceManager 
    {
        public:
            explicit ResourceManager(Renderer &renderer, SoundPlayer &soundPlayer);
            ~ResourceManager() = default;

            // Non-copyable, movable.
            ResourceManager(const ResourceManager &) = delete;
            ResourceManager &operator=(const ResourceManager &) = delete;
            ResourceManager(ResourceManager &&) = default;
            ResourceManager &operator=(ResourceManager &&) = default;

            // ── Accessors ──────────────────────────────────────────────────────────
            [[nodiscard]] Renderer *getRenderer() { return &m_renderer; }

            // ── Resource Management ────────────────────────────────────────────────
            void clear();
            void play(const std::string &id, int loops = 0);

        private:
            Renderer &m_renderer;
            SoundPlayer &m_soundPlayer;
        #ifdef HAS_SDL2_MIXER
            std::unordered_map<std::string, SoundPlayer::SoundPtr> m_sounds; ///< Sound effect cache.
        #endif

    };

} // namespace core