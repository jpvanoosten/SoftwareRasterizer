#pragma once

namespace Graphics
{
/// <summary>
/// The state of the mouse.
/// Use Mouse::getState() to query the current state of the mouse.
/// </summary>
struct MouseState
{
    /// <summary>
    /// The state of the left mouse button.
    /// `true` if the left mouse button is held, `false` otherwise.
    /// </summary>
    bool leftButton;

    /// <summary>
    /// The state of the middle mouse button.
    /// `true` if the middle mouse button is held, `false` otherwise.
    /// </summary>
    bool middleButton;

    /// <summary>
    /// State of the right mouse button.
    /// `true` if the right mouse button is held, `false` otherwise.
    /// </summary>
    bool rightButton;

    /// <summary>
    /// State of the first extra mouse button.
    /// `true` if the first extra mouse button is held, `false` otherwise.
    /// </summary>
    bool xButton1;

    /// <summary>
    /// State of the second extra mouse button.
    /// `true` if the second extra mouse button is held, `false` otherwise.
    /// </summary>
    bool xButton2;

    /// <summary>
    /// The x-coordinate of the mouse cursor relative to the top-left corner of the window that received the mouse event.
    /// </summary>
    /// <remarks>
    /// If the mouse cursor is locked to a window, then this is the relative x mouse
    /// motion, otherwise it is the position of the mouse cursor relative to the window that
    /// sent the event.
    /// </remarks>
    /// <see cref="Mouse::lockToWindow" />
    int x;

    /// <summary>
    /// The y-coordinate of the mouse cursor relative to the top-left corner of the window that received the mouse event.
    /// </summary>
    /// <remarks>
    /// If the mouse cursor is locked to a window, then this is the relative y mouse
    /// motion, otherwise it is the position of the mouse cursor relative to the window that
    /// sent the event.
    /// </remarks>
    /// <see cref="Mouse::lockToWindow" />
    int y;

    /// <summary>
    /// The x-coordinate of the mouse cursor in screen space pixels.
    /// </summary>
    /// <remarks>
    /// If the mouse cursor is locked to a window, the <b>x</b>, and <b>y</b> coordinates are relative
    /// to the window that has the cursor lock. In this case, the <b>screenX</b> and
    /// <b>screenY</b> coordinates are no longer updated and should not be trusted.
    /// </remarks>
    int screenX;

    /// <summary>
    /// The y-coordinate of the mouse cursor in screen space pixels.
    /// </summary>
    /// <remarks>
    /// If the mouse cursor is locked to a window, the <b>x</b>, and <b>y</b> coordinates are relative
    /// to the window that has the cursor lock. In this case, the <b>screenX</b> and
    /// <b>screenY</b> coordinates are no longer updated and should not be trusted.
    /// </remarks>
    int screenY;

    /// <summary>
    /// The value of the mouse's vertical scroll wheel.
    /// </summary>
    float vScrollWheel;

    /// <summary>
    /// The value of the mouse's horizontal scroll wheel.
    /// </summary>
    float hScrollWheel;

    /// <summary>
    /// Used to check if this MouseState is equivalent to another.
    /// </summary>
    /// <remarks>
    /// Used to check if the mouse state has changed since the last time it was queried.
    /// </remarks>
    /// <returns>`true` if they are equal, `false` otherwise.</returns>
    bool operator==( const MouseState& ) const = default;

    /// <summary>
    /// Used to check if this MouseState is different to another.
    /// </summary>
    /// <remarks>
    /// Used to check if the mouse state has changed since the last time it was queried.
    /// </remarks>
    /// <returns>`true` if they are not equal, `false` otherwise.</returns>
    bool operator!=( const MouseState& ) const = default;
};
}  // namespace Graphics