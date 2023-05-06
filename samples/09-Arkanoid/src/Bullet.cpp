#include <Bullet.hpp>

Bullet::Bullet( std::shared_ptr<Graphics::SpriteSheet> spriteSheet, std::span<const int> frames )
: spriteAnim { std::move( spriteSheet ), FPS, frames }
, aabb { { 0, 0, 0 }, { 16, 8, 0 } }
{
    transform.setAnchor( { 8, 4 } );
}

void Bullet::update( float deltaTime ) noexcept
{
    switch ( state )
    {
    case State::Fired:
        doFired( deltaTime );
        break;
    case State::Hit:
        doHit( deltaTime );
        break;
    }
}

void Bullet::draw( Graphics::Image& image ) const noexcept
{
    if ( state == State::None )
        return;

    image.drawSprite( spriteAnim, transform );
}

void Bullet::fire( const glm::vec2& pos )
{
    setPosition( pos );
    setState( State::Fired );
}

void Bullet::hit() noexcept
{
    spriteAnim.reset();
    setState( State::Hit );
}

void Bullet::setPosition( const glm::vec2& pos ) noexcept
{
    transform.setPosition( pos );
}

const glm::vec2& Bullet::getPosition() const noexcept
{
    return transform.getPosition();
}

Math::AABB Bullet::getAABB() const noexcept
{
    return transform * aabb;
}

void Bullet::setState( State _state ) noexcept
{
    state = _state;
}

Bullet::State Bullet::getState() const noexcept
{
    return state;
}

void Bullet::doFired( float deltaTime )
{
    auto pos = transform.getPosition();
    pos -= SPEED * deltaTime;
    transform.setPosition( pos );
}

void Bullet::doHit( float deltaTime )
{
    spriteAnim.update( deltaTime );
    if (spriteAnim.isDone())
    {
        setState( State::None );
    }
}
