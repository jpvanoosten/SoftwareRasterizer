#pragma once

#include "Player.hpp"

#include <Image.hpp>
#include <Math/Sphere.hpp>
#include <SpriteSheet.hpp>

class Pickup
{
public:
    Pickup() = default;
    Pickup( const sr::SpriteSheet& sprites, const Math::Sphere& collision );

    /// <summary>
    /// Check to see if the player collides with this pickup.
    /// </summary>
    bool collides( const Player& player ) const noexcept
    {
        return player.getAABB().intersect( sphere );
    }

    /// <summary>
    /// Update the pickup animation.
    /// </summary>
    /// <param name="deltaTime"></param>
    void update( float deltaTime );

    /// <summary>
    /// Draw this pickup to the specified image.
    /// </summary>
    /// <param name="image"></param>
    void draw( sr::Image& image ) const;

    /// <summary>
    /// Get the transform of the pickup.
    /// </summary>
    /// <returns></returns>
    const Math::Transform2D getTransform() const noexcept
    {
        return transform;
    }

private:
    // Collision sphere.
    Math::Sphere sphere;

    // Transform for this pickup.
    Math::Transform2D      transform;

    // Shared sprite sheet for the pickup.
    const sr::SpriteSheet* spriteSheet = nullptr;

    // Frame-rate for sprite animation.
    uint32_t frameRate = 20u;

    // Animation time for determining the frame to draw for this pickup.
    float time = 0.0f;
};