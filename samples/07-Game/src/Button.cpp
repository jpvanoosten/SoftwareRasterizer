#include "Button.hpp"

#include <numbers>

using namespace sr;
using namespace Math;

Transform2D operator+( Transform2D t, const glm::vec2& v )
{
    t.translate( v );
    return t;
}

Transform2D operator-( Transform2D t, const glm::vec2& v )
{
    t.translate( -v );
    return t;
}

Button::Button( const sr::SpriteSheet& sheet, const Math::Transform2D& transform, const std::function<void()>& onClick )
: spriteSheet { sheet }
, transform { transform }
, onClick { onClick }
{
    aabb = AABB::fromRect( spriteSheet[0].getRect() );
    aabb *= transform;

    // A simple animation curve when the button changes state.
    animCurve = Curve<float>( []( float x ) {
        const float p = 0.2f;
        if (x < p)
            return 2.0f * std::abs( x / p - std::floor( x / p + 0.5f ) );
        return 0.0f;
    } );
}

void Button::processEvents( const sr::Event& event )
{
    if ( !enabled )
        return;

    switch ( event.type )
    {
    case Event::MouseMoved:
        if ( aabb.contains( { event.mouseMove.x, event.mouseMove.y, 0 } ) )
            setState( State::Hover );
        else
            setState( State::Default );
        break;

    case Event::MouseButtonPressed:
        if ( state == State::Hover )
            setState( State::Pressed );
        break;

    case Event::MouseButtonReleased:
        if ( state == State::Pressed )
            if ( onClick )
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

    // Button animation.
    animTimer.tick();
    const float animTime = static_cast<float>( animTimer.totalSeconds() );
    const float yOffset  = animCurve( animTime );

    if ( spriteToDraw )
        image.drawSprite( *spriteToDraw, transform - glm::vec2 { 0, state == State::Pressed ? -yOffset : yOffset } );

    // image.drawAABB( aabb, Color::Red, {}, FillMode::WireFrame );
}

void Button::setState( State newState )
{
    if (state != newState)
    {
        endState( state );
        state = newState;
        startState( state );
    }
}

void Button::endState( State oldState ) {}

void Button::startState( State newState )
{
    switch ( newState )
    {
    case State::Hover:
    case State::Pressed:
        animTimer.reset();
        break;
    }
}
