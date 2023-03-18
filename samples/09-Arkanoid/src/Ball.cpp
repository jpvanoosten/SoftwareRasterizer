#include <Ball.hpp>

#include <Graphics/ResourceManager.hpp>

using namespace Graphics;
using namespace Math;

inline Circle operator*( const Camera2D& camera, const Circle& c )
{
    return { camera * c.center, camera.getZoom() * c.radius };
}

Ball::Ball()
: circle { {0, 0}, 2.5f }
, sprite { ResourceManager::loadImage( "assets/Arkanoid/vaus.png" ), { 0, 40, 16, 8 }, BlendMode::AlphaBlend }
{
    transform.setAnchor( { 2.5f, 2.5f } );
}

Ball::Ball( const glm::vec2& position )
: transform { position }
, circle { position, 2.5f }
, sprite { ResourceManager::loadImage( "assets/Arkanoid/vaus.png" ), { 0, 40, 16, 8 }, BlendMode::AlphaBlend }
{
    transform.setAnchor( { 2.5f, 2.5f } );
}

void Ball::update( float deltaTime )
{
    auto position = transform.getPosition();

    position += velocity * deltaTime;
    circle.center = position;

    transform.setPosition( position );
}

void Ball::draw( Graphics::Image& image )
{
    const int x =  static_cast<int>( std::round( transform.getPosition().x - circle.radius ) );
    const int y = static_cast<int>( std::round( transform.getPosition().y - circle.radius ) );

    image.drawSprite( sprite, x, y );

#if _DEBUG
    //image.drawCircle( circle, Color::Yellow, {}, FillMode::WireFrame );
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
