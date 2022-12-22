#include "Button.hpp"

using namespace sr;
using namespace Math;

Button::Button( const sr::SpriteSheet& sheet, const Math::Transform2D& transform, const std::function<void()>& onClick )
: spriteSheet { sheet }
, transform { transform }
, onClick { onClick }
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
        if ( aabb.contains( { event.mouseMove.x, event.mouseMove.y, 0 } ) )
            state = State::Hover;
        else
            state = State::Default;
        break;

    case Event::MouseButtonPressed:
        if ( state == State::Hover )
            state = State::Pressed;
        break;

    case Event::MouseButtonReleased:
        if ( state == State::Pressed )
            onClick();
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

    //image.drawAABB( aabb, Color::Red, {}, FillMode::WireFrame );
}
