#pragma once

#include "Config.hpp"
#include "MouseState.hpp"

#include <glm/vec2.hpp>

namespace Graphics
{
class Window;

/// <summary>
/// A static class that is used to retrieve information about the mouse.
/// </summary>
class SR_API Mouse
{
public:
    /// <summary>
    /// Is a mouse is currently connected?
    /// </summary>
    /// <returns>`true` if a mouse is detected, `false` otherwise.</returns>
    static bool isConnected();

    /// <summary>
    /// Is the mouse cursor currently visible?
    /// </summary>
    /// <returns>`true` if the mouse cursor is currently visible, `false`
    /// otherwise.</returns>
    static bool isVisible();

    /// <summary>
    /// Set the visibility of the mouse cursor.
    /// </summary>
    /// <param name="visible">`true` to show the mouse cursor, `false` to hide it.</param>
    static void setVisible( bool visible );

    /// <summary>
    /// Lock the mouse cursor to the window.
    /// </summary>
    /// <remarks>
    /// When the mouse cursor is locked to a window, the mouse coordinates are
    /// relative to the previous frame.
    /// </remarks>
    /// <param name="window">The window to lock the mouse cursor to.</param>
    static void lockToWindow( const Window& window );

    /// <summary>
    /// Unlock the mouse cursor.
    /// </summary>
    /// <remarks>
    /// When the mouse cursor is unlocked, the mouse coordinates are in screen
    /// coordinates.
    /// </remarks>
    static void unlock();

    /// <summary>
    /// Check to see if the mouse is currently locked to a window.
    /// </summary>
    /// <returns>`true` if the mouse is locked to a window, `false` otherwise.</returns>
    static bool isLocked();

    /// <summary>
    /// Get the current state of the mouse.
    /// </summary>
    /// <returns>The current mouse state.</returns>
    static MouseState getState();

    /// <summary>
    /// Get the current position of the mouse cursor, in screen coordinates.
    /// </summary>
    /// <returns>The position of the mouse cursor, in screen coordinates.</returns>
    static glm::ivec2 getPosition();

    /// <summary>
    /// Get the current position of the mouse cursor, in the client coordinates of the
    /// window.
    /// </summary>
    /// <param name="window">The reference window.</param>
    /// <returns>The current position of the mouse cursor in the client coordinates of the
    /// window.</returns>
    static glm::ivec2 getPosition( const Window& window );

    /// <summary>
    /// Set the current position of the mouse, in screen coordinates.
    /// </summary>
    /// <param name="pos">The position of the mouse in screen coordinates.</param>
    static void setPosition( const glm::ivec2& pos );

    /// <summary>
    /// Set the current position of the mouse relative to the window.
    /// </summary>
    /// <param name="pos">The relative position of the mouse cursor.</param>
    /// <param name="window">The reference window.</param>
    static void setPosition( const glm::ivec2& pos, const Window& window );

    // Static class, delete constructors and assignment operators.
    Mouse()               = delete;
    Mouse( const Mouse& ) = delete;
    Mouse( Mouse&& )      = delete;
    ~Mouse()              = delete;

    Mouse& operator=( const Mouse& ) const = delete;
    Mouse& operator=( Mouse&& ) const      = delete;
};
}  // namespace Graphics