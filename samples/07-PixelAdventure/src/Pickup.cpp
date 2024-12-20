#include <Pickup.hpp>

#include <random>

static std::random_device             rd;
static std::minstd_rand               rng( rd() );
static std::uniform_real_distribution dist { 0.0f, 1.0f };

Pickup::Pickup( std::shared_ptr<Graphics::SpriteSheet> sprites, const Math::Sphere& collision )
: sphere { collision }
, transform { collision.center }
, spriteSheet { std::move( sprites ) }
, time { dist( rng ) }  // Some random offset for time (so pickups are not all in sync)
{
    // Set the anchor point to the center of the sprite.
    transform.setAnchor( glm::vec2 { spriteSheet->getSpriteWidth() / 2, spriteSheet->getSpriteHeight() / 2 } );
}

void Pickup::update( float deltaTime )
{
    time += deltaTime;

    if ( abs( gravity ) > 0.0f )
    {
        glm::vec2 vel = getVelocity();
        glm::vec2 pos = getPosition();

        vel.y -= gravity * deltaTime;
        pos += vel * deltaTime;

        setVelocity( vel );
        setPosition( pos );
    }
}

void Pickup::draw( Graphics::Image& image ) const
{
    if ( !spriteSheet || spriteSheet->getNumSprites() == 0 )
        return;

    const size_t frame = static_cast<size_t>( time * static_cast<float>( frameRate ) ) % spriteSheet->getNumSprites();
    image.drawSprite( ( *spriteSheet )[frame], transform );

#if _DEBUG
    image.drawCircle( sphere, Graphics::Color::Yellow, {}, Graphics::FillMode::WireFrame );
#endif
}
