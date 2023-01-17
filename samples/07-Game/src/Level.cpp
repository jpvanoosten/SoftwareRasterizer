#include "LDtkLoader/Project.hpp"
#include "Player.hpp"

#include <Level.hpp>

#include <BlendMode.hpp>
#include <Color.hpp>
#include <ResourceManager.hpp>

#include <numbers>
#include <random>

static std::random_device rd;
static std::minstd_rand   rng( rd() );

using namespace Math;
using namespace sr;

Box loadBox( const std::filesystem::path& basePath, int hitPoints )
{
    Box box { hitPoints };

    // First, load the sprite sheets for the box animations.
    const auto idle        = ResourceManager::loadSpriteSheet( basePath / "Idle.png", 28, 24, 0, 0, BlendMode::AlphaBlend );
    const auto hit         = ResourceManager::loadSpriteSheet( basePath / "Hit (28x24).png", 28, 24, 0, 0, BlendMode::AlphaBlend );
    const auto breakSprite = ResourceManager::loadSpriteSheet( basePath / "Break.png", 28, 24, 0, 0, BlendMode::AlphaBlend );

    // Now add the sprite animations to the box.
    box.addAnimation( "Idle", SpriteAnim { idle, 20 } );
    box.addAnimation( "Hit", SpriteAnim { hit, 20 } );
    box.addAnimation( "Break", SpriteAnim { breakSprite, 20 } );

    return box;
}

Level::Level( const ldtk::Project& project, const ldtk::World& world, const ldtk::Level& level )
: world { &world }
, level { &level }
, levelName { level.name }
{
    const std::filesystem::path projectPath = project.getFilePath().directory();

    // Load the fruit collected animation.
    {
        auto spriteSheet = ResourceManager::loadSpriteSheet( projectPath / "Items/Fruits/Collected.png", 32, 32, 0, 0, BlendMode::AlphaBlend );
        pickupCollected  = SpriteAnim { spriteSheet, 20 };
    }

    // Load the fruit sprites.
    const auto& tilesets = project.allTilesets();
    for ( auto& tileset: tilesets )
    {
        if ( tileset.hasTag( "Fruit" ) )
        {
            auto sprites               = ResourceManager::loadSpriteSheet( projectPath / tileset.path, tileset.tile_size, tileset.tile_size, static_cast<uint32_t>( tileset.padding ), static_cast<uint32_t>( tileset.spacing ), BlendMode::AlphaBlend );
            fruitSprites[tileset.name] = std::move( sprites );
        }
    }

    // Load the box prefabs/prototypes.
    boxPrefabs["Box1"] = loadBox( projectPath / "Items/Boxes/Box1", 1 );
    boxPrefabs["Box2"] = loadBox( projectPath / "Items/Boxes/Box2", 5 );
    boxPrefabs["Box3"] = loadBox( projectPath / "Items/Boxes/Box3", 5 );

    // Parse collisions.
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
            .aabb     = AABB { { p.x, p.y, 0.0 }, { p.x + s.x - 1, p.y + s.y - 1, 0.0f } },
            .isOneWay = oneWay ? *oneWay : false,
        };

        colliders.push_back( collider );
    }

    // Parse pickups
    const auto& pickups = entities.getEntitiesByName( "Pickup" );
    for ( auto& pickup: pickups )
    {
        auto& e    = pickup.get();
        auto& p    = e.getPosition();
        auto& type = e.getField<ldtk::FieldType::Enum>( "PickupType" );

        auto&  fruitSprite = fruitSprites[type->name];
        Sphere collider { { p.x, p.y, 0 }, 8.0f };

        allPickups.emplace_back( fruitSprite, collider );
    }

    // Parse boxes
    const auto& boxEntities = entities.getEntitiesByName( "Box" );
    for ( auto& box: boxEntities )
    {
        auto& e    = box.get();
        auto& p    = e.getPosition();
        auto& type = e.getField<ldtk::FieldType::Enum>( "BoxType" );

        Box newBox = boxPrefabs[type->name];
        newBox.setPosition( { p.x, p.y } );

        auto b = boxes.emplace_back( std::make_shared<Box>( std::move( newBox ) ) );
        b->setAnimation( "Idle" );
    }

    const auto& tilesLayer = level.getLayer( "Tiles" );
    const auto& intGrid    = level.getLayer( "IntGrid" );  // TODO: Should probably rename this layer..
    const auto& gridSize   = tilesLayer.getGridSize();
    const auto& tileSet    = tilesLayer.getTileset();

    {
        auto spriteSheet = ResourceManager::loadSpriteSheet( projectPath / tileSet.path, tileSet.tile_size, tileSet.tile_size, tileSet.padding, tileSet.spacing, BlendMode::AlphaBlend );
        tileMap          = TileMap( spriteSheet, gridSize.x, gridSize.y );
    }

    for ( auto& tile: intGrid.allTiles() )
    {
        const auto& gridPos             = tile.getGridPosition();
        tileMap( gridPos.y, gridPos.x ) = tile.tileId;
    }

    for ( auto& tile: tilesLayer.allTiles() )
    {
        const auto& gridPos             = tile.getGridPosition();
        tileMap( gridPos.y, gridPos.x ) = tile.tileId;
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
    updateCollisions( deltaTime );
    updatePickups( deltaTime );
    updateEffects( deltaTime );
    updateBoxes( deltaTime );
}

void Level::updateCollisions( float deltaTime )
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

                    continue;
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

                continue;
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

            continue;
        }
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

            continue;
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

void Level::checkPickupCollision( const Sphere& pickupCollider, const AABB& colliderAABB, glm::vec2& pos, glm::vec2& vel )
{
    // Check to see if the pickup is colliding with the top edge of the collider.
    Line topEdge { { colliderAABB.min.x, colliderAABB.min.y, 0 }, { colliderAABB.max.x, colliderAABB.min.y, 0 } };
    if ( pickupCollider.intersect( topEdge ) )
    {
        // Set the position of the pickup to the top edge of the collider.
        pos.y = colliderAABB.min.y - pickupCollider.radius;
        // And negate the velocity.
        vel.y = -vel.y;

        return;
    }

    // Check to see if the pickup is colliding with the bottom edge of the collider.
    Line bottomEdge { { colliderAABB.min.x, colliderAABB.max.y, 0 }, { colliderAABB.max.x, colliderAABB.max.y, 0 } };
    if ( pickupCollider.intersect( bottomEdge ) )
    {
        // Set the position of the pickup to the bottom edge of the collider.
        pos.y = colliderAABB.max.y + pickupCollider.radius;
        // And negate the velocity.
        vel.y = -vel.y;

        return;
    }

    // Check to see if the pickup is colliding with the left edge of the collider.
    Line leftEdge { { colliderAABB.min.x, colliderAABB.min.y, 0 }, { colliderAABB.min.x, colliderAABB.max.y, 0 } };
    if ( pickupCollider.intersect( leftEdge ) )
    {
        // Set the position of the pickup to the left edge of the collider.
        pos.x = colliderAABB.min.x - pickupCollider.radius;
        // And negate the x velocity
        vel.x = -vel.x;

        return;
    }

    // Check to see if the pickup is colliding with the right edge of the collider.
    Line rightEdge { { colliderAABB.max.x, colliderAABB.min.y, 0 }, { colliderAABB.max.x, colliderAABB.max.y, 0 } };
    if ( pickupCollider.intersect( rightEdge ) )
    {
        // Set the position of the pickup to the right edge of the collider.
        pos.x = colliderAABB.max.x + pickupCollider.radius;
        // And negate the x velocity.
        vel.x = -vel.x;

        return;
    }
}

void Level::updatePickups( float deltaTime )
{
    for ( auto iter = allPickups.begin(); iter != allPickups.end(); )
    {
        if ( iter->collides( player ) )
        {
            // Play the collected animation at the pickup location.
            effects.emplace_back( pickupCollected, iter->getTransform() );
            iter = allPickups.erase( iter );
        }
        else
            ++iter;
    }

    // update remaining pickups.
    for ( auto& pickup: allPickups )
    {
        pickup.update( deltaTime );

        Sphere    pickupCollider = pickup.getCollider();
        glm::vec2 pos            = pickup.getPosition();
        glm::vec2 vel            = pickup.getVelocity();

        // Check if the pickup collides with a level collider.
        for ( auto& collider: colliders )
        {
            AABB colliderAABB = collider.aabb;
            checkPickupCollision( pickupCollider, colliderAABB, pos, vel );
        }

        // Collide with boxes.
        for ( auto& box: boxes )
        {
            AABB boxCollider = box->getAABB();
            checkPickupCollision( pickupCollider, boxCollider, pos, vel );
        }

        // Dampen velocity.
        vel /= 1.0f + 2.0f * deltaTime;

        // Update the pickup's position and velocity.
        pickup.setPosition( pos );
        pickup.setVelocity( vel );
    }
}

void Level::updateEffects( float deltaTime )
{
    for ( auto iter = effects.begin(); iter != effects.end(); )
    {
        iter->update( deltaTime );
        if ( iter->isDone() )
        {
            iter = effects.erase( iter );
        }
        else
        {
            ++iter;
        }
    }
}

