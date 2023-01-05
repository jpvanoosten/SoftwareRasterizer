#include <Pickup.hpp>

#include <random>

static std::random_device             rd;
static std::minstd_rand               rng( rd() );
static std::uniform_real_distribution dist { 0.0f, 1.0f };

Pickup::Pickup( const sr::SpriteSheet& sprites, const Math::Sphere& collision )
: sphere { collision }
, transform { collision.center }
, spriteSheet { &sprites }
, time { dist( rng ) }  // Some random offset for time (so pickups are not all in sync)
{
    // Set the anchor point to the center of the sprite.
    transform.setAnchor( glm::vec2 { sprites.getSpriteWidth() / 2, sprites.getSpriteHeight() / 2 } );
}

void Pickup::update( float deltaTime )
{
    time += deltaTime;
}

void Pickup::draw( sr::Image& image ) const
{
    if ( !spriteSheet || spriteSheet->getNumSprites() == 0 )
        return;

    const size_t frame = static_cast<size_t>( time * static_cast<float>( frameRate ) ) % spriteSheet->getNumSprites();
    image.drawSprite( ( *spriteSheet )[frame], transform );
}
