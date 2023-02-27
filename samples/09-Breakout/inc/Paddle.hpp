#pragma once

#include <Graphics/Image.hpp>
#include <Graphics/SpriteAnim.hpp>

#include <Math/AABB.hpp>

#include <glm/vec2.hpp>

class Paddle
{
public:
    Paddle();

    Paddle( const std::shared_ptr<Graphics::SpriteSheet>& spriteSheet, const glm::vec2& pos );

    void update( float deltaTime );

    void draw( Graphics::Image& image );

    void             setPosition( const glm::vec2& pos );
    const glm::vec2& getPosition() const;

    Math::AABB getAABB() const;

    /// <summary>
    /// Check for collision with the paddle.
    /// </summary>
    /// <param name="c">The circle to check for collision with.</param>
    bool collidesWith( const Math::Circle& c ) const;

private:
    Math::AABB        aabb;
    Math::Circle      leftCircle;
    Math::Circle      rightCircle;

    Math::Transform2D transform;

    Graphics::SpriteAnim defaultSpriteAnim;
    Graphics::SpriteAnim gunsSpriteAnim;
};
