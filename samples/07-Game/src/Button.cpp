#include "Button.hpp"

#include <utility>

using namespace sr;
using namespace Math;

Button::Button( sr::SpriteSheet sheet, const Math::Transform2D& transform, std::function<void()> onClick )
: spriteSheet { std::move( sheet ) }
, transform { transform }
, onClick { std::move( onClick ) }
{
    aabb = AABB::fromRect( spriteSheet[0].getRect() );
    aabb *= transform;
}

void Button::processEvents( const sr::Event& event )
{
    if ( !enabled )
        return;

    switch ( event.type )
    {
    case Event::MouseMoved:
        break;

    case Event::MouseButtonPressed:
        break;

    case Event::MouseButtonReleased:
        break;
    }
}

void Button::draw( sr::Image& image )
{
    if ( !enabled )
        return;

    const Sprite* spriteToDraw = &spriteSheet[0];

    switch ( state )
    {
    case State::Hover:
        if ( spriteSheet.getNumSprites() > 1 )
            spriteToDraw = &spriteSheet[1];
        break;
    case State::Pressed:
        if ( spriteSheet.getNumSprites() > 2 )
            spriteToDraw = &spriteSheet[2];
        break;
    }

    if ( spriteToDraw )
        image.drawSprite( *spriteToDraw, transform );

    image.drawAABB( aabb, Color::Red, {}, FillMode::WireFrame );
}
