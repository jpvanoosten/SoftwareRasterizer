#pragma once

#include <Graphics/Image.hpp>
#include <Graphics/SpriteAnim.hpp>
#include <Graphics/SpriteSheet.hpp>

#include <Math/AABB.hpp>
#include <Math/Transform2D.hpp>

#include <span>

class Bullet
{
public:
    enum class State
    {
        None,   // Bullet should not be drawn updated, or perform collisions.
        Fired,  // Bullet is fired.
        Hit,    // Bullet hit something.
    };

    Bullet() = default;

    Bullet( std::shared_ptr<Graphics::SpriteSheet> spriteSheet, std::span<const int> frames );

    void update( float deltaTime ) noexcept;
    void draw( Graphics::Image& image ) const noexcept;

    /// <summary>
    /// Fire the bullet from a particular position.
    /// </summary>
    /// <param name="pos">The position to fire the bullet from.</param>
    void fire( const glm::vec2& pos );

    /// <summary>
    /// Invoke when the Bullet hits something (either a brick or an enemy)
    /// </summary>
    void hit() noexcept;

    void             setPosition( const glm::vec2& pos ) noexcept;
    const glm::vec2& getPosition() const noexcept;

    Math::AABB getAABB() const noexcept;

    void  setState( State state ) noexcept;
    State getState() const noexcept;

private:
    static inline float     FPS   = 12.0f;
    static inline glm::vec2 SPEED = { 0.0f, 300.0f };  // How fast the bullet moves up the screen.

    void doFired( float deltaTime );
    void doHit( float deltaTime );

    Graphics::SpriteAnim spriteAnim;
    Math::AABB           aabb;
    Math::Transform2D    transform;
    State                state = State::None;
};