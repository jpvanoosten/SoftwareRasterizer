#pragma once

#include "Config.hpp"
#include "Events.hpp"  // For ButtonState
#include "MouseState.hpp"

namespace Graphics
{
/// <summary>
/// The MouseStateTracker can be used to check if the state of the mouse buttons have
/// changed since the last time the state of the mouse was queried. This alleviates the
/// user from having to keep track of the mouse state separately to detect if a mouse
/// button has been pressed or released this frame.
/// </summary>
/// <example>
/// The following example shows how to use the `MouseStateTracker`:
/// <code>
/// // TODO: Create an example using MouseStateTracker.
/// </code>
/// </example>
struct SR_API MouseStateTracker
{
    /// <summary>
    /// The state of the left mouse button.
    /// </summary>
    /// <see cref="ButtonState" />
    ButtonState leftButton;

    /// <summary>
    /// The state of the middle mouse button.
    /// </summary>
    /// <see cref="ButtonState" />
    ButtonState middleButton;

    /// <summary>
    /// The state of the right mouse button.
    /// </summary>
    /// <see cref="ButtonState" />
    ButtonState rightButton;

    /// <summary>
    /// The state of the first extra mouse button.
    /// </summary>
    /// <see cref="ButtonState" />
    ButtonState xButton1;

    /// <summary>
    /// The state of the second extra mouse button.
    /// </summary>
    /// <see cref="ButtonState" />
    ButtonState xButton2;

    /// <summary>
    /// The change in the X position of the mouse.
    /// </summary>
    int x = 0;

    /// <summary>
    /// The change in Y position of the mouse.
    /// </summary>
    int y = 0;

    /// <summary>
    /// The change in the horizontal scroll wheel of the mouse.
    /// </summary>
    float hScrollWheel = 0.0f;

    /// <summary>
    /// The change in the vertical scroll wheel of the mouse.
    /// </summary>
    float vScrollWheel = 0.0f;

    MouseStateTracker() noexcept;

    /// <summary>
    /// Update the MouseStateTracker with the current mouse state. The current mouse state
    /// is retrieved from `Mouse::getState()`.
    /// </summary>
    /// <param name="state">The current mouse state.</param>
    /// <returns>`true` if the new state is different than the previous state, `false`
    /// otherwise.</returns>
    /// <see cref="Mouse::getState" />
    bool update( const MouseState& state ) noexcept;

    /// <summary>
    /// Reset the mouse state. All buttons are set to the ButtonState::Up state.
    /// </summary>
    void reset() noexcept;

    /// <summary>
    /// Get the last known mouse state.
    /// </summary>
    /// <returns>The last state that was passed to
    /// `MouseStateTracker::update()`.</returns>
    [[nodiscard]] MouseState getLastState() const noexcept;

private:
    MouseState lastState;
};

inline MouseStateTracker::MouseStateTracker() noexcept // NOLINT(cppcoreguidelines-pro-type-member-init)
{
    reset();
}

inline MouseState MouseStateTracker::getLastState() const noexcept
{
    return lastState;
}
}  // namespace Graphics