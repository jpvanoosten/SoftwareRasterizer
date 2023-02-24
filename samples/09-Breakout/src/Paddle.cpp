#include <Paddle.hpp>

using namespace sr;
using namespace Math;

Paddle::Paddle() {}

Paddle::Paddle( const std::shared_ptr<sr::SpriteSheet>& spriteSheet, const glm::vec2& pos )
: aabb { { 30, 0, 0 }, { 210, 64, 0 } }
, leftCircle{ {0, 0}, 32 }
, rightCircle( {0, 0}, 32 )
, transform { pos }
{
    // The animation frames in the sprite sheet for the various states of the paddle.
    constexpr int defaultAnimFrames[] { 49, 50, 51 };
    constexpr int gunsAnimFrames[] { 52, 53, 54 };

    defaultSpriteAnim = SpriteAnim { spriteSheet, 15, defaultAnimFrames };
    gunsSpriteAnim    = SpriteAnim { spriteSheet, 15, gunsAnimFrames };

    transform.setAnchor( { 121.5, 32 } );
}

void Paddle::update( float deltaTime )
{
    defaultSpriteAnim.update( deltaTime );
    gunsSpriteAnim.update( deltaTime );
}

void Paddle::draw( sr::Image& image )
{
    image.drawSprite( defaultSpriteAnim, transform );

#if _DEBUG
    image.drawAABB( getAABB(), Color::Yellow, {}, FillMode::WireFrame );
    image.drawCircle( leftCircle, Color::Yellow, {}, FillMode::WireFrame );
    image.drawCircle( rightCircle, Color::Yellow, {}, FillMode::WireFrame );
#endif
}

void Paddle::setPosition( const glm::vec2& pos )
{
    transform.setPosition( pos );

    constexpr glm::vec2 paddleWidth { 90, 0 };

    leftCircle.center  = pos - paddleWidth;
    rightCircle.center = pos + paddleWidth;
}

const glm::vec2& Paddle::getPosition() const
{
    return transform.getPosition();
}

Math::AABB Paddle::getAABB() const
{
    return aabb * transform;
}

bool Paddle::collidesWith( const Math::Circle& c ) const
{
    return getAABB().intersect( Sphere { glm::vec3 { c.center, 0 }, c.radius } ) ||
           leftCircle.intersect( c ) || rightCircle.intersect( c );
}
