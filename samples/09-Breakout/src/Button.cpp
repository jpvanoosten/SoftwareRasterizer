#include "Button.hpp"

#include <iostream>

using namespace Graphics;
using namespace Math;

Button::Button( std::string_view text, std::shared_ptr<Font> font, const Graphics::Color& color, const Math::RectF& rect, std::function<void()> onClick )
: onClick { std::move( onClick ) }
, buttonText { text }
, buttonFont { std::move( font ) }
, textColor { color }
{
    setRect( rect );
}

void Button::setRect( const Math::RectF& rect ) noexcept
{
    aabb = AABB::fromRect( rect );
    transform.setPosition( rect.center() );
    transform.setAnchor( rect.center() - rect.topLeft() );
}

void Button::draw( Graphics::Image& image )
{
    if ( !enabled )
        return;

    Sprite* sprite = defaultSprite ? &defaultSprite : nullptr;

    switch ( state )
    {
    case State::Hover:
        sprite = hoverSprite ? &hoverSprite : sprite;
        break;
    case State::Pressed:
        sprite = pressedSprite ? &pressedSprite : sprite;
        break;
    }

    // Draw the sprite for the button (if there is one)
    if ( sprite )
    {
        image.drawSprite( *sprite, transform );
    }
    // Draw the button text.
    if ( buttonFont && !buttonText.empty() )
    {
        const auto size = buttonFont->getSize( buttonText ) / 2;
        const auto pos  = transform.getPosition() - glm::vec2 { size.x, -size.y };
        image.drawText( *buttonFont, static_cast<int>( pos.x ), static_cast<int>( pos.y ), buttonText, textColor );
    }

#if _DEBUG
    image.drawAABB( aabb, Color::Yellow, {}, FillMode::WireFrame );
#endif
}

void Button::processEvents( const Graphics::Event& event )
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

void Button::startState( State newState )
{
    switch ( newState )
    {
    case State::Default:
        std::cout << buttonText << ": Default" << std::endl;
        break;
    case State::Hover:
        std::cout << buttonText << ": Hovered" << std::endl;
        break;
    case State::Pressed:
        std::cout << buttonText << ": Pressed" << std::endl;
        break;
    }
}
