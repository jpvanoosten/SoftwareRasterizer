#pragma once

#include "Curve.hpp"

#include <Events.hpp>
#include <Image.hpp>
#include <SpriteSheet.hpp>
#include <Timer.hpp>

#include <Math/AABB.hpp>
#include <Math/Transform2D.hpp>

#include <functional>

class Button final
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
    /// <param name="sheet">A sprite sheet that contains the sprites for the button states.</param>
    /// <param name="transform">The transform to place the button on the screen.</param>
    /// <param name="onClick">The callback function to invoke when the button is clicked.</param>
    Button( const sr::SpriteSheet& sheet, const Math::Transform2D& transform = Math::Transform2D {}, const std::function<void()>& onClick = {} );

    void setTransform( const Math::Transform2D& transform ) noexcept
    {
        this->transform = transform;
        aabb            = Math::AABB::fromRect( spriteSheet[0].getRect() );
        aabb *= transform;
    }

    const Math::Transform2D& getTransform() const noexcept
    {
        return transform;
    }

    void setCallback( std::function<void()> onClick )
    {
        this->onClick = std::move( onClick );
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

    float getWidth() const noexcept
    {
        return aabb.width();
    }

    float getHeight() const noexcept
    {
        return aabb.height();
    }

    glm::vec2 getSize() const noexcept
    {
        return { getWidth(), getHeight() };
    }

    /// <summary>
    /// Forward any window events to the button. This allows the button to handle mouse over/mouse clicked events.
    /// </summary>
    /// <param name="event">The event to process.</param>
    void processEvents( const sr::Event& event );

    /// <summary>
    /// Draw this sprite to the image.
    /// </summary>
    /// <param name="image">The image to draw this button to.</param>
    void draw( sr::Image& image );

private:
    void setState( State newState );
    void endState( State oldState );
    void startState( State newState );

    sr::SpriteSheet       spriteSheet;
    Math::Transform2D     transform;
    Math::AABB            aabb;
    sr::Timer             animTimer;
    Curve<float>          animCurve;
    std::function<void()> onClick;
    State                 state   = State::Default;
    bool                  enabled = true;
};