#pragma once

#include "Physics.hpp"

#include <Graphics/SpriteAnim.hpp>
#include <Graphics/SpriteSheet.hpp>

#include <Math/AABB.hpp>
#include <Math/Transform2D.hpp>

#include <memory>

class Brick
{
public:
    Brick();

    /// <summary>
    /// Create a brick from a series of frames that should be played when the brick is hit.
    /// </summary>
    /// <param name="spriteSheet">The sprite sheet containing the brick sprites.</param>
    /// <param name="frames">The frames in the sprite sheet for this brick.</param>
    /// <param name="points">The number of points you get for breaking the brick.</param>
    /// <param name="hitPoints">The number of hits needed to break this brick.</param>
    Brick( std::shared_ptr<Graphics::SpriteSheet> spriteSheet, std::span<const int> frames, int points = 50, int hitPoints = 1 );

    /// <summary>
    /// Update the brick's animation.
    /// </summary>
    /// <param name="deltaTime">The elapsed time (in seconds).</param>
    void update( float deltaTime );

    /// <summary>
    /// Draw brick sprite to the image.
    /// </summary>
    /// <param name="image">The image to draw the brick to.</param>
    void draw( Graphics::Image& image ) const;

    /// <summary>
    /// Hitting the brick will decrement the hit count of the brick.
    /// </summary>
    /// <returns>The new hit count (after being hit). If this returns 0, the brick should be destroyed.</returns>
    int hit();

    /// <summary>
    /// Use this method to animate the bricks (for example, bricks are animated when Vaus appears).
    /// </summary>
    void animate();

    void             setPosition( const glm::vec2& pos ) noexcept;
    const glm::vec2& getPosition() const noexcept;

    void setHitPoints( int _hitPoints ) noexcept
    {
        hitPoints = _hitPoints;
    }

    int getHitPoints() const noexcept
    {
        return hitPoints;
    }

    void setPoints( int _points ) noexcept
    {
        points = _points;
    }

    int getPoints() const noexcept
    {
        return points;
    }

    /// <summary>
    /// Check for collision with this brick and a moving circle.
    /// Note: This method does not decrement the brick's hit count. Use the Brick::hit method for that.
    /// </summary>
    /// <param name="c">The circle to check for collision with.</param>
    /// <param name="v">The velocity of the circle.</param>
    /// <returns>An optional HitInfo structure that contains the point of collision and the collision normal.</returns>
    std::optional<Physics::HitInfo> checkCollision( const Math::Circle& c, const glm::vec2& v ) const;

    /// <summary>
    /// Check collision with this brick and an other AABB.
    /// </summary>
    /// <param name="aabb">The AABB to check for collisions with.</param>
    /// <returns>`true` if they are colliding, `false` otherwise.</returns>
    bool checkCollision( const Math::AABB& aabb ) const;

    /// <summary>
    /// Get the brick's AABB.
    /// </summary>
    /// <returns>The AABB of the brick.</returns>
    Math::AABB getAABB() const noexcept;

private:
    static inline float  FPS = 12.0f;

    Graphics::SpriteAnim spriteAnim;

    bool doAnimation = false;
    int points    = 50;
    int hitPoints = 1;

    Math::AABB aabb;
    Math::Transform2D transform;
};

