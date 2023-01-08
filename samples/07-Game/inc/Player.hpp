#pragma once

#include "Character.hpp"

#include <Image.hpp>
#include <Math/Transform2D.hpp>

#include <vector>

class Player final
{
public:
    enum class State
    {
        Idle,
        Run,
        Jump,
        Hit,
        DoubleJump,
        Falling,
        LeftWallJump,
        RightWallJump,
        Dead,
    };

    explicit Player( const Math::Transform2D& transform = Math::Transform2D {} );

    Player( const Player& copy );
    Player( Player&& other ) noexcept;
    ~Player() = default;

    Player& operator=( const Player& copy );
    Player& operator=( Player&& other ) noexcept;

    /// <summary>
    /// Reset the character (and choose a different character).
    /// </summary>
    void reset();

    /// <summary>
    /// Update the player.
    /// </summary>
    /// <param name="deltaTime">The elapsed time (in seconds).</param>
    void update( float deltaTime ) noexcept;

    /// <summary>
    /// Draw the player to the image.
    /// </summary>
    /// <param name="image">The image to draw the player to.</param>
    void draw( sr::Image& image ) const noexcept;

    /// <summary>
    /// Set the state of the player.
    /// </summary>
    /// <param name="newState">The new state to set the player to.</param>
    void setState( State newState );

    /// <summary>
    /// Get the current player's state.
    /// </summary>
    /// <returns></returns>
    State getState() const noexcept
    {
        return state;
    }

    void setPosition( const glm::vec2& pos ) noexcept
    {
        transform.setPosition( pos );
    }

    const glm::vec2& getPosition() const noexcept
    {
        return transform.getPosition();
    }

    void setVelocity( const glm::vec2& vel ) noexcept
    {
        velocity = vel;
    }

    const glm::vec2& getVelocity() const noexcept
    {
        return velocity;
    }

    void setTransform( const Math::Transform2D& _transform )
    {
        transform = _transform;
    }

    const Math::Transform2D& getTransform() const noexcept
    {
        return transform;
    }

    Math::AABB getAABB() const noexcept
    {
        return aabb * transform;
    }

    Math::AABB getTopAABB() const noexcept
    {
        return topAABB * transform;
    }

    Math::AABB getBottomAABB() const noexcept
    {
        return bottomAABB * transform;
    }

    Math::AABB getLeftAABB() const noexcept
    {
        return leftAABB * transform;
    }

    Math::AABB getRightAABB() const noexcept
    {
        return rightAABB * transform;
    }

    void setCharacter( size_t characterId );

    size_t getNumCharacters() const noexcept
    {
        return characters.size();
    }

private:
    void startState( State newState );
    void endState( State oldState );

    float doHorizontalMovement( float deltaTime );
    void  doIdle( float deltaTime );
    void  doRun( float deltaTime );
    void  doJump( float deltaTime );
    void  doHit( float deltaTime );
    void  doDoubleJump( float deltaTime );
    void  doFalling( float deltaTime );
    void  doWallJump( float deltaTime );

    // A list of possible character models.
    using CharacterList = std::vector<std::shared_ptr<Character>>;

    CharacterList              characters;
    std::shared_ptr<Character> currentCharacter;

    // The player's transform.
    Math::Transform2D transform;

    // The player's AABB.
    Math::AABB aabb;
    Math::AABB topAABB;
    Math::AABB bottomAABB;
    Math::AABB leftAABB;
    Math::AABB rightAABB;

    // Player's current state.
    State state = State::Idle;

    // How much dampening to apply.
    float xDampen = 50.0f;

    // How fast the player accelerates.
    const float accel = 5e3f;
    // The maximum speed of the player.
    const float maxSpeed = 150.0f;
    // Maximum jump height in pixels.
    const float jumpHeight = 60.0f;
    // Jump time is the time (in seconds) to the apex of the jump.
    const float jumpTime = 0.25f;
    // Gravity and jump speed are computed to achieve the desired jump parabola.
    // Source: https://2dengine.com/?p=platformers#Jumping
    // Source: https://jobtalle.com/2d_platformer_physics.html
    const float gravity   = 2.0f * jumpHeight / ( jumpTime * jumpTime );
    const float jumpSpeed = std::sqrt( 2.0f * jumpHeight * gravity );
    // How fast the player jumps off the wall.
    const float wallJumpSpeed = 500.0f;

    // True if the player can double jump.
    bool canDoubleJump = true;

    // Player's current velocity.
    glm::vec2 velocity { 0 };
};