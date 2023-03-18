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
    Vaus();

    Vaus( const std::shared_ptr<Graphics::SpriteSheet>& spriteSheet, const glm::vec2& pos );

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
    Math::AABB        aabb;
    Math::Circle      leftCircle;
    Math::Circle      rightCircle;

    Math::Transform2D transform;

    Graphics::SpriteAnim defaultSpriteAnim;
    Graphics::SpriteAnim gunsSpriteAnim;
};
