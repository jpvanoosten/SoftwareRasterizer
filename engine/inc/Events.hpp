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

    struct Event
    {
        enum Type
        {
            None,
            Close,
            KeyPressed,
            KeyReleased,
        };

        Type type = Event::None;

        union
        {
            KeyEventArgs key;
        };

    };
}