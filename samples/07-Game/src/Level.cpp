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
    AABB previousAABB = player.getAABB();

    // Update Player.
    player.update( deltaTime );

    // Check player collision
    AABB      currentAABB = player.getAABB();
    glm::vec2 p0 { currentAABB.min.x, currentAABB.max.y };
    glm::vec2 p1 { currentAABB.max.x, currentAABB.max.y };

    // Check if the bottom of the player's AABB is intersecting with any of the level AABB's
    bool isColliding = false;
    for ( auto& collider: colliders )
    {
        if ( collider.intersect( { p0, 0 }, { p1, 0 } ) )
        {
            // Set the player's position to the top of the AABB.
            auto& pos = player.getPosition();
            player.setPosition( { pos.x, collider.min.y } );

            // And 0 the Y velocity.
            auto& vel = player.getVelocity();
            player.setVelocity( { vel.x, 0 } );

            if ( player.getState() == Player::State::Falling )
                player.setState( Player::State::Run );

            isColliding = true;
            break;
        }
    }

    // If the player was moving but is no longer colliding, then start falling.
    if ( player.getState() == Player::State::Run && !isColliding )
        player.setState( Player::State::Falling );
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
