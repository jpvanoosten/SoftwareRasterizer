#pragma once

#include "Config.hpp"
#include "Events.hpp"  // For ButtonState
#include "GamePadState.hpp"

namespace Graphics
{
class SR_API GamePadStateTracker
{
public:
    ButtonState a;
    ButtonState b;
    ButtonState x;
    ButtonState y;

    ButtonState leftStick;
    ButtonState rightStick;

    ButtonState leftShoulder;
    ButtonState rightShoulder;

    union
    {
        ButtonState back;
        ButtonState view;
    };

    union
    {
        ButtonState start;
        ButtonState menu;
    };

    ButtonState dPadUp;
    ButtonState dPadDown;
    ButtonState dPadLeft;
    ButtonState dPadRight;

    ButtonState leftStickUp;
    ButtonState leftStickDown;
    ButtonState leftStickLeft;
    ButtonState leftStickRight;

    ButtonState rightStickUp;
    ButtonState rightStickDown;
    ButtonState rightStickLeft;
    ButtonState rightStickRight;

    ButtonState leftTrigger;
    ButtonState rightTrigger;

    GamePadStateTracker() noexcept;

    bool update( const GamePadState& state ) noexcept;

    void reset() noexcept;

    [[nodiscard]] GamePadState getLastState() const noexcept;

private:
    GamePadState lastState;
};

inline GamePadStateTracker::GamePadStateTracker() noexcept  // NOLINT(cppcoreguidelines-pro-type-member-init)
{
    reset();
}

inline GamePadState GamePadStateTracker::getLastState() const noexcept
{
    return lastState;
}

}  // namespace Graphics
