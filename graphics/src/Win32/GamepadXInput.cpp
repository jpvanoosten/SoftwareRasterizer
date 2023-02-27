/// <summary>
/// XInput implementation. Based on DirectXTK12.
/// See: https://github.com/microsoft/DirectXTK12
/// </summary>

#include "IncludeWin32.hpp"

#include <Graphics/GamePad.hpp>
#include <Graphics/Timer.hpp>

#include <Xinput.h>

// Link the xinput.lib library.
#pragma comment( lib, "xinput.lib" )

namespace Graphics
{
// Defined in GamePad.cpp
float ApplyLinearDeadZone( float value, float maxValue, float deadZoneSize = 0.0f ) noexcept;
void  ApplyStickDeadZone( float x, float y, DeadZone deadZoneMode, float maxValue, float deadZoneSize, float& resultX,
                          float& resultY ) noexcept;

}  // namespace Graphics

using namespace Graphics;

//const int GamePad::MAX_PLAYERS = XUSER_MAX_COUNT;

// Don't poll disconnected joysticks for N seconds.
static constexpr double POLLING_TIMEOUT = 5.0;
static double           GamePadTimeout[XUSER_MAX_COUNT] {};
// Polling timeouts for each controller.
static Timer GamePadTimer[XUSER_MAX_COUNT];

/// <summary>
/// Check to see if the game pad should be polled.
/// </summary>
/// <param name="player">The game pad slot to check.</param>
/// <returns>true if the game pad state should be polled, or false to wait before polling the game pad again.</returns>
static bool ShouldPoll( int player )
{
    if ( player >= 0 && player < XUSER_MAX_COUNT )
    {
        GamePadTimer[player].tick();
        GamePadTimeout[player] -= GamePadTimer[player].elapsedSeconds();
        return GamePadTimeout[player] < 0.0;
    }

    return false;
}

/// <summary>
/// Reset the polling timeout for the GamePad.
/// </summary>
/// <param name="player">The GamePad slot to reset.</param>
static void ResetTimer( int player ) noexcept
{
    if ( player >= 0 && player < XUSER_MAX_COUNT )
    {
        GamePadTimeout[player] = POLLING_TIMEOUT;
    }
}

GamePadState GamePad::getState( int player, DeadZone deadZoneMode )
{
    GamePadState state {};

    if ( !ShouldPoll( player ) )
        return state;

    XINPUT_STATE xState {};
    if ( XInputGetState( static_cast<DWORD>( player ), &xState ) == ERROR_DEVICE_NOT_CONNECTED )
    {
        ResetTimer( player );
        return state;
    }

    state.connected = true;
    state.packet    = static_cast<uint64_t>( xState.dwPacketNumber );

    const auto buttons          = xState.Gamepad.wButtons;
    state.buttons.a             = ( buttons & XINPUT_GAMEPAD_A ) != 0;
    state.buttons.b             = ( buttons & XINPUT_GAMEPAD_B ) != 0;
    state.buttons.x             = ( buttons & XINPUT_GAMEPAD_X ) != 0;
    state.buttons.y             = ( buttons & XINPUT_GAMEPAD_Y ) != 0;
    state.buttons.leftStick     = ( buttons & XINPUT_GAMEPAD_LEFT_THUMB ) != 0;
    state.buttons.rightStick    = ( buttons & XINPUT_GAMEPAD_RIGHT_THUMB ) != 0;
    state.buttons.leftShoulder  = ( buttons & XINPUT_GAMEPAD_LEFT_SHOULDER ) != 0;
    state.buttons.rightShoulder = ( buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER ) != 0;
    state.buttons.back          = ( buttons & XINPUT_GAMEPAD_BACK ) != 0;
    state.buttons.start         = ( buttons & XINPUT_GAMEPAD_START ) != 0;

    state.dPad.up    = ( buttons & XINPUT_GAMEPAD_DPAD_UP ) != 0;
    state.dPad.down  = ( buttons & XINPUT_GAMEPAD_DPAD_DOWN ) != 0;
    state.dPad.left  = ( buttons & XINPUT_GAMEPAD_DPAD_LEFT ) != 0;
    state.dPad.right = ( buttons & XINPUT_GAMEPAD_DPAD_RIGHT ) != 0;

    const float deadzoneSize = deadZoneMode == DeadZone::None ? 0.0f : XINPUT_GAMEPAD_TRIGGER_THRESHOLD;

    state.triggers.left  = ApplyLinearDeadZone( xState.Gamepad.bLeftTrigger, 255.0f, deadzoneSize );
    state.triggers.right = ApplyLinearDeadZone( xState.Gamepad.bRightTrigger, 255.0f, deadzoneSize );

    ApplyStickDeadZone( xState.Gamepad.sThumbLX, xState.Gamepad.sThumbLY, deadZoneMode, 32767.0f,
                        XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, state.thumbSticks.leftX, state.thumbSticks.leftY );
    ApplyStickDeadZone( xState.Gamepad.sThumbRX, xState.Gamepad.sThumbRY, deadZoneMode, 32767.0f,
                        XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE, state.thumbSticks.rightX, state.thumbSticks.rightY );

    return state;
}

bool GamePad::setVibration( int player, float leftMotor, float rightMotor, float /*leftTrigger*/,
                            float /*rightTrigger*/ )
{
    if ( !ShouldPoll( player ) )
    {
        return false;
    }

    XINPUT_VIBRATION vibration;
    vibration.wLeftMotorSpeed  = static_cast<WORD>( leftMotor * 65535.0f );
    vibration.wRightMotorSpeed = static_cast<WORD>( rightMotor * 65535.0f );

    if ( XInputSetState( player, &vibration ) == ERROR_DEVICE_NOT_CONNECTED )
    {
        ResetTimer( player );
        return false;
    }

    return true;
}