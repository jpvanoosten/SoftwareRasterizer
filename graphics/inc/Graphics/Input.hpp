#pragma once

#include "Config.hpp"
#include "Events.hpp"
#include "GamePadStateTracker.hpp"
#include "KeyboardStateTracker.hpp"
#include "MouseStateTracker.hpp"

#include <functional>
#include <span>
#include <string_view>

namespace Graphics
{
/// <summary>
/// An AxisCallback function takes a GamePadStateTracker (for each connected GamePad), KeyboardStateTracker, and a MouseStateTracker and returns
/// the value of the axis being queried (in the range [-1...1]).
/// </summary>
using AxisCallback = std::function<float(std::span<const GamePadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker&)>;

/// <summary>
/// A ButtonCallback function takes a GamePadStateTracker (for each connected GamePad), a KeyboardStateTracker, and a MouseStateTracker and returns
/// the state of the button.
/// </summary>
using ButtonCallback = std::function<bool(std::span<const GamePadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker&)>;

/// <summary>
/// A KeyCallback function takes a KeyboardStateTracker and returns the key state.
/// </summary>
using KeyCallback = std::function<bool(const KeyboardStateTracker&)>;

/// <summary>
/// A MouseButtonCallback takes a MouseStateTracker and returns the state of the mouse button.
/// </summary>
using MouseButtonCallback = std::function<bool(MouseStateTracker&)>;

class SR_API Input
{
public:
    /// <summary>
    /// Update the input state. Should only be called once per frame.
    /// </summary>
    static void update();

    /// <summary>
    /// Returns the value of the axis identified by axisName.
    /// </summary>
    /// <param name="axisName"></param>
    /// <returns>A value in the range [-1...1] that represents the value of the axis.</returns>
    static float getAxis( std::string_view axisName );

    /// <summary>
    /// Returns the value of the button identified by buttonName.
    /// </summary>
    /// <param name="buttonName">The name of the button to query.</param>
    /// <returns>`true` if the button is pressed, `false` otherwise.</returns>
    static bool getButton( std::string_view buttonName );

    /// <summary>
    /// Returns `true` in the frame that the button is pressed.
    /// </summary>
    /// <param name="buttonName">The name of the button to query.</param>
    /// <returns>`true` if the button was pressed this frame, `false` otherwise.</returns>
    static bool getButtonDown( std::string_view buttonName );

    /// <summary>
    /// Returns `true` in the frame that the button is released.
    /// </summary>
    /// <param name="buttonName">The name of the button to query.</param>
    /// <returns>`true` if the button was released this frame, `false` otherwise.</returns>
    static bool getButtonUp( std::string_view buttonName );

    /// <summary>
    /// Returns `true` while the user is holding down a specific key on the keyboard.
    /// </summary>
    /// <param name="keyName">The name of the key to query.</param>
    /// <returns>`true` if the key is being held, `false` otherwise.</returns>
    static bool getKey( std::string_view keyName );

    /// <summary>
    /// Returns `true` in the frame that the key is pressed.
    /// </summary>
    /// <param name="keyName">The name of the key to query.</param>
    /// <returns>`true` if the key is pressed this frame, `false` otherwise.</returns>
    static bool getKeyDown( std::string_view keyName );

    /// <summary>
    /// Return `true` in the frame that the key is released.
    /// </summary>
    /// <param name="keyName">The name of the key to query.</param>
    /// <returns>`true` if the key is released this frame.</returns>
    static bool getKeyUp( std::string_view keyName );

    /// <summary>
    /// Returns `true` while the user is holding down a specific key on the keyboard.
    /// </summary>
    /// <param name="key">The KeyCode enum to query.</param>
    /// <returns>`true` if the key is being held, `false` otherwise.</returns>
    static bool getKey( KeyCode key );

    /// <summary>
    /// Returns `true` in the frame that the key is pressed.
    /// </summary>
    /// <param name="key">The KeyCode enum to query.</param>
    /// <returns>`true` if the key is pressed this frame, `false` otherwise.</returns>
    static bool getKeyDown( KeyCode key );

    /// <summary>
    /// Returns `true` in the frame that the key is released.
    /// </summary>
    /// <param name="key">The KeyCode enum to query.</param>
    /// <returns></returns>
    static bool getKeyUp( KeyCode key );

    /// <summary>
    /// Returns true when the given mouse button is held down.
    /// </summary>
    /// <param name="button">The MouseButton enum to check.</param>
    /// <returns>`true` if the mouse button is pressed, `false` otherwise.</returns>
    static bool getMouseButton( MouseButton button );

    /// <summary>
    /// Returns true when the given mouse button is pressed this frame.
    /// </summary>
    /// <param name="button">The MouseButton enum to check.</param>
    /// <returns>`true` if the mouse button is pressed this frame, `false` otherwise.</returns>
    static bool getMouseButtonDown( MouseButton button );

    /// <summary>
    /// Returns true when the give mouse button is released this frame.
    /// </summary>
    /// <param name="button">The MouseButton enum to check.</param>
    /// <returns>`true` if the mouse button is released this frame.</returns>
    static bool getMouseButtonUp( MouseButton button );

    /// <summary>
    /// Map an axis name to an axis callback function.
    /// </summary>
    /// <param name="axisName">The name of the axis to map.</param>
    /// <param name="callback">The callback function to use to return the value of the axis.</param>
    static void mapAxis( std::string_view axisName, AxisCallback callback );

    /// <summary>
    /// Map a button name to a button callback function.
    /// </summary>
    /// <param name="buttonName"></param>
    /// <param name="callback"></param>
    static void mapButton( std::string_view buttonName, ButtonCallback callback );

    /// <summary>
    /// Map a button name to a button callback function.
    /// </summary>
    /// <param name="buttonName">The button to map to the callback function.</param>
    /// <param name="callback">The callback function.</param>
    static void mapButtonDown( std::string_view buttonName, ButtonCallback callback );

    /// <summary>
    /// Map a button name to a button callback function.
    /// </summary>
    /// <param name="buttonName">The button name to map to the callback function.</param>
    /// <param name="callback">The callback function./param>
    static void mapButtonUp( std::string_view buttonName, ButtonCallback callback );
    
    // Static class, delete constructors and assignment operators.
    Input()                    = delete;
    ~Input()                   = delete;
    Input( const Input& )      = delete;
    Input( Input && ) noexcept = delete;

    Input& operator=( const Input& ) = delete;
    Input& operator=( Input&& )      = delete;
};
}  // namespace Graphics
