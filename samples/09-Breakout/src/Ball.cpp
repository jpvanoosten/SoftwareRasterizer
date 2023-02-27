#include <Ball.hpp>

#include <Graphics/ResourceManager.hpp>

using namespace sr;
using namespace Math;

Ball::Ball()
: sprite { ResourceManager::loadImage( "assets/Breakout/Sprite Sheet/Breakout_Tile_Free.png" ), { 1403, 652, 64, 64 }, BlendMode::AlphaBlend }
, circle { position, 32.0f }
{}

Ball::Ball( const glm::vec2& position )
: position { position }
, sprite { ResourceManager::loadImage( "assets/Breakout/Sprite Sheet/Breakout_Tile_Free.png" ), { 1403, 652, 64, 64 }, BlendMode::AlphaBlend }
, circle { position, 32.0f }
{}

void Ball::update( float deltaTime )
{
    position += velocity * deltaTime;
    circle.center = position;
}

void Ball::draw( sr::Image& image )
{
    image.drawSprite( sprite, static_cast<int>( position.x - circle.radius ), static_cast<int>( position.y - circle.radius ) );

#if _DEBUG
    image.drawCircle( circle, Color::Yellow, {}, FillMode::WireFrame );
#endif
}

void Ball::setPosition( const glm::vec2& pos )
{
    position = pos;
}

const glm::vec2& Ball::getPosition() const
{
    return position;
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
    position = circle.center;
}

const Math::Circle& Ball::getCircle() const
{
    return circle;
}
