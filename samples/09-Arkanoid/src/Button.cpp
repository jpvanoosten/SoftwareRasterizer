#include "Button.hpp"

#include "glm/trigonometric.hpp"

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

void Button::setSprite( State _state, const Graphics::Sprite& sprite )
{
    switch ( _state )
    {
    case State::Default:
        defaultSprite = sprite;
        break;
    case State::Hover:
        hoverSprite = sprite;
        break;
    case State::Pressed:
        pressedSprite = sprite;
        break;
    default: ;
    }
}

void Button::setRect( const Math::RectF& rect ) noexcept
{
    aabb = AABB { { 0, 0, 0 }, { rect.width, rect.height, 0 } };
    const glm::vec2 anchor { rect.width / 2.0f, rect.height };
    transform.setPosition( rect.topLeft() + anchor );
    //transform.setPosition( rect.topLeft() );
    transform.setAnchor( anchor );
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
        auto t = transform;
        auto s = sprite->getSize();
        t.setAnchor( { s.x / 2, s.y } );
        image.drawSprite( *sprite, t );
    }
    // Draw the button text.
    if ( buttonFont && !buttonText.empty() )
    {
        // Center the text on the button.
        const auto size = buttonFont->getSize( buttonText );
        const auto pos  = glm::vec2 { getAABB().center() } - glm::vec2 { size.x, -size.y } / 2.0f;
        image.drawText( *buttonFont, buttonText, static_cast<int>( pos.x ), static_cast<int>( pos.y + ( state == State::Pressed ? 5.0f : 0.0f ) ), textColor );
    }

#if _DEBUG
    image.drawAABB( getAABB(), Color::Yellow, {}, FillMode::WireFrame );
#endif
}

void Button::processEvents( const Graphics::Event& event )
{
    if ( !enabled )
        return;

    switch ( event.type )
    {
    case Event::MouseMoved:
        if ( getAABB().contains( { event.mouseMove.x, event.mouseMove.y, 0 } ) )
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
