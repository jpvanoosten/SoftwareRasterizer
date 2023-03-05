#pragma once

#include <Graphics/Events.hpp>
#include <Graphics/Font.hpp>
#include <Graphics/Image.hpp>
#include <Graphics/SpriteSheet.hpp>

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
    /// <param name="font">The font to use to render the button text.</param>
    /// <param name="color">(optional) The color of the button text. Default: White.</param>
    /// <param name="rect">(optional) The rectangle that represents the button's position on screen. Default: 0</param>
    /// <param name="onClick">(optional) The callback function to invoke when the button is clicked. Default: none</param>
    Button( std::string_view text, std::shared_ptr<Graphics::Font> font, const Graphics::Color& color = Graphics::Color::White, const Math::RectF& rect = {}, std::function<void()> onClick = {} );

    void setRect( const Math::RectF& rect ) noexcept;

    void setCallback( std::function<void()> _onClick ) noexcept
    {
        onClick = std::move( _onClick );
    }

    void setDefaultSprite( const Graphics::Sprite& sprite ) noexcept
    {
        defaultSprite = sprite;
    }

    void setHoverSprite( const Graphics::Sprite& sprite ) noexcept
    {
        hoverSprite = sprite;
    }

    void setPressedSprite( const Graphics::Sprite& sprite ) noexcept
    {
        pressedSprite = sprite;
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
    void processEvents( const Graphics::Event& event );

    /// <summary>
    /// Draw this button to the image.
    /// </summary>
    /// <param name="image">The image to draw this button to.</param>
    void draw( Graphics::Image& image );

private:
    void setState( State newState );
    void endState( State oldState );
    void startState( State newState );

    std::function<void()> onClick;

    std::string                     buttonText;
    std::shared_ptr<Graphics::Font> buttonFont;
    Graphics::Color                 textColor;

    Math::Transform2D transform;
    Math::AABB        aabb;

    Graphics::Sprite defaultSprite;
    Graphics::Sprite hoverSprite;
    Graphics::Sprite pressedSprite;

    State state   = State::Default;
    bool  enabled = true;
};
