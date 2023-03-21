#include <Field.hpp>

using namespace Graphics;

Field::Field() = default;

Field::Field( std::shared_ptr<Graphics::SpriteSheet> _fieldSprites )
: fieldSprites { std::move( _fieldSprites ) }
{
    {
        constexpr int frames[] { 9, 10, 11, 12, 11, 10 };
        exitAnim = SpriteAnim { fieldSprites, FPS, frames };
    }
    {
        constexpr int frames[] { 13, 14, 15, 16, 17, 18, 18, 17, 16, 15, 14 };
        leftEnemyGate  = SpriteAnim { fieldSprites, FPS, frames };
        rightEnemyGate = SpriteAnim { fieldSprites, FPS, frames };
    }

    shipSprites[0] = fieldSprites->getSprite( 19 );
    shipSprites[1] = fieldSprites->getSprite( 20 );
    shipSprites[2] = fieldSprites->getSprite( 21 );
    shipSprites[3] = fieldSprites->getSprite( 22 );
    shipSprites[4] = fieldSprites->getSprite( 23 );
}

void Field::setLevel( int _level )
{
    level = _level;
}

void Field::setLives( int _lives )
{
    lives = _lives;
}

void Field::update( float deltaTime )
{
    exitAnim.update( deltaTime );
    leftEnemyGate.update( deltaTime );
    rightEnemyGate.update( deltaTime );
}

void Field::draw( Graphics::Image& image )
{
    // The ship sprite to use is based on the field background.
    Sprite shipSprite;

    // The field to use for the background is based on the current level.
    if ( level < 32 )
    {
        // There are 4 fields. The field to use is (level % 4).
        // Since there are 2 field variations in the sprite sheet,
        // the field to use for this level is (level % 4) * 2.
        int field = ( level % 4 ) * 2;
        image.drawSprite( fieldSprites->getSprite( field ), 0, 16 );

        shipSprite = shipSprites[level % 4];
    }
    else
    {
        // The final stage has a different field at index 8 in the spritesheet.
        image.drawSprite( fieldSprites->getSprite( 8 ), 0, 16 );
        shipSprite = shipSprites[4];
    }

    image.drawSprite( exitAnim, 216, 215 );
    image.drawSprite( leftEnemyGate, 40, 16 );
    image.drawSprite( rightEnemyGate, 152, 16 );

    // Draw a ship sprite for each life.
    for ( int i = 0; i < lives; ++i )
    {
        image.drawSprite( shipSprite, i * shipSprite.getWidth() + 8, 248 );
    }
}
