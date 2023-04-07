#include <Game.hpp>
#include <Level.hpp>

#include "ParseRects.hpp"
#include "Stages.hpp"

using namespace Graphics;

// Get the spritesheet for the bricks.
std::shared_ptr<SpriteSheet> GetBrickSprites()
{
    // Lazy initialize static brick sprites (avoid loading more than once).
    static std::shared_ptr<SpriteSheet> sprites = std::make_shared<SpriteSheet>( "assets/Arkanoid/blocks.png", ParseRects( "assets/Arkanoid/blocks.xml" ) );
    return sprites;
}

// Get a brick prefab.
const Brick& GetBrick( Stages::B b )
{
    static auto  sprites  = GetBrickSprites();
    static Brick bricks[] = {
        { sprites, {}, 0, 0 },                                // None.
        { sprites, { { 0 } }, 50, 1 },                        // White.
        { sprites, { { 1 } }, 60, 1 },                        // Orange.
        { sprites, { { 2 } }, 70, 1 },                        // Cyan.
        { sprites, { { 3 } }, 80, 1 },                        // Green.
        { sprites, { { 4 } }, 90, 1 },                        // Red.
        { sprites, { { 5 } }, 100, 1 },                       // Blue.
        { sprites, { { 6 } }, 110, 1 },                       // Magenta.
        { sprites, { { 7 } }, 120, 1 },                       // Yellow.
        { sprites, { { 8, 9, 10, 11, 12, 13 } }, 50, 2 },     // Sliver.
        { sprites, { { 14, 15, 16, 17, 18, 19 } }, 0, 255 },  // Gold.
    };

    return bricks[b];
}

Level::Level( Game& game, int levelId )
: pGame { &game }
{
    Stages::Stage stage = Stages::stages[levelId];

    // Bricks start at (8,24) pixels in screen space.
    int y = 24;
    for ( auto& row: stage.b )
    {
        int x = 8;
        for ( auto b: row )
        {
            // Get the brick prefab.
            Brick brick = GetBrick( b );
            brick.setPosition( { x, y } );
            // Check for silver bricks.
            if ( b == Stages::Si )
            {
                // Silver bricks take more than one hit to destroy.
                // In the beginning, they only require two to destroy, but the number of hits it takes to remove them increases by one every eight stages.
                brick.setHitPoints( 2 + ( levelId + 1 ) / 8 );
                // To calculate the number of points Silver bricks are worth, multiply 50 by the stage number.
                brick.setPoints( 50 * ( levelId + 1 ) );
            }

            bricks.push_back( brick );

            x += 16;  // Bricks are 16 pixels wide.
        }
        y += 8;  // Bricks are 8 pixels high.
    }
}

void Level::update( float deltaTime )
{
    for ( auto& brick: bricks )
    {
        brick.update( deltaTime );
    }
}

void Level::draw( Graphics::Image& image ) const
{
    for ( const auto& brick: bricks )
    {
        brick.draw( image );
    }
}

void Level::animateBricks()
{
    for ( auto& b: bricks )
    {
        b.animate();
    }
}

std::optional<Physics::HitInfo> Level::checkCollision( const Ball& ball )
{
    for ( auto& brick: bricks )
    {
        if ( brick.getHitPoints() > 0 )
        {
            if ( const auto hit = brick.checkCollision( ball.getCircle(), ball.getVelocity() ) )
            {
                if ( brick.hit() == 0 && pGame != nullptr) // Block is destroyed.
                {
                    pGame->addPoints( brick.getPoints() );
                }
                return hit;
            }
        }
    }

    return {};
}
