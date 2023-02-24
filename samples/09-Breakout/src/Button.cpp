#include "Button.hpp"

using namespace sr;

void Button::draw( sr::Image& image )
{
    if ( !enabled )
        return;

    switch ( state )
    {
    case State::Default:
        break;
    case State::Hover:
        break;
    case State::Pressed:
        break;
    }

#if _DEBUG
    image.drawAABB( aabb, Color::Yellow, {}, FillMode::WireFrame );
#endif
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

void Button::setState( State newState )
{
    if ( state != newState )
    {
        endState( state );
        state = newState;
        startState( state );
    }
}

void Button::endState( State oldState ) {}

void Button::startState( State newState ) {}
