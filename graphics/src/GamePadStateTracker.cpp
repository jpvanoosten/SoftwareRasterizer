#include <Graphics/GamePadStateTracker.hpp>

#include <cstring> // For memset.

using namespace Graphics;

#define UPDATE_BUTTON_STATE( field ) static_cast<ButtonState>( ( !!state.buttons.field ) | ( ( !!state.buttons.field ^ !!lastState.buttons.field ) << 1 ) )
#define UPDATE_DPAD_STATE( field )   static_cast<ButtonState>( ( !!state.dPad.field ) | ( ( !!state.dPad.field ^ !!lastState.dPad.field ) << 1 ) )

bool GamePadStateTracker::update( const GamePadState& state ) noexcept
{
    a = UPDATE_BUTTON_STATE( a );
    b = UPDATE_BUTTON_STATE( b );
    x = UPDATE_BUTTON_STATE( x );
    y = UPDATE_BUTTON_STATE( y );

    leftStick  = UPDATE_BUTTON_STATE( leftStick );
    rightStick = UPDATE_BUTTON_STATE( rightStick );

    leftShoulder  = UPDATE_BUTTON_STATE( leftShoulder );
    rightShoulder = UPDATE_BUTTON_STATE( rightShoulder );

    back  = UPDATE_BUTTON_STATE( back );
    start = UPDATE_BUTTON_STATE( start );

    dPadUp    = UPDATE_DPAD_STATE( up );
    dPadDown  = UPDATE_DPAD_STATE( down );
    dPadLeft  = UPDATE_DPAD_STATE( left );
    dPadRight = UPDATE_DPAD_STATE( right );

    // Handle 'threshold' tests which emulate buttons
    // TODO: Make a macro for these...
    // clang-format off
    bool threshold = state.isLeftThumbStickUp();
    leftStickUp    = static_cast<ButtonState>( ( !!threshold ) | ( ( !!threshold ^ !!lastState.isLeftThumbStickUp() ) << 1 ) );

    threshold     = state.isLeftThumbStickDown();
    leftStickDown = static_cast<ButtonState>( ( !!threshold ) | ( ( !!threshold ^ !!lastState.isLeftThumbStickDown() ) << 1 ) );

    threshold     = state.isLeftThumbStickLeft();
    leftStickLeft = static_cast<ButtonState>( ( !!threshold ) | ( ( !!threshold ^ !!lastState.isLeftThumbStickLeft() ) << 1 ) );

    threshold      = state.isLeftThumbStickRight();
    leftStickRight = static_cast<ButtonState>( ( !!threshold ) | ( ( !!threshold ^ !!lastState.isLeftThumbStickRight() ) << 1 ) );

    threshold    = state.isRightThumbStickUp();
    rightStickUp = static_cast<ButtonState>( ( !!threshold ) | ( ( !!threshold ^ !!lastState.isRightThumbStickUp() ) << 1 ) );

    threshold      = state.isRightThumbStickDown();
    rightStickDown = static_cast<ButtonState>( ( !!threshold ) | ( ( !!threshold ^ !!lastState.isRightThumbStickDown() ) << 1 ) );

    threshold      = state.isRightThumbStickLeft();
    rightStickLeft = static_cast<ButtonState>( ( !!threshold ) | ( ( !!threshold ^ !!lastState.isRightThumbStickLeft() ) << 1 ) );

    threshold       = state.isRightThumbStickRight();
    rightStickRight = static_cast<ButtonState>( ( !!threshold ) | ( ( !!threshold ^ !!lastState.isRightThumbStickRight() ) << 1 ) );

    threshold   = state.isLeftTriggerPressed();
    leftTrigger = static_cast<ButtonState>( ( !!threshold ) | ( ( !!threshold ^ !!lastState.isLeftTriggerPressed() ) << 1 ) );

    threshold    = state.isRightTriggerPressed();
    rightTrigger = static_cast<ButtonState>( ( !!threshold ) | ( ( !!threshold ^ !!lastState.isRightTriggerPressed() ) << 1 ) );
    // clang-format on

    if ( lastState != state )
    {
        lastState = state;
        return true;
    }

    return false;
}

void GamePadStateTracker::reset() noexcept
{
    memset( this, 0, sizeof( GamePadStateTracker ) );
}
