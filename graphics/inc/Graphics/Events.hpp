#pragma once

#include "KeyCodes.hpp"

namespace Graphics
{
/// <summary>
/// State of a key on the keyboard.
/// </summary>
enum class KeyState
{
    /// <summary>
    /// The key was pressed this frame.
    /// </summary>
    Pressed = 0,
    /// <summary>
    /// The key was released this frame.
    /// </summary>
    Released = 1
};

/// <summary>
/// The state of the a mouse or joystick button.
/// </summary>
enum class ButtonState
{
    /// <summary>
    /// The button is not being held.
    /// </summary>
    Up = 0,
    /// <summary>
    /// The button is being held.
    /// </summary>
    Held = 1,
    /// <summary>
    /// The button was released this frame.
    /// </summary>
    Released = 2,
    /// <summary>
    /// The button was pressed this frame.
    /// </summary>
    Pressed = 3
};

/// <summary>
/// The mouse button that was pressed or released.
/// </summary>
enum class MouseButton
{
    /// <summary>
    /// Unknown mouse button.
    /// </summary>
    None = 0,
    /// <summary>
    /// The left mouse button.
    /// </summary>
    Left = 1,
    /// <summary>
    /// The right mouse button.
    /// </summary>
    Right = 2,
    /// <summary>
    /// The middle mouse button.
    /// </summary>
    Middle = 3,
    /// <summary>
    /// The first extra mouse button.
    /// </summary>
    XButton1 = 4,
    /// <summary>
    /// The second extra mouse button.
    /// </summary>
    XButton2 = 5
};

/// <summary>
/// A key on the keyboard was pressed or released.
/// </summary>
struct KeyEventArgs
{
    /// <summary>
    /// The key code of the key that was pressed or released.
    /// </summary>
    KeyCode code;
    /// <summary>
    /// The 32-bit character code of the key that was pressed or released.
    /// </summary>
    unsigned int character;
    /// <summary>
    /// Was the key pressed or released?
    /// </summary>
    KeyState state;
    /// <summary>
    /// `true` if the Ctrl key was pressed when this event was triggered.
    /// </summary>
    bool ctrl;
    /// <summary>
    /// `true` if the Shift key was pressed when this event was triggered.
    /// </summary>
    bool shift;
    /// <summary>
    /// `true` if the Alt key was pressed when this event was triggered.
    /// </summary>
    bool alt;
    /// <summary>
    /// `true` if the Windows key or the Command key is pressed.
    /// </summary>
    bool super;
};

/// <summary>
/// The mouse was moved.
/// </summary>
struct MouseMovedEventArgs
{
    /// <summary>
    /// `true` if the left mouse button was pressed when this event was triggered.
    /// </summary>
    bool leftButton;
    /// <summary>
    /// `true` if the middle mouse button was pressed when this event was triggered.
    /// </summary>
    bool middleButton;
    /// <summary>
    /// `true` if the right mouse button was pressed when this event was triggered.
    /// </summary>
    bool rightButton;
    /// <summary>
    /// `true` if the Ctrl key was pressed when this event was triggered.
    /// </summary>
    bool ctrl;
    /// <summary>
    /// `true` if the Shift key was pressed when this event was triggered.
    /// </summary>
    bool shift;
    /// <summary>
    /// `true` if the Alt key was pressed when this event was triggered.
    /// </summary>
    bool alt;
    /// <summary>
    /// `true` if the Windows key or the Command key is pressed.
    /// </summary>
    bool super;
    /// <summary>
    /// The X position of the mouse cursor relative to the upper-left corner of the
    /// window's client area.
    /// </summary>
    int x;
    /// <summary>
    /// The Y position of the mouse cursor relative to the upper-left corner of the
    /// window's client area.
    /// </summary>
    int y;
    /// <summary>
    /// The X position of the mouse cursor in screen space pixels.
    /// </summary>
    int screenX;
    /// <summary>
    /// The Y position of the mouse cursor in screen space pixels.
    /// </summary>
    int screenY;
    /// <summary>
    /// The relative X mouse motion since the last event.
    /// </summary>
    int relX;
    /// <summary>
    /// The relative Y mouse motion since the last event.
    /// </summary>
    int relY;
};

/// <summary>
/// A button on the mouse was pressed or released.
/// </summary>
struct MouseButtonEventArgs
{
    /// <summary>
    /// The mouse button that was pressed or released.
    /// </summary>
    MouseButton button;
    /// <summary>
    /// Was the button pressed or released?
    /// </summary>
    ButtonState state;
    /// <summary>
    /// `true` if the left mouse button was pressed when this event was triggered.
    /// </summary>
    bool leftButton;
    /// <summary>
    /// `true` if the middle mouse button was pressed when this event was triggered.
    /// </summary>
    bool middleButton;
    /// <summary>
    /// `true` if the right mouse button was pressed when this event was triggered.
    /// </summary>
    bool rightButton;
    /// <summary>
    /// `true` if the Ctrl key was pressed when this event was triggered.
    /// </summary>
    bool ctrl;
    /// <summary>
    /// `true` if the Shift key was pressed when this event was triggered.
    /// </summary>
    bool shift;
    /// <summary>
    /// `true` if the Alt key was pressed when this event was triggered.
    /// </summary>
    bool alt;
    /// <summary>
    /// `true` if the Windows key or the Command key is pressed.
    /// </summary>
    bool super;
    /// <summary>
    /// The X position of the mouse cursor relative to the upper-left corner of the
    /// window's client area.
    /// </summary>
    int x;
    /// <summary>
    /// The Y position of the mouse cursor relative to the upper-left corner of the
    /// window's client area.
    /// </summary>
    int y;
    /// <summary>
    /// The X position of the mouse cursor in screen space pixels.
    /// </summary>
    int screenX;
    /// <summary>
    /// The Y position of the mouse cursor in screen space pixels.
    /// </summary>
    int screenY;
};

/// <summary>
/// The mouse wheel was scrolled.
/// </summary>
struct MouseWheelEventArgs
{
    /// <summary>
    /// The change in mouse wheel position.
    /// </summary>
    float wheelDelta;
    /// <summary>
    /// `true` if the left mouse button was pressed when this event was triggered.
    /// </summary>
    bool leftButton;
    /// <summary>
    /// `true` if the middle mouse button was pressed when this event was triggered.
    /// </summary>
    bool middleButton;
    /// <summary>
    /// `true` if the right mouse button was pressed when this event was triggered.
    /// </summary>
    bool rightButton;
    /// <summary>
    /// `true` if the Ctrl key was pressed when this event was triggered.
    /// </summary>
    bool ctrl;
    /// <summary>
    /// `true` if the Shift key was pressed when this event was triggered.
    /// </summary>
    bool shift;
    /// <summary>
    /// `true` if the Alt key was pressed when this event was triggered.
    /// </summary>
    bool alt;
    /// <summary>
    /// `true` if the Windows key or the Command key is pressed.
    /// </summary>
    bool super;
    /// <summary>
    /// The X position of the mouse cursor relative to the upper-left corner of the
    /// window's client area.
    /// </summary>
    int x;
    /// <summary>
    /// The Y position of the mouse cursor relative to the upper-left corner of the
    /// window's client area.
    /// </summary>
    int y;
    /// <summary>
    /// The X position of the mouse cursor in screen space pixels.
    /// </summary>
    int screenX;
    /// <summary>
    /// The Y position of the mouse cursor in screen space pixels.
    /// </summary>
    int screenY;
};

/// <summary>
/// The state of the window during a resize event.
/// </summary>
enum class WindowState
{
    /// <summary>
    /// The window has been resized.
    /// </summary>
    Resized = 0,
    /// <summary>
    /// The window has been restored.
    /// </summary>
    Restored = 1,
    /// <summary>
    /// The window has been minimized.
    /// </summary>
    Minimized = 2,
    /// <summary>
    /// The window has been maximized.
    /// </summary>
    Maximized = 3
};

/// <summary>
/// The window was resized.
/// </summary>
struct ResizeEventArgs
{
    /// <summary>
    /// The width of the window's client area.
    /// </summary>
    int width;
    /// <summary>
    /// The height of the window's client area.
    /// </summary>
    int height;
    /// <summary>
    /// The state of the window that triggered the event.
    /// </summary>
    WindowState state;
};
struct Event
{
    enum Type
    {
        /// <summary>
        /// Default (unknown) event.
        /// </summary>
        None,
        /// <summary>
        /// The window is closing.
        /// </summary>
        Close,
        /// <summary>
        /// A key on the keyboard was pressed.
        /// </summary>
        KeyPressed,
        /// <summary>
        /// A key on the keyboard was released.
        /// </summary>
        KeyReleased,
        /// <summary>
        /// The mouse was moved or dragged over the window.
        /// </summary>
        MouseMoved,
        /// <summary>
        /// A button on the mouse was pressed.
        /// </summary>
        MouseButtonPressed,
        /// <summary>
        /// A button on the mouse was released.
        /// </summary>
        MouseButtonReleased,
        /// <summary>
        ///  The vertical mouse wheel was scrolled.
        /// </summary>
        MouseWheel,
        /// <summary>
        /// The horizontal mouse wheel was scrolled.
        /// </summary>
        MouseHWheel,
        /// <summary>
        /// The mouse entered the client area of the window.
        /// </summary>
        MouseEnter,
        /// <summary>
        /// The mouse left the client area of the window.
        /// </summary>
        MouseLeave,
        /// <summary>
        /// The window was resized.
        /// </summary>
        Resize,
        /// <summary>
        /// The window is finished resizing.
        /// </summary>
        EndResize,
    };

    Type type = Event::None;

    union
    {
        /// <summary>
        /// `KeyPressed` and `KeyReleased` events.
        /// </summary>
        KeyEventArgs key;
        /// <summary>
        /// `Resize`, `BeginResize`, `EndResize`, `Minimize`, `Maximize`, and `Restore`
        /// events.
        /// </summary>
        ResizeEventArgs resize;
        /// <summary>
        /// `MouseMoved` and `MouseEnter` events.
        /// </summary>
        MouseMovedEventArgs mouseMove;
        /// <summary>
        /// `MouseButtonPressed` and `MouseButtonReleased` events.
        /// </summary>
        MouseButtonEventArgs mouseButton;
        /// <summary>
        /// `MouseWheel` and `MouseHWheel` events.
        /// </summary>
        MouseWheelEventArgs mouseWheel;
    };
};
}  // namespace Graphics