#include <Brick.hpp>

Brick::Brick() = default;

Brick::Brick( std::shared_ptr<Graphics::SpriteSheet> spriteSheet, std::span<const int> frames, int points, int hitPoints )
: spriteAnim { std::move( spriteSheet ), FPS, frames }
, points { points }
, hitPoints { hitPoints }
, aabb { glm::vec3 { 0, 0, 0 }, glm::vec3 { 16, 8, 0 } }
{}

void Brick::update( float deltaTime )
{
    if (doAnimation)
    {
        spriteAnim.update( deltaTime );
        if (spriteAnim.isDone())
        {
            doAnimation = false;
        }
    }
}

void Brick::draw( Graphics::Image& image ) const
{
    if (hitPoints > 0)
    {
        const int x = static_cast<int>( transform.getPosition().x );
        const int y = static_cast<int>( transform.getPosition().y );
        image.drawSprite( spriteAnim, x + 8, y + 8, Graphics::Color::Black.withAlpha( 0.3f ) );
        image.drawSprite( spriteAnim, x, y );
    }
}

int Brick::hit()
{
    --hitPoints;

    if (hitPoints > 0)
    {
        // Play the sprite animation.
        spriteAnim.reset();
        doAnimation = true;
    }

    return hitPoints;
}

void Brick::animate()
{
    spriteAnim.reset();
    doAnimation = true;
}

void Brick::setPosition( const glm::vec2& pos ) noexcept
{
    transform.setPosition( pos );
}

const glm::vec2& Brick::getPosition() const noexcept
{
    return transform.getPosition();
}

std::optional<Physics::HitInfo> Brick::checkCollision( const Math::Circle& c, const glm::vec2& v ) const
{
    return Physics::collidesWith( getAABB(), c, v );
}

bool Brick::checkCollision( const Math::AABB& _aabb ) const
{
    return getAABB().intersect( _aabb );
}

Math::AABB Brick::getAABB() const noexcept
{
    return transform * aabb;
}
