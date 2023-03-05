#pragma once

#include "Config.hpp"
#include "Font.hpp"
#include "Image.hpp"
#include "SpriteSheet.hpp"

#include <filesystem>
#include <memory>

namespace Graphics
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
    /// Load a sprite sheet from a file.
    /// </summary>
    /// <param name="filePath">The file path to the image.</param>
    /// <param name="spriteWidth">(optional) The width (in pixels) of a sprite in the sprite sheet. Default: image width.</param>
    /// <param name="spriteHeight">(optional) The height (in pixels) of a sprite in the sprite sheet. Default: image height.</param>
    /// <param name="padding">(optional) The amount of space (in pixels) between each sprite in the sprite sheet. Default: 0.</param>
    /// <param name="margin">(optional) The amount of space (in pixels) around the entire image. Default: 0.</param>
    /// <param name="blendMode">(optional) The blend mode to use when rendering the sprites in this sprite sheet. Default: No blending.</param>
    /// <returns>The loaded SpriteSheet.</returns>
    static std::shared_ptr<SpriteSheet> loadSpriteSheet( const std::filesystem::path& filePath, std::optional<uint32_t> spriteWidth = {}, std::optional<uint32_t> spriteHeight = {}, uint32_t padding = 0u, uint32_t margin = 0u, const BlendMode& blendMode = {} );

    /// <summary>
    /// Load a font from a file.
    /// </summary>
    /// <param name="fontFile">The path to the font to load.</param>
    /// <param name="size">(optional) The size of the font (in pixels). Default: 12</param>
    /// <param name="firstChar">(optional) The first character in the font texture. Default: ' '.</param>
    /// <param name="numChars">(optional) The number of characters in the font texture. Default: 96.</param>
    /// <returns>A shared pointer to the loaded font.</returns>
    static std::shared_ptr<Font> loadFont( const std::filesystem::path& fontFile, float size = 12.0f, uint32_t firstChar = 32u, uint32_t numChars = 96u );

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

}  // namespace Graphics
