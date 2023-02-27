#pragma once

#include "Config.hpp"
#include "SpriteSheet.hpp"

namespace Graphics
{
class SR_API SpriteAnim
{
public:
    SpriteAnim() = default;

    /// <summary>
    /// Create a SpriteAnim based on a sprite sheet and a frame-rate for the animation.
    /// </summary>
    /// <param name="spriteSheet">The sprite sheet that contains the sprites for the animation.</param>
    /// <param name="fps">(optional) The frame-rate of the animation. Default: 30 FPS.</param>
    /// <param name="frames">(optional) The frames of the sprites in the sprite sheet. Default: Uses all of sprites of the sprite sheet.</param>
    explicit SpriteAnim( std::shared_ptr<SpriteSheet> spriteSheet, float fps = 30.0f, std::span<const int> frames = {} );

    /// <summary>
    /// Allow conversion to sprite.
    /// </summary>
    /// <returns>The current Sprite frame to render for this Sprite animation.</returns>
    operator const Sprite&() const noexcept;

    /// <summary>
    /// Get a sprite from the sprite sheet.
    /// </summary>
    /// <param name="i">The index of the sprite in the sprite sheet.
    /// The sprite returned is `i % numSprites`.
    /// </param>
    /// <returns>The sprite in the sprite sheet.</returns>
    const Sprite& operator[]( size_t i ) const noexcept;

    /// <summary>
    /// Get the sprite at a specific moment in time.
    /// </summary>
    /// <param name="time">The animation time (in seconds).</param>
    /// <returns>A reference to the sprite at the given time.</returns>
    const Sprite& at( float time ) const noexcept;

    /// <summary>
    /// Update the internal timer for the sprite animation.
    /// The frame of the sprite animation is chosen based on the animation timer.
    /// </summary>
    /// <param name="deltaTime">The elapsed time in seconds.</param>
    void update( float deltaTime ) noexcept;

    /// <summary>
    /// Reset the internal frame timer.
    /// </summary>
    void reset() noexcept;

    /// <summary>
    /// Get the duration (in seconds) of the animation.
    /// </summary>
    /// <returns>The duration of the animation (in seconds).</returns>
    float getDuration() const noexcept;

    /// <summary>
    /// For "one shot" sprite animations, the animation is done
    /// when all the frames in the sprite animation have played.
    /// </summary>
    /// <returns>`true` if all of the frames of the animation have played at least once, `false` otherwise.</returns>
    bool isDone() const noexcept;

private:
    std::shared_ptr<SpriteSheet> spriteSheet;
    std::vector<int>             frames;
    float                        frameRate = 30.0f;  // Default to 30 FPS.
    float                        time      = 0.0f;
};
}  // namespace Graphics
