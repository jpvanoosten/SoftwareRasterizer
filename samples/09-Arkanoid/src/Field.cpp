#include <Field.hpp>

using namespace Graphics;

Field::Field() = default;

Field::Field( std::shared_ptr<Graphics::SpriteSheet> _fieldSprites )
    : fieldSprites{std::move(_fieldSprites)}
{
    {
        constexpr int frames[] { 9, 10, 11, 12, 11, 10 };
        exitAnim = SpriteAnim { fieldSprites, FPS, frames };
    }
    {
        constexpr int frames[] { 13, 14, 15, 16, 17, 18, 18, 17, 16, 15, 14 };
        leftEnemyGate = SpriteAnim { fieldSprites, FPS, frames };
        rightEnemyGate = SpriteAnim { fieldSprites, FPS, frames };
    }
}

void Field::setField( int _field )
{
    field = _field;
}

void Field::update( float deltaTime )
{
    exitAnim.update( deltaTime );
    leftEnemyGate.update( deltaTime );
    rightEnemyGate.update( deltaTime );
}

void Field::draw( Graphics::Image& image )
{
    image.drawSprite( fieldSprites->getSprite( field * 2 ), 0, 16 );
    image.drawSprite( exitAnim, 216, 215 );
    image.drawSprite( leftEnemyGate, 40, 16 );
    image.drawSprite( rightEnemyGate, 152, 16 );
}
