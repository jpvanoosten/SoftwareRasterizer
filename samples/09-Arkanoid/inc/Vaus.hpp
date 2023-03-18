#pragma once

#include "Physics.hpp"

#include <Graphics/Image.hpp>
#include <Graphics/SpriteAnim.hpp>

#include <Math/AABB.hpp>
#include <Math/Camera2D.hpp>

#include <glm/vec2.hpp>

class Ball;

class Vaus
{
public:
    enum class State
    {
        Wait,    // Wait for "ready" state.
        Appear,  // When vaus appears on the screen (after dying or at the beginning of the level).
        Default,
        ToLaser,  // Transition to laser mode.
        Laser,
        Enlarge,
        ExplodeStage1,  // First stage explosion.
        ExplodeStage2,  // Second stage explosion.
        Dead,           // Vaus is dead.
    };

    Vaus();

    Vaus( const std::shared_ptr<Graphics::SpriteSheet>& spriteSheet, const glm::vec2& pos );

    void setState( State newState );

    void update( float deltaTime );

    void draw( Graphics::Image& image, const Math::Camera2D& camera );

    void             setPosition( const glm::vec2& pos );
    const glm::vec2& getPosition() const;

    Math::AABB getAABB() const;

    /// <summary>
    /// Check for collision with the paddle.
    /// </summary>
    /// <param name="ball">The ball to check for collision with.</param>
    /// <returns>An optional HitInfo structure if a collision occurred.</returns>
    std::optional<Physics::HitInfo> collidesWith( const Ball& ball ) const;

private:
    // Framerate for all sprite animations.
    static inline const float FPS = 6.0f;

    void beginState( State newState );
    void endState( State oldState );

    // Update functions for each state/mode.
    void doWait(float deltaTime);
    void doAppear( float deltaTime );
    void doDefault( float deltaTime );
    void doToLaser( float deltaTime );
    void doLaser( float deltaTime );
    void doEnlarge( float deltaTime );
    void doExplosion( float deltaTime );
    void doDead( float deltaTime );

    State state = State::Appear;

    Math::AABB   aabb;
    Math::AABB   enlargeAABB;
    Math::Circle leftCircle;
    Math::Circle rightCircle;

    Math::Transform2D transform;
    Math::Transform2D enlargeTransform;
    Math::Transform2D explosionTransform;

    Graphics::SpriteAnim appearMode;
    Graphics::SpriteAnim defaultMode;
    Graphics::SpriteAnim enlargeMode;
    Graphics::SpriteAnim toLaserMode;
    Graphics::SpriteAnim laserMode;
    Graphics::SpriteAnim explode1;
    Graphics::SpriteAnim explode2;
};
