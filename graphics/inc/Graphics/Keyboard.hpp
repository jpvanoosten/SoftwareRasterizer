#pragma once

#include "Config.hpp"
#include "KeyboardState.hpp"

namespace Graphics
{
class SR_API Keyboard
{
public:
    /// <summary>
    /// Is a keyboard currently connected?
    /// </summary>
    /// <returns>`true` if a keyboard is detected, `false` otherwise.</returns>
    static bool isConnected();

    /// <summary>
    /// Retrieve the current state of the keyboard.
    /// </summary>
    /// <returns>The current keyboard state.</returns>
    static KeyboardState getState();

    /// <summary>
    /// Reset the state of the keyboard.
    /// </summary>
    static void reset();

    // Static class, delete constructors and assignment operators.
    Keyboard()               = delete;
    Keyboard( const Keyboard& ) = delete;
    Keyboard( Keyboard&& )      = delete;
    ~Keyboard()                 = delete;

    Keyboard& operator=( const Keyboard& ) const = delete;
    Keyboard& operator=( Keyboard&& ) const = delete;
};
}  // namespace ls