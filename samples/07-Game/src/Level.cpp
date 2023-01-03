#include <Level.hpp>

#include <BlendMode.hpp>
#include <Color.hpp>

using namespace Math;
using namespace sr;

Level::Level( const ldtk::World& world, const ldtk::Level& level )
: world { &world }
, level { &level }
{
    const auto& entities   = level.getLayer( "Entities" );
    const auto& collisions = entities.getEntitiesByName( "Collision" );
    for ( auto& collision: collisions )
    {
        auto& e = collision.get();
        auto& p = e.getPosition();
        auto& s = e.getSize();

        colliders.emplace_back( glm::vec3 { p.x, p.y, 0.0f }, glm::vec3 { p.x + s.x, p.y + s.y, 0.0f } );
    }

    // Player start position
    const auto& playerStart = entities.getEntitiesByName( "Player_Start" )[0].get();
    Transform2D playerTransform { { playerStart.getPosition().x, playerStart.getPosition().y } };
    // Player sprite is 32x32 pixels.
    // Place the anchor point in the bottom center of the sprite.
    playerTransform.setAnchor( { 16, 32 } );

    player.setTransform( playerTransform );
}

void Level::update( float deltaTime )
{
    // Player.
    player.update( deltaTime );
}

void Level::reset()
{
    player.reset();
}

void Level::draw( sr::Image& image ) const
{
    for ( const auto& collider: colliders )
    {
        image.drawAABB( collider, Color::Red, BlendMode::Disable, FillMode::WireFrame );
    }

    player.draw( image );
}
