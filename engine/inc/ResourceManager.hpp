#pragma once

#include "Config.hpp"
#include "Image.hpp"
#include "SpriteSheet.hpp"

#include <filesystem>
#include <memory>

namespace sr
{
class SR_API ResourceManager final
{
public:
    /// <summary>
    /// Load an image from a file.
    /// </summary>
    /// <param name="filePath">The path to the file to load.</param>
    /// <returns>The loaded image.</returns>
    static std::shared_ptr<Image> loadImage( const std::filesystem::path& filePath );

    /// <summary>
    /// 
    /// </summary>
    /// <param name="filePath">The file path to the image.</param>
    /// <param name="spriteWidth">(optional) The width (in pixels) of a sprite in the sprite sheet. Default: image width.</param>
    /// <param name="spriteHeight">(optional) The height (in pixels) of a sprite in the sprite sheet. Default: image height.</param>
    /// <param name="blendMode">(optional) The blend mode to use when rendering the sprites in this sprite sheet. Default: No blending.</param>
    /// <returns>The loaded SpriteSheet.</returns>
    static std::shared_ptr<SpriteSheet> loadSpriteSheet( const std::filesystem::path& filePath, std::optional<uint32_t> spriteWidth = {}, std::optional<uint32_t> spriteHeight = {}, const BlendMode blendMode = {} );

    /// <summary>
    /// Unload all resources.
    /// </summary>
    static void clear();

    // Singleton class.
    ResourceManager()                         = delete;
    ~ResourceManager()                        = delete;
    ResourceManager( const ResourceManager& ) = delete;
    ResourceManager( ResourceManager&& )      = delete;

    ResourceManager& operator=( const ResourceManager& ) = delete;
    ResourceManager& operator=( ResourceManager&& )      = delete;
};

}  // namespace sr
