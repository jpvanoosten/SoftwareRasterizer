#include "Ball.hpp"

#include <Graphics/ResourceManager.hpp>

using namespace Graphics;
using namespace Math;

Ball::Ball( const glm::vec2& pos )
: sprite( ResourceManager::loadImage( "assets/textures/ball.png" ) )
, transform{pos}
, circle { pos, static_cast<float>( sprite.getWidth() ) / 2.0f }
{
    transform.setAnchor( glm::vec2 { sprite.getSize() } * 0.5f );
}

void Ball::update( float dt )
{
    auto pos = transform.getPosition();
    pos += velocity * dt;
    transform.setPosition( pos );
    circle.center = pos;
}

void Ball::draw( Graphics::Image& image ) const
{
    image.drawSprite( sprite, transform );
}

void Ball::setCircle( const Math::Circle& _circle )
{
    circle = _circle;
    transform.setPosition( circle.center );
}

const Math::Circle& Ball::getCircle() const
{
    return circle;
}

void Ball::setPosition( const glm::vec2& position )
{
    transform.setPosition( position );
    circle.center = position;
}

const glm::vec2& Ball::getPosition() const
{
    return transform.getPosition();
}

void Ball::setVelocity( const glm::vec2& _velocity )
{
    velocity = _velocity;
}

const glm::vec2& Ball::getVelocity() const
{
    return velocity;
}
