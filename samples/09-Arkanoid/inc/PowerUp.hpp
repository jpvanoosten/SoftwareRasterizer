#pragma once

#include <Graphics/SpriteAnim.hpp>
#include <Graphics/SpriteSheet.hpp>
#include <Math/AABB.hpp>

class PowerUp
{
public:
    enum Type
    {
        None,        ///< No power-up.
        Slow,        ///< Collect the orange capsule to slow the velocity at which the ball moves. Collecting multiple orange capsules will have a cumulative effect and the ball velocity can become extremely slow. However, the ball velocity will gradually increase as it bounces and destroys bricks. The velocity may sometimes suddenly increase with little warning.
        Catch,       ///< Collect the green capsule to gain the catch ability. When the ball hits the Vaus, it will stick to the surface. Press the Fire button to release the ball. The ball will automatically release after a certain period of time has passed.
        Laser,       ///< Collect the red capsule to transform the Vaus into its Laser-firing configuration. In this form, you can fire lasers at the top of the screen by pushing the fire button. Lasers can be used against every brick except Gold bricks, and against enemies. Silver bricks can only be destroyed by lasers when they are hit the required number of times.
        Enlarge,     ///< Collect the blue capsule to extend the width of the Vaus.
        Disruption,  ///< Collect the cyan capsule to cause the ball to split into three instances of itself. All three balls can be kept aloft. There is no penalty for losing the first two balls. No colored capsules will fall as long as there is more than one ball in play. This is the only power up that, while in effect, prevents other power ups from falling.
        Break,       ///< Collect the violet capsule to create a "break out" exit on the right side of the stage. Passing through this exit will cause you to advance to the next stage immediately, as well as earn a 10,000 point bonus.
        Player,      ///< Collect the gray capsule to earn an extra Vaus.
    };

    PowerUp() = default;

    /// <summary>
    /// Create a particular power-up.
    /// </summary>
    /// <param name="spriteSheet">The spritesheet that contains the sprites for the power-ups.</param>
    /// <param name="frames">The frames from the spritesheet for this power-up.</param>
    /// <param name="type">The type of the power-up.</param>
    PowerUp( std::shared_ptr<Graphics::SpriteSheet> spriteSheet, std::span<const int> frames, Type type );

    /// <summary>
    /// Update the power-up's animation.
    /// </summary>
    /// <param name="deltaTime">The elapsed time (in seconds).</param>
    void update( float deltaTime );

    /// <summary>
    /// Draw power-up sprite to the image.
    /// </summary>
    /// <param name="image">The image to draw the brick to.</param>
    void draw( Graphics::Image& image ) const;

    /// <summary>
    /// Set the position of the power-up.
    /// </summary>
    /// <param name="pos"></param>
    void             setPosition( const glm::vec2& pos ) noexcept;
    const glm::vec2& getPosition() const noexcept;

    /// <summary>
    /// Get the AABB of the power-up.
    /// </summary>
    /// <returns></returns>
    Math::AABB getAABB() const noexcept;

    /// <summary>
    /// Check collision with this power-up and an other AABB.
    /// </summary>
    /// <param name="aabb">The AABB to check for collisions with.</param>
    /// <returns>`true` if they are colliding, `false` otherwise.</returns>
    bool checkCollision( const Math::AABB& aabb ) const;

    /// <summary>
    /// Get the type of this power-up.
    /// </summary>
    /// <returns>The type of this power-up.</returns>
    Type getType() const noexcept
    {
        return type;
    }

private:
    static inline float FPS   = 12.0f;   ///< FPS for sprite animation.
    static inline float SPEED = 50.0f;  ///< Speed of falling sprites (pixels/second).

    Graphics::SpriteAnim sprites;
    Math::AABB           aabb;
    Math::Transform2D    transform;
    Type                 type = None;
};
