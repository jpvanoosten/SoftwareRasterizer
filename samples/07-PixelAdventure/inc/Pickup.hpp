#pragma once

#include "Player.hpp"

#include <Graphics/Image.hpp>
#include <Graphics/SpriteSheet.hpp>
#include <Math/Sphere.hpp>

class Pickup final
{
public:
    Pickup() = default;
    Pickup( std::shared_ptr<sr::SpriteSheet> sprites, const Math::Sphere& collision );
    ~Pickup() = default;

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
    /// Set the gravity for the pickup.
    /// Set to 0 to disable gravity.
    /// </summary>
    /// <param name="g">The gravity to apply.</param>
    void setGravity( float g )
    {
        gravity = g;
    }

    /// <summary>
    /// Get the transform of the pickup.
    /// </summary>
    /// <returns></returns>
    const Math::Transform2D& getTransform() const noexcept
    {
        return transform;
    }

    const Math::Sphere& getCollider() const noexcept
    {
        return sphere;
    }

    const glm::vec2& getPosition() const noexcept
    {
        return transform.getPosition();
    }

    void setPosition( const glm::vec2& pos )
    {
        transform.setPosition( pos );
        sphere.center = glm::vec3 { pos.x, pos.y, 0 };
    }

    const glm::vec2& getVelocity() const noexcept
    {
        return velocity;
    }

    void setVelocity( const glm::vec2& vel )
    {
        velocity = vel;
    }

private:
    // Collision sphere.
    Math::Sphere sphere;

    // Transform for this pickup.
    Math::Transform2D transform;

    // Shared sprite sheet for the pickup.
    std::shared_ptr<sr::SpriteSheet> spriteSheet;

    // Frame-rate for sprite animation.
    uint32_t frameRate = 20u;

    // Animation time for determining the frame to draw for this pickup.
    float time = 0.0f;

    // Whether to apply gravity for this pickup.
    float gravity = 0.0f;
    // Current velocity of the pickup.
    glm::vec2 velocity { 0 };
};