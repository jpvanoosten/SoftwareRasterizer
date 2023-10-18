#include <Graphics/GamePad.hpp>
#include <hash.hpp>

#include <GLFW/glfw3.h>

#include <cassert>

namespace Graphics
{
// Defined in GamePad.cpp
float ApplyLinearDeadZone( float value, float maxValue, float deadZoneSize = 0.0f ) noexcept;
void  ApplyStickDeadZone( float x, float y, DeadZone deadZoneMode, float maxValue, float deadZoneSize, float& resultX,
                          float& resultY ) noexcept;

}  // namespace Graphics

using namespace Graphics;

template<>
struct std::hash<GamePadState::Buttons>
{
    size_t operator()( const GamePadState::Buttons& buttons ) const noexcept
    {
        size_t seed = 0;

        hash_combine( seed, buttons.a );
        hash_combine( seed, buttons.b );
        hash_combine( seed, buttons.x );
        hash_combine( seed, buttons.y );
        hash_combine( seed, buttons.leftStick );
        hash_combine( seed, buttons.rightStick );
        hash_combine( seed, buttons.leftShoulder );
        hash_combine( seed, buttons.rightShoulder );
        hash_combine( seed, buttons.back );
        hash_combine( seed, buttons.start );

        return seed;
    }
};

template<>
struct std::hash<GamePadState::DPad>
{
    size_t operator()( const GamePadState::DPad& dPad ) const noexcept
    {
        size_t seed = 0;

        hash_combine( seed, dPad.up );
        hash_combine( seed, dPad.down );
        hash_combine( seed, dPad.right );
        hash_combine( seed, dPad.left );

        return seed;
    }
};

template<>
struct std::hash<GamePadState::ThumbSticks>
{
    size_t operator()( const GamePadState::ThumbSticks& thumbSticks ) const noexcept
    {
        size_t seed = 0;

        hash_combine( seed, thumbSticks.leftX );
        hash_combine( seed, thumbSticks.leftY );
        hash_combine( seed, thumbSticks.rightX );
        hash_combine( seed, thumbSticks.rightY );

        return seed;
    }
};

template<>
struct std::hash<GamePadState::Triggers>
{
    size_t operator()( const GamePadState::Triggers& triggers ) const noexcept
    {
        size_t seed = 0;

        hash_combine( seed, triggers.left );
        hash_combine( seed, triggers.right );

        return seed;
    }
};

template<>
struct std::hash<GamePadState>
{
    size_t operator()( const GamePadState& state ) const noexcept
    {
        size_t seed = 0;

        hash_combine( seed, state.buttons );
        hash_combine( seed, state.dPad );
        hash_combine( seed, state.thumbSticks );
        hash_combine( seed, state.triggers );

        return seed;
    }
};

GamePadState GamePad::getState( int player, DeadZone deadZoneMode )
{
    static std::hash<GamePadState> hasher {};

    GamePadState state {};

    state.connected = glfwJoystickPresent( player );
    if ( !state.connected )
        return state;

    int        count;
    const auto buttons = glfwGetJoystickButtons( player, &count );
    assert( count >= GLFW_GAMEPAD_BUTTON_LAST );

    state.buttons.a             = buttons[GLFW_GAMEPAD_BUTTON_A] == GLFW_PRESS;
    state.buttons.b             = buttons[GLFW_GAMEPAD_BUTTON_B] == GLFW_PRESS;
    state.buttons.x             = buttons[GLFW_GAMEPAD_BUTTON_X] == GLFW_PRESS;
    state.buttons.y             = buttons[GLFW_GAMEPAD_BUTTON_Y] == GLFW_PRESS;
    state.buttons.leftStick     = buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB] == GLFW_PRESS;
    state.buttons.rightStick    = buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB] == GLFW_PRESS;
    state.buttons.leftShoulder  = buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] == GLFW_PRESS;
    state.buttons.rightShoulder = buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] == GLFW_PRESS;
    state.buttons.back          = buttons[GLFW_GAMEPAD_BUTTON_BACK] == GLFW_PRESS;
    state.buttons.start         = buttons[GLFW_GAMEPAD_BUTTON_START] == GLFW_PRESS;

    state.dPad.up    = buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP] == GLFW_PRESS;
    state.dPad.down  = buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] == GLFW_PRESS;
    state.dPad.right = buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] == GLFW_PRESS;
    state.dPad.left  = buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] == GLFW_PRESS;

    const auto axis = glfwGetJoystickAxes( player, &count );
    assert( count >= GLFW_GAMEPAD_AXIS_LAST );

    const auto lx = axis[GLFW_GAMEPAD_AXIS_LEFT_X];
    const auto ly = axis[GLFW_GAMEPAD_AXIS_LEFT_Y];
    const auto rx = axis[GLFW_GAMEPAD_AXIS_RIGHT_X];
    const auto ry = axis[GLFW_GAMEPAD_AXIS_RIGHT_Y];

    // These deadzone values are based on Xinput's deadzone constants:
    constexpr float leftThumbDeadzone  = 7849.0f / 32767.0f;  // XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
    constexpr float rightThumbDeadzone = 8689.0f / 32767.0f;   // XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
    constexpr float triggerThreshold   = 30.0f / 255.0f;      // XINPUT_GAMEPAD_TRIGGER_THRESHOLD

    ApplyStickDeadZone( lx, ly, deadZoneMode, 1.0f, leftThumbDeadzone, state.thumbSticks.leftX, state.thumbSticks.leftY );
    ApplyStickDeadZone( rx, ry, deadZoneMode, 1.0f, rightThumbDeadzone, state.thumbSticks.rightX, state.thumbSticks.rightY );

    const auto lt = axis[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
    const auto rt = axis[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];

    state.triggers.left  = ApplyLinearDeadZone( lt, 1.0f, deadZoneMode == DeadZone::None ? 0.0f : triggerThreshold );
    state.triggers.right = ApplyLinearDeadZone( rt, 1.0f, deadZoneMode == DeadZone::None ? 0.0f : triggerThreshold );

    // The packet only changes if the state changes.
    state.packet = hasher( state );

    return state;
}

bool GamePad::setVibration( int /*player*/, float /*leftMotor*/, float /*rightMotor*/, float /*leftTrigger*/, float /*rightTrigger*/ )
{
    // GLFW does not support vibration :(
    return false;
}
