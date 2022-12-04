#pragma once

#include "Config.hpp"
#include "Image.hpp"

#include <glm/vec2.hpp>

namespace sr
{
class SR_API Sprite
{
public:
    Sprite() = default;

    /// <summary>
    /// Construct a Sprite from an Image.
    /// </summary>
    /// <param name="image">The image to create the sprite from.</param>
    explicit Sprite( const Image& image ) noexcept
    : image { &image }
    , uv { 0, 0 }
    , size { image.getWidth(), image.getHeight() }
    {}

    /// <summary>
    /// Construct a sprite from a region of the image.
    /// </summary>
    /// <param name="image">The image that contains the sprite sheet.</param>
    /// <param name="uv">The UV coordinates of the sprite in the image.</param>
    /// <param name="size">The size of the sprite in the image.</param>
    Sprite( const Image& image, const glm::ivec2& uv, const glm::ivec2& size ) noexcept
    : image { &image }
    , uv { uv }
    , size { size }
    {}

    const glm::ivec2& getUV() const noexcept
    {
        return uv;
    }

    const glm::ivec2& getSize() const noexcept
    {
        return size;
    }

    const Image* getImage() const noexcept
    {
        return image;
    }

    const Color& getColor() const noexcept
    {
        return color;
    }

    void setColor(const Color& _color) noexcept
    {
        color = _color;
    }

private:
    // The image that stores the pixels for this sprite.
    const Image* image = nullptr;

    // The UV coords of the sprite in the image.
    glm::ivec2 uv { 0 };
    // The size of the sprite in the image.
    glm::ivec2 size { 0 };
    // The color to apply to the sprite.
    Color color { Color::White };
};

}  // namespace sr
