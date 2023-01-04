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
        auto& e      = collision.get();
        auto& p      = e.getPosition();
        auto& s      = e.getSize();
        auto& oneWay = e.getField<ldtk::FieldType::Bool>( "OneWay" );

        Collider collider {
            .type     = ColliderType::Default,
            .aabb     = AABB { { p.x, p.y, 0.0 }, { p.x + s.x, p.y + s.y, 0.0f } },
            .isOneWay = oneWay ? *oneWay : false,
        };

        colliders.push_back( collider );
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
    // Store the previous position so we know if the
    // player moved through a 1-way collider.
    glm::vec2 prevPos = player.getPosition();

    // Update Player.
    player.update( deltaTime );

    // Check player collision
    AABB playerAABB = player.getAABB();

    glm::vec2 vel = player.getVelocity();
    glm::vec2 pos = player.getPosition();

    // Number of pixels padding to account for collisions.
    const float padding = 3.0f;

    bool onGround    = false;
    bool onLeftWall  = false;
    bool onRightWall = false;
    for ( auto& collider: colliders )
    {
        AABB colliderAABB = collider.aabb;

        // Player is moving right.
        if ( vel.x > 0.0f )
        {
            // Check to see if the player is colliding with the left edge of the collider.
            Line leftEdge { { colliderAABB.min.x, colliderAABB.min.y + padding, 0 }, { colliderAABB.min.x, colliderAABB.max.y - padding, 0 } };
            if ( !collider.isOneWay && playerAABB.intersect( leftEdge ) )
            {
                // Set the player's position to the left edge of the collider.
                pos.x = colliderAABB.min.x - playerAABB.width() * 0.5f;
                // And 0 the X velocity.
                vel.x = 0.0f;

                // On a wall that is right of the player.
                onRightWall = true;
            }
        }
        // Player is moving left.
        else if ( vel.x < 0.0f )
        {
            // Check to see if the player is colliding with the right edge of the collider.
            Line rightEdge { { colliderAABB.max.x, colliderAABB.min.y + padding, 0 }, { colliderAABB.max.x, colliderAABB.max.y - padding, 0 } };
            if ( !collider.isOneWay && playerAABB.intersect( rightEdge ) )
            {
                // Set the player's position to the right edge of the collider.
                pos.x = colliderAABB.max.x + playerAABB.width() * 0.5f;
                // And 0 the X velocity.
                vel.x = 0.0f;

                // Player is on a wall that is to the left of the player.
                onLeftWall = true;
            }
        }
        else
        {
            // Check to see if the player is still colliding with the left or right edge of the collider.
            Line leftEdge { { colliderAABB.min.x, colliderAABB.min.y + padding, 0 }, { colliderAABB.min.x, colliderAABB.max.y - padding, 0 } };
            if ( !collider.isOneWay && playerAABB.intersect( leftEdge ) )
            {
                onRightWall = true;
            }

            Line rightEdge { { colliderAABB.max.x, colliderAABB.min.y + padding, 0 }, { colliderAABB.max.x, colliderAABB.max.y - padding, 0 } };
            if ( !collider.isOneWay && playerAABB.intersect( rightEdge ) )
            {
                onLeftWall = true;
            }
        }

        // Player is moving down (falling).
        if ( vel.y < 0.0f )
        {
            // Check to see if the player is colliding with the top edge of the collider.
            Line topEdge { { colliderAABB.min.x + padding, colliderAABB.min.y, 0 }, { colliderAABB.max.x - padding, colliderAABB.min.y, 0 } };
            if ( playerAABB.intersect( topEdge ) )
            {
                // We only collide with 1-way colliders if the player's previous position was above the collider.
                if ( !collider.isOneWay || prevPos.y < colliderAABB.min.y )
                {
                    // Set the player's position to the top of the AABB.
                    pos.y = colliderAABB.min.y;
                    // And 0 the Y velocity.
                    vel.y = 0.0f;

                    // And change state to running (to preserve momentum.)
                    player.setState( Player::State::Run );

                    onGround = true;
                }
            }
        }
        // Player is moving up (jumping)
        else if ( vel.y > 0.0f )
        {
            // Check to see if the player is colliding with the bottom edge of the collider.
            Line bottomEdge { { colliderAABB.min.x + padding, colliderAABB.max.y, 0 }, { colliderAABB.max.x - padding, colliderAABB.max.y, 0 } };
            if ( !collider.isOneWay && playerAABB.intersect( bottomEdge ) )
            {
                // Set the player's position to the bottom of the collider.
                pos.y = colliderAABB.max.y + playerAABB.height();
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
            Line topEdge { { colliderAABB.min.x + padding, colliderAABB.min.y, 0 }, { colliderAABB.max.x - padding, colliderAABB.min.y, 0 } };
            if ( playerAABB.intersect( topEdge ) )
            {
                onGround = true;
            }
        }
    }

    Player::State playerState = player.getState();

    // If the player was falling and is touching a wall, then start wall jump
    if ( playerState == Player::State::Falling )
    {
        if ( onLeftWall )
            player.setState( Player::State::LeftWallJump );
        else if ( onRightWall )
            player.setState( Player::State::RightWallJump );
    }

    // If the player was wall jumping but is no longer touching a wall, then start falling.
    if ( playerState == Player::State::LeftWallJump || playerState == Player::State::RightWallJump )
    {
        if ( !onLeftWall && !onRightWall )
            player.setState( Player::State::Falling );
    }

    // If the player was moving but is no longer colliding, then start falling.
    if ( playerState == Player::State::Run && !onGround )
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
        image.drawAABB( collider.aabb, collider.isOneWay ? Color::Yellow : Color::Red, BlendMode::Disable, FillMode::WireFrame );
    }

    player.draw( image );
}
