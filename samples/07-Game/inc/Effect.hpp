#pragma once

#include <Image.hpp>
#include <SpriteAnim.hpp>

#include <Math/Transform2D.hpp>

class Effect final
{
public:
    Effect() = default;
    Effect( const sr::SpriteAnim& spriteAnim, const Math::Transform2D& transform );

    /// <summary>
    /// Update the effect's animation.
    /// </summary>
    /// <param name="deltaTime">The elapsed time (in seconds)</param>
    void update( float deltaTime );

    /// <summary>
    /// Draw the effect to the image.
    /// </summary>
    /// <param name="image">The image to draw the effect to.</param>
    void draw( sr::Image& image ) const;

    /// <summary>
    /// Check if the effect is done playing.
    /// This is useful for 1-shot effects that should only play once and be removed.
    /// </summary>
    /// <returns></returns>
    bool isDone() const noexcept;

private:
    const sr::SpriteAnim*  spriteAnim = nullptr;
    Math::Transform2D      transform;
    float                  time = 0.0f;
};