#pragma once

#include "Character.hpp"
#include "Curve.hpp"

#include <Image.hpp>
#include <Math/Transform2D.hpp>

#include <vector>

class Player
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
        WallJump,
        Dead,
    };

    explicit Player( const Math::Transform2D& transform = Math::Transform2D {} );

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
    /// Get the current player's state.
    /// </summary>
    /// <returns></returns>
    State getState() const noexcept
    {
        return state;
    }

    void                     setTransform( const Math::Transform2D& transform );
    const Math::Transform2D& getTransform() const noexcept;

    /// <summary>
    /// Draw the player to the image.
    /// </summary>
    /// <param name="image">The image to draw the player to.</param>
    void draw( sr::Image& image ) noexcept;

private:

    using CharacterList = std::vector<Character>;

    void setState( State newState );
    void startState( State newState );
    void endState( State oldState );

    float doHorizontalMovement( float deltaTime );
    void doIdle( float deltaTime );
    void doRun( float deltaTime );
    void doJump( float deltaTime );
    void doHit( float deltaTime );
    void doDoubleJump(float deltaTime );
    void doFalling( float deltaTime );
    void doWallJump( float deltaTime );

    // A list of possible character models.
    CharacterList           characters;
    CharacterList::iterator currentCharacter = characters.end();

    // The player's transform.
    Math::Transform2D transform;

    // Player's current state.
    State state = State::Idle;

    // Player acceleration curve.
    Curve<float> accelCurve;

    // The maximum speed of the player.
    const float playerSpeed = 150.0f;
    const float jumpVelocity = 500.0f;
    const float gravity      = 2000.0f;

    // True if the player can double jump.
    bool canDoubleJump = true;

    // Player's current velocity.
    glm::vec2 velocity { 0 };
};