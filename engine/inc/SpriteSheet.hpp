#pragma once

#include "Config.hpp"
#include "Image.hpp"
#include "Sprite.hpp"
#include "BlendMode.hpp"

#include <Math/Rect.hpp>

#include <cstdint>
#include <filesystem>
#include <span>

namespace sr
{
class SR_API SpriteSheet
{
public:
    /// <summary>
    /// Default constructor for a sprite sheet.
    /// A default sprite sheet contains no sprites.
    /// </summary>
    SpriteSheet() = default;

    /// <summary>
    /// Create a sprite sheet based on the size of each sprite in the sprite sheet.
    /// </summary>
    /// <param name="fileName">The file path to the image.</param>
    /// <param name="spriteWidth">(optional) The width (in pixels) of a sprite in the sprite sheet. Default: image width.</param>
    /// <param name="spriteHeight">(optional) The height (in pixels) of a sprite in the sprite sheet. Default: image height.</param>
    /// <param name="blendMode">(optional) The blend mode to use when rendering the sprites in this sprite sheet. Default: No blending.</param>
    SpriteSheet( const std::filesystem::path& fileName, std::optional<uint32_t> spriteWidth = {}, std::optional<uint32_t> spriteHeight = {}, const BlendMode& blendMode = {} );

    /// <summary>
    /// Create a sprite sheet based on a set of rectangles in the source image.
    /// </summary>
    /// <param name="fileName">The file path to the image.</param>
    /// <param name="rects">The rectangles for the sprites in the sprite sheet.</param>
    /// <param name="blendMode">(optional) The blend mode to use when rendering the sprites in this sprite sheet. Default: No blending.</param>
    SpriteSheet( const std::filesystem::path& fileName, std::span<const Math::RectI> rects, const BlendMode& blendMode = {} );

    /// <summary>
    /// Copy constructor.
    /// </summary>
    /// <param name="copy">The sprite sheet to copy.</param>
    SpriteSheet( const SpriteSheet& copy ) = default;

    /// <summary>
    /// Move constructor.
    /// </summary>
    /// <param name="other">The sprite sheet to move from.</param>
    SpriteSheet( SpriteSheet&& other ) noexcept;

    /// <summary>
    /// Copy assignment operator.
    /// </summary>
    /// <param name="copy">The sprite sheet to copy.</param>
    /// <returns></returns>
    SpriteSheet& operator=( const SpriteSheet& copy ) = default;

    /// <summary>
    /// Move assignment operator.
    /// </summary>
    /// <param name="other">The sprite sheet to move from.</param>
    /// <returns></returns>
    SpriteSheet& operator=( SpriteSheet&& other ) noexcept;

    /// <summary>
    /// Get the number of sprites in the sprite sheet.
    /// </summary>
    /// <returns>The number of sprites in the sprite sheet.</returns>
    size_t getNumSprites() const noexcept
    {
        return sprites.size();
    }

    /// <summary>
    /// Get the number of rows in the sprite sheet.
    /// </summary>
    /// <returns>The number of rows in the sprite sheet.</returns>
    size_t getNumRows() const noexcept
    {
        return rows;
    }

    /// <summary>
    /// Get the number of columns in the sprite sheet.
    /// </summary>
    /// <returns>The number of columns in the sprite sheet.</returns>
    size_t getNumColumns() const noexcept
    {
        return columns;
    }

    /// <summary>
    /// Get the width (in pixels) of each sprite in the sprite sheet.
    /// </summary>
    /// <returns>The width of the sprites in the sprite sheet.</returns>
    size_t getSpriteWidth() const noexcept
    {
        return image.getWidth() / columns;
    }

    /// <summary>
    /// Get the height (in pixels) of each sprite in the sprite sheet.
    /// </summary>
    /// <returns>The height of the sprites in the sprite sheet.</returns>
    size_t getSpriteHeight() const noexcept
    {
        return image.getHeight() / rows;
    }

    /// <summary>
    /// Retrieve a sprite from the sprite sheet.
    /// </summary>
    /// <param name="index">The 1D index of the sprite in the sprite sheet.</param>
    /// <returns>The sprite at the given index, or a default "empty" sprite if index is out of range.</returns>
    const Sprite& operator[]( size_t index ) const noexcept;

    /// <summary>
    /// Retrieve a sprite from the sprite sheet.
    /// </summary>
    /// <param name="i">The row index.</param>
    /// <param name="j">The column index.</param>
    /// <returns>The sprite at the given coordinates, or an "empty" sprite if the indices are out of range.</returns>
    const Sprite& operator()( size_t i, size_t j ) const noexcept;

    /// <summary>
    /// Load a SpriteSheet from a grid of sprites.
    /// </summary>
    /// <param name="fileName">The path to the image file that contains the sprites.</param>
    /// <param name="columns">The number of sprites in the x-axis.</param>
    /// <param name="rows">(optional) The number of sprites in the y-axis. Default: 1</param>
    /// <param name="blendMode">(optional) The blending to apply to the sprites. Default: blending disabled.</param>
    /// <returns></returns>
    static SpriteSheet fromGrid( const std::filesystem::path& fileName, uint32_t columns, uint32_t rows = 1, const BlendMode& blendMode = {} );

private:
    // Create a sprite sheet from a pre-loaded sprite image.
    void init( Image image, uint32_t spriteWidth, uint32_t spriteHeight, const BlendMode& blendMode );

    // The image that contains the sprites.
    Image image;

    // The sprites in the sprite sheet.
    std::vector<Sprite> sprites;

    // The number of sprites in the X-axis of the image.
    uint32_t columns = 0u;
    // The number of sprites in the Y-axis of the image.
    uint32_t rows    = 0u;
};

}  // namespace sr
