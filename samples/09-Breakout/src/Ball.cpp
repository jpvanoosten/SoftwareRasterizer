#include <Ball.hpp>

#include <Graphics/ResourceManager.hpp>

using namespace Graphics;
using namespace Math;

inline Circle operator*( const Camera2D& camera, const Circle& c )
{
    return { camera * c.center, camera.getZoom() * c.radius };
}

Ball::Ball()
: circle { {0, 0}, 32.0f }
, sprite { ResourceManager::loadImage( "assets/Breakout/Sprite Sheet/Breakout_Tile_Free.png" ), { 1403, 652, 64, 64 }, BlendMode::AlphaBlend }
{
    transform.setAnchor( { 32, 32 } );
}

Ball::Ball( const glm::vec2& position )
: transform { position }
, circle { position, 32.0f }
, sprite { ResourceManager::loadImage( "assets/Breakout/Sprite Sheet/Breakout_Tile_Free.png" ), { 1403, 652, 64, 64 }, BlendMode::AlphaBlend }
{
    transform.setAnchor( { 32, 32 } );
}

void Ball::update( float deltaTime )
{
    auto position = transform.getPosition();

    position += velocity * deltaTime;
    circle.center = position;

    transform.setPosition( position );
}

void Ball::draw( Graphics::Image& image, const Math::Camera2D& camera )
{
    image.drawSprite( sprite, camera * transform );

#if _DEBUG
    image.drawCircle( camera * circle, Color::Yellow, {}, FillMode::WireFrame );
#endif
}

void Ball::setPosition( const glm::vec2& pos )
{
    transform.setPosition( pos );
}

const glm::vec2& Ball::getPosition() const
{
    return transform.getPosition();
}

void Ball::setVelocity( const glm::vec2& vel )
{
    velocity = vel;
}

const glm::vec2& Ball::getVelocity() const
{
    return velocity;
}

void Ball::setCircle( const Math::Circle& _circle )
{
    circle   = _circle;
    transform.setPosition( circle.center );
}

const Math::Circle& Ball::getCircle() const
{
    return circle;
}
