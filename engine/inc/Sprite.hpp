#pragma once

#include "BlendMode.hpp"
#include "Config.hpp"
#include "Image.hpp"

#include <Math/Rect.hpp>

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
    /// <param name="blendMode">The blend mode to apply when rendering.</param>
    explicit Sprite( const Image& image, const BlendMode& blendMode = {} ) noexcept
    : image { &image }
    , rect { 0, 0, static_cast<int32_t>( image.getWidth() ), static_cast<int32_t>( image.getHeight() ) }
    , blendMode { blendMode }
    {}

    /// <summary>
    /// Construct a sprite from a region of the image.
    /// </summary>
    /// <param name="image">The image that contains the sprite sheet.</param>
    /// <param name="rect">The source rectangle of this sprite in the image.</param>
    /// <param name="blendMode">The blend mode to apply when rendering.</param>
    Sprite( const Image& image, const Math::RectI& rect, const BlendMode& blendMode = {} ) noexcept
    : image { &image }
    , rect{rect}
    , blendMode { blendMode }
    {}

    glm::ivec2 getUV() const noexcept
    {
        return { rect.left, rect.top };
    }

    glm::ivec2 getSize() const noexcept
    {
        return { rect.width, rect.height };
    }

    const Math::RectI& getRect() const noexcept
    {
        return rect;
    }

    const Image* getImage() const noexcept
    {
        return image;
    }

    const Color& getColor() const noexcept
    {
        return color;
    }

    void setColor( const Color& _color ) noexcept
    {
        color = _color;
    }

    const BlendMode& getBlendMode() const noexcept
    {
        return blendMode;
    }

    void setBlendMode( const BlendMode& _blendMode )
    {
        blendMode = _blendMode;
    }

private:
    // The image that stores the pixels for this sprite.
    const Image* image = nullptr;

    // The source rectangle of this sprite in the image.
    Math::RectI rect;

    // The color to apply to the sprite.
    Color color { Color::White };

    // The blend mode to apply when rendering.
    BlendMode blendMode;
};

}  // namespace sr
