#pragma once

#include <Events.hpp>
#include <Font.hpp>
#include <Image.hpp>
#include <SpriteSheet.hpp>

#include <functional>

class Button
{
public:
    /// <summary>
    /// The current button state.
    /// </summary>
    enum class State
    {
        Default,  ///< The button is in it's default state.
        Hover,    ///< The mouse is over the button.
        Pressed,  ///< The mouse button is pressed over the button.
    };

    Button() = default;

    /// <summary>
    /// Construct a button.
    /// </summary>
    /// <param name="text">The button text.</param>
    /// <param name="rect">The rectangle that represents the button's position on screen.</param>
    /// <param name="onClick">The callback function to invoke when the button is clicked.</param>
    Button( std::string_view text, const Math::RectF& rect, const std::function<void()>& onClick = {} );

    void setCallback( std::function<void()> _onClick )
    {
        onClick = std::move( _onClick );
    }

    /// <summary>
    /// Check if this button is enabled.
    /// </summary>
    /// <returns>`true` if the button is enabled, otherwise, false.</returns>
    bool isEnabled() const noexcept
    {
        return enabled;
    }

    /// <summary>
    /// Enable/disable the button.
    /// </summary>
    /// <param name="enable">`true` to enable the button, `false` to disable it.</param>
    void enable( bool enable ) noexcept
    {
        enabled = enable;
    }

    /// <summary>
    /// Forward any window events to the button. This allows the button to handle mouse over/mouse clicked events.
    /// </summary>
    /// <param name="event">The event to process.</param>
    void processEvents( const sr::Event& event );

    /// <summary>
    /// Draw this button to the image.
    /// </summary>
    /// <param name="image">The image to draw this button to.</param>
    void draw( sr::Image& image );

private:
    void setState( State newState );
    void endState( State oldState );
    void startState( State newState );

    std::function<void()> onClick;

    std::string buttonText;
    sr::Font    buttonFont;


    Math::RectF rect;
    Math::AABB  aabb;

    sr::Sprite defaultSprite;
    sr::Sprite hoverSprite;
    sr::Sprite pressedSprite;

    State state   = State::Default;
    bool  enabled = true;
};
