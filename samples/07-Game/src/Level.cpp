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
    const auto& startPos = entities.getEntitiesByName( "Player_Start" )[0].get();
    playerStart          = { startPos.getPosition().x, startPos.getPosition().y };

    Transform2D playerTransform { { playerStart.x, playerStart.y } };
    // Player sprite is 32x32 pixels.
    // Place the anchor point in the bottom center of the sprite.
    playerTransform.setAnchor( { 16, 32 } );

    player.setTransform( playerTransform );
}

void Level::update( float deltaTime )
{
    // Update Player.
    player.update( deltaTime );

    // Check player collision
    AABB aabb = player.getAABB();

    glm::vec2 vel = player.getVelocity();
    glm::vec2 pos = player.getPosition();

    // Number of pixels padding to account for collisions.
    const float padding = 3.0f;

    bool onGround = false;
    bool onWall   = false;
    for ( auto& collider: colliders )
    {
        // Player is moving right.
        if ( vel.x > 0.0f )
        {
            // Check to see if the player is colliding with the left edge of the collider.
            Line leftEdge { { collider.min.x, collider.min.y + padding, 0 }, { collider.min.x, collider.max.y - padding, 0 } };
            if ( aabb.intersect( leftEdge ) )
            {
                // Set the player's position to the left edge of the collider.
                pos.x = collider.min.x - aabb.width() * 0.5f;
                // And 0 the X velocity.
                vel.x = 0.0f;

                onWall = true;
            }
        }
        // Player is moving left.
        else if ( vel.x < 0.0f )
        {
            // Check to see if the player is colliding with the right edge of the collider.
            Line rightEdge { { collider.max.x, collider.min.y + padding, 0 }, { collider.max.x, collider.max.y - padding, 0 } };
            if ( aabb.intersect( rightEdge ) )
            {
                // Set the player's position to the right edge of the collider.
                pos.x = collider.max.x + aabb.width() * 0.5f;
                // And 0 the X velocity.
                vel.x = 0.0f;

                onWall = true;
            }
        }
        else
        {
            // Check to see if the player is still colliding with the left or right edge of the collider.
            Line leftEdge { { collider.min.x, collider.min.y + padding, 0 }, { collider.min.x, collider.max.y - padding, 0 } };
            Line rightEdge { { collider.max.x, collider.min.y + padding, 0 }, { collider.max.x, collider.max.y - padding, 0 } };

            if ( aabb.intersect( leftEdge ) || aabb.intersect( rightEdge ))
            {
                onWall = true;
            }
        }

        // Player is moving down (falling).
        if ( vel.y < 0.0f )
        {
            // Check to see if the player is colliding with the top edge of the collider.
            Line topEdge { { collider.min.x + padding, collider.min.y, 0 }, { collider.max.x - padding, collider.min.y, 0 } };
            if ( aabb.intersect( topEdge ) )
            {
                // Set the player's position to the top of the AABB.
                pos.y = collider.min.y;
                // And 0 the Y velocity.
                vel.y = 0.0f;

                // And change state to running (to preserve momentum.)
                player.setState( Player::State::Run );

                onGround = true;
            }
        }
        // Player is moving up (jumping)
        else if ( vel.y > 0.0f )
        {
            // Check to see if the player is colliding with the bottom edge of the collider.
            Line bottomEdge { { collider.min.x + padding, collider.max.y, 0 }, { collider.max.x - padding, collider.max.y, 0 } };
            if ( aabb.intersect( bottomEdge ) )
            {
                // Set the player's position to the bottom of the collider.
                pos.y = collider.max.y + aabb.height();
                // And 0 the Y velocity.
                vel.y = 0.0f;

                // And start falling
                player.setState( Player::State::Falling );
            }
        }
        // Player is idle or running.
        else
        {
            // Check to see if the player is colliding with the top edge of the collider.
            Line topEdge { { collider.min.x + padding, collider.min.y, 0 }, { collider.max.x - padding, collider.min.y, 0 } };
            if ( aabb.intersect( topEdge ) )
            {
                onGround = true;
            }
        }
    }

    // If the player was falling and is touching a wall, then start wall jump
    if ( player.getState() == Player::State::Falling && onWall )
        player.setState( Player::State::WallJump );

    // If the player was wall jumping but is no longer touching a wall, then start falling.
    if ( player.getState() == Player::State::WallJump && !onWall )
        player.setState( Player::State::Falling );

    // If the player was moving but is no longer colliding, then start falling.
    if ( player.getState() == Player::State::Run && !onGround )
        player.setState( Player::State::Falling );

    // Update player position and velocity.
    player.setPosition( pos );
    player.setVelocity( vel );
}

void Level::reset()
{
    player.setPosition( playerStart );
    player.setVelocity( { 0, 0 } );
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
