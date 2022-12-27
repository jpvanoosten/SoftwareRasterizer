#pragma once

#include "Config.hpp"
#include "KeyboardState.hpp"

namespace sr
{
class SR_API KeyboardStateTracker
{
public:
    KeyboardState released;
    KeyboardState pressed;

    KeyboardStateTracker() noexcept;

    bool update( const KeyboardState& state ) noexcept;

    void reset() noexcept;

    bool isKeyPressed( KeyCode key ) const noexcept;

    bool isKeyReleased( KeyCode key ) const noexcept;

    KeyboardState getLastState() const noexcept;

private:
    KeyboardState lastState;
};
}  // namespace sr