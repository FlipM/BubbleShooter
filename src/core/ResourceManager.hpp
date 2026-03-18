// core/ResourceManager.hpp
// Caches SDL textures and TTF fonts by string ID using shared_ptr.
#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <unordered_map>

namespace core {

    struct TextureDeleter 
    {
        void operator()(SDL_Texture *t) const noexcept { SDL_DestroyTexture(t); }
    };
    using TexturePtr = std::shared_ptr<SDL_Texture>;

    class ResourceManager 
    {
        public:
            explicit ResourceManager(SDL_Renderer *renderer);
            ~ResourceManager() = default;

            // Non-copyable.
            ResourceManager(const ResourceManager &) = delete;
            ResourceManager &operator=(const ResourceManager &) = delete;

            /// Returns cached texture, loading from `filePath` on first access.
            /// Returns nullptr (stub) if file is missing — caller must handle.
            [[nodiscard]] TexturePtr getTexture(const std::string &id,
                                                const std::string &filePath);

            /// Invalidates a single resource.
            void unload(const std::string &id);

            /// Purge all cached resources.
            void clear();

            // TODO: getFont(id, path, ptSize) → TTF_Font wrapper

        private:
            SDL_Renderer *m_renderer{nullptr}; ///< Non-owning — lifetime managed by Renderer.
            std::unordered_map<std::string, TexturePtr> m_textures;
    };

} // namespace core
