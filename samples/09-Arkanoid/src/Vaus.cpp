#include "Ball.hpp"

#include <Vaus.hpp>

using namespace Graphics;
using namespace Math;

inline Circle operator*( const Camera2D& camera, const Circle& c )
{
    return { camera * c.center, camera.getZoom() * c.radius };
}

AABB operator*( const Camera2D& camera, const AABB& aabb )
{
    return { camera * aabb.min, camera * aabb.max };
}

Vaus::Vaus() {}

Vaus::Vaus( const std::shared_ptr<Graphics::SpriteSheet>& spriteSheet, const glm::vec2& pos )
: aabb { { 30, 0, 0 }, { 210, 64, 0 } }
, leftCircle { { 0, 0 }, 32 }
, rightCircle( { 0, 0 }, 32 )
, transform { pos }
{
    // The animation frames in the sprite sheet for the various states of the paddle.
    constexpr int defaultAnimFrames[] { 49, 50, 51 };
    constexpr int gunsAnimFrames[] { 52, 53, 54 };

    defaultSpriteAnim = SpriteAnim { spriteSheet, 15, defaultAnimFrames };
    gunsSpriteAnim    = SpriteAnim { spriteSheet, 15, gunsAnimFrames };

    // Set the anchor point to the center of the paddle.
    transform.setAnchor( { 121.5, 32 } );
}

void Vaus::update( float deltaTime )
{
    defaultSpriteAnim.update( deltaTime );
    gunsSpriteAnim.update( deltaTime );
}

void Vaus::draw( Graphics::Image& image, const Math::Camera2D& camera )
{
    image.drawSprite( defaultSpriteAnim, camera * transform );

#if _DEBUG
    image.drawAABB( camera * getAABB(), Color::Yellow, {}, FillMode::WireFrame );
    image.drawCircle( camera * leftCircle, Color::Yellow, {}, FillMode::WireFrame );
    image.drawCircle( camera * rightCircle, Color::Yellow, {}, FillMode::WireFrame );
#endif
}

void Vaus::setPosition( const glm::vec2& pos )
{
    transform.setPosition( pos );

    constexpr glm::vec2 paddleWidth { 90, 0 };

    leftCircle.center  = pos - paddleWidth;
    rightCircle.center = pos + paddleWidth;
}

const glm::vec2& Vaus::getPosition() const
{
    return transform.getPosition();
}

Math::AABB Vaus::getAABB() const
{
    return transform * aabb;
}

std::optional<Physics::HitInfo> Vaus::collidesWith( const Ball& ball ) const
{
    const auto& c = ball.getCircle();

    if ( const auto hit = Physics::collidesWith( getAABB(), ball.getCircle(), ball.getVelocity() ) )
    {
        return hit;
    }
    if ( const auto hit = Physics::collidesWith( leftCircle, c ) )
    {
        return hit;
    }
    if ( const auto hit = Physics::collidesWith( rightCircle, c ) )
    {
        return hit;
    }

    return {};
}