void Level::updateBoxes( float deltaTime )
{
    glm::vec2 vel        = player.getVelocity();
    glm::vec2 pos        = player.getPosition();
    AABB      playerAABB = player.getAABB();

    // Number of pixels padding to account for collisions.
    const float padding = 3.0f;

    bool onGround    = false;
    bool onLeftWall  = false;
    bool onRightWall = false;

    for ( auto iter = boxes.begin(); iter != boxes.end(); ++iter)
    {
        auto& box = *iter;

        AABB boxCollider = box->getAABB();

        // Player is moving down (falling).
        if ( vel.y < 0.0f )
        {
            // Check to see if the player is colliding with the top edge of the collider.
            Line topEdge { { boxCollider.min.x + padding, boxCollider.min.y, 0 }, { boxCollider.max.x - padding, boxCollider.min.y, 0 } };
            if ( playerAABB.intersect( topEdge ) )
            {
                // Set the player's position to the top of the AABB.
                pos.y = boxCollider.min.y;
                // And make the player jump off the box:
                player.setState( Player::State::Jump );

                // Jumping changes the player's velocity.
                vel = player.getVelocity();

                // Hit the box.
                box->hit();

                continue;
            }
        }
        // Player is moving up.
        else if ( vel.y > 0 )
        {
            // Check to see if the player is colliding with the bottom edge of the collider.
            Line bottomEdge { { boxCollider.min.x + padding, boxCollider.max.y, 0 }, { boxCollider.max.x - padding, boxCollider.max.y, 0 } };
            if ( playerAABB.intersect( bottomEdge ) )
            {
                // Set the player's position to the bottom of the collider.
                pos.y = boxCollider.max.y + playerAABB.height();
                // And 0 the Y velocity.
                vel.y = 0.0f;

                // Hit the box.
                box->hit();

                // And start falling
                player.setState( Player::State::Falling );

                continue;
            }
        }
        // Player is Idle or running.
        else
        {
            // Check to see if the player is colliding with the top edge of the collider.
            Line topEdge { { boxCollider.min.x + padding, boxCollider.min.y, 0 }, { boxCollider.max.x - padding, boxCollider.min.y, 0 } };
            if ( playerAABB.intersect( topEdge ) )
            {
                onGround = true;
            }
        }

        // Check to see if the player is colliding with the left edge of the box.
        Line leftEdge { { boxCollider.min.x, boxCollider.min.y + padding, 0 }, { boxCollider.min.x, boxCollider.max.y - padding, 0 } };
        if ( playerAABB.intersect( leftEdge ) )
        {
            // Set the player's position to the left edge of the box.
            pos.x = boxCollider.min.x - playerAABB.width() * 0.5f;
            // And 0 the X velocity.
            vel.x = 0.0f;

            onRightWall = true;
            continue;
        }

        // Check to see if the player is colliding with the right edge of the collider.
        Line rightEdge { { boxCollider.max.x, boxCollider.min.y + padding, 0 }, { boxCollider.max.x, boxCollider.max.y - padding, 0 } };
        if ( playerAABB.intersect( rightEdge ) )
        {
            // Set the player's position to the right edge of the collider.
            pos.x = boxCollider.max.x + playerAABB.width() * 0.5f;
            // And 0 the X velocity.
            vel.x = 0.0f;

            // Player is on a wall that is to the left of the player.
            onLeftWall = true;
            continue;
        }
    }

    // Update the boxes.
    for ( auto iter = boxes.begin(); iter != boxes.end(); )
    {
        auto& box = *iter;

        // Update the box.
        box->update( deltaTime );

        Box::State boxState = box->getState();
        if ( boxState == Box::State::Break )
        {
            AABB boxCollider = box->getAABB();
            iter             = boxes.erase( iter );

            // Spawn some fruit out of the box.
            addPickup( "Apple", glm::vec2 { boxCollider.center() } );
            addPickup( "Banana", glm::vec2 { boxCollider.center() } );
            addPickup( "Cherry", glm::vec2 { boxCollider.center() } );
            addPickup( "Kiwi", glm::vec2 { boxCollider.center() } );
            addPickup( "Melon", glm::vec2 { boxCollider.center() } );
            addPickup( "Strawberry", glm::vec2 { boxCollider.center() } );
        }
        else
        {
            ++iter;
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

void Level::setCharacter( size_t characterId )
{
    player.setCharacter( characterId );
}

void Level::draw( sr::Image& image ) const
{
    tileMap.draw( image );

    for ( auto& pickup: allPickups )
    {
        pickup.draw( image );
    }

    for ( auto& effect: effects )
    {
        effect.draw( image );
    }

    for ( auto& box: boxes )
    {
        box->draw( image );
    }

    player.draw( image );

#if _DEBUG
    for ( const auto& collider: colliders )
    {
        image.drawAABB( collider.aabb, collider.isOneWay ? Color::Yellow : Color::Red, BlendMode::Disable, FillMode::WireFrame );
    }
#endif
}

void Level::addPickup( std::string_view name, const glm::vec2& p )
{
    // Random distribution between 0..PI (for generating random vectors on a hemisphere).
    static std::uniform_real_distribution dist { 0.0f, std::numbers::pi_v<float> };

    auto&  fruitSprite = fruitSprites[std::string( name )];
    Sphere collider { { p.x, p.y, 0 }, 8.0f };

    // Generate a random initial velocity for the pickup.
    const float angle = dist( rng );
    float       x     = std::cos( angle ) * 200.0f;
    float       y     = std::sin( angle ) * -300.0f;

    auto& pickup = allPickups.emplace_back( fruitSprite, collider );
    pickup.setVelocity( { x, y } );  // Give it some initial force.
    pickup.setGravity( -Player::gravity );
}
