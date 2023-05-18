#include <Graphics/Color.hpp>
#include <PowerUp.hpp>

PowerUp::PowerUp( std::shared_ptr<Graphics::SpriteSheet> spriteSheet, std::span<const int> frames, Type type )
: sprites { std::move( spriteSheet ), FPS, frames }
, aabb { { 0, 0, 0 }, { 16, 8, 0 } }
, type { type }
{}

void PowerUp::update( float deltaTime )
{
    if ( type == None )
        return;

    sprites.update( deltaTime );

    glm::vec2 pos = transform.getPosition();
    pos += glm::vec2 { 0, SPEED } * deltaTime;
    transform.setPosition( pos );
}

void PowerUp::draw( Graphics::Image& image ) const
{
    if ( type == None )
        return;

    const int x = static_cast<int>( transform.getPosition().x );
    const int y = static_cast<int>( transform.getPosition().y );

    image.drawSprite( sprites, x + 2, y + 2, Graphics::Color::Black );
    image.drawSprite( sprites, x, y );

#if _DEBUG
    image.drawAABB( getAABB(), Graphics::Color::Yellow, {}, Graphics::FillMode::WireFrame );
#endif
}

void PowerUp::setPosition( const glm::vec2& pos ) noexcept
{
    transform.setPosition( pos );
}

const glm::vec2& PowerUp::getPosition() const noexcept
{
    return transform.getPosition();
}

Math::AABB PowerUp::getAABB() const noexcept
{
    return transform * aabb;
}

bool PowerUp::checkCollision( const Math::AABB& _aabb ) const
{
    return getAABB().intersect( _aabb );
}
