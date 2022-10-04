#pragma once

#include "KeyCodes.hpp"

namespace sr
{
    enum class KeyState
    {
        Pressed = 0,
        Released = 1
    };

    struct KeyEventArgs
    {
        KeyCode code;
        unsigned int character;
        KeyState state;
        bool ctrl;
        bool shift;
        bool alt;
        bool super;
    };

    enum class WindowState
    {
        Resized = 0,
        Restored = 1,
        Minimized = 2,
        Maximized = 3
    };

    struct ResizeEventArgs
    {
        int width;
        int height;
        WindowState state;
    };

    struct Event
    {
        enum Type
        {
            None,
            Close,
            KeyPressed,
            KeyReleased,
            Resize,
            EndResize,
        };

        Type type = Event::None;

        union
        {
            KeyEventArgs key;
            ResizeEventArgs resize;
        };
    };
}