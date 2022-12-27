#include <Input.hpp>

#include <GamePad.hpp>
#include <Keyboard.hpp>
#include <Mouse.hpp>

#include <algorithm>
#include <map>
#include <string>

using namespace sr;

static GamePadStateTracker  g_GamePadStateTrackers[GamePad::MAX_PLAYERS];
static KeyboardStateTracker g_KeyboardStateTracker;
static MouseStateTracker    g_MouseStateTracker;

static std::map<std::string, AxisCallback> g_AxisMap = {
    { "Horizontal", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
         float leftX  = 0.0f;
         float rightX = 0.0f;

         for ( auto& gamePadState: gamePadStates )
         {
             const auto state = gamePadState.getLastState();

             leftX += state.thumbSticks.leftX;
             rightX += state.thumbSticks.rightX;
         }

        const auto keyState = keyboardState.getLastState();

         const float a     = keyState.A ? 1.0f : 0.0f;
         const float d     = keyState.D ? 1.0f : 0.0f;
         const float left  = keyState.Left ? 1.0f : 0.0f;
         const float right = keyState.Right ? 1.0f : 0.0f;

        const float x = static_cast<float>( mouseState.x );

         return std::clamp( leftX + rightX - a + d - left + right + x, -1.0f, 1.0f );
     } },
    { "Vertical", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
         float leftY  = 0.0f;
         float rightY = 0.0f;

         for ( auto& gamePadState: gamePadStates )
         {
             const auto state = gamePadState.getLastState();

             leftY += state.thumbSticks.leftY;
             rightY += state.thumbSticks.rightY;
         }

        const auto keyState = keyboardState.getLastState();

         const float s    = keyState.S ? 1.0f : 0.0f;
         const float w    = keyState.W ? 1.0f : 0.0f;
         const float up   = keyState.Up ? 1.0f : 0.0f;
         const float down = keyState.Down ? 1.0f : 0.0f;

        const float y = static_cast<float>( mouseState.y );

         return std::clamp( leftY + rightY - s + w - down + up + y, -1.0f, 1.0f );
     } },
};

static std::map<std::string, ButtonCallback> g_ButtonMap;

void Input::update()
{
    for ( int i = 0; i < GamePad::MAX_PLAYERS; ++i )
        g_GamePadStateTrackers[i].update( GamePad::getState( i ) );

    g_KeyboardStateTracker.update( Keyboard::getState() );
    g_MouseStateTracker.update( Mouse::getState() );
}

float Input::getAxis( std::string_view axisName )
{
    if ( const auto& iter = g_AxisMap.find( std::string( axisName ) ); iter != g_AxisMap.end() )
    {
        return iter->second( g_GamePadStateTrackers, g_KeyboardStateTracker, g_MouseStateTracker );
    }

    return 0.0f;
}
