#include <Graphics/Input.hpp>
#include <Graphics/GamePad.hpp>
#include <Graphics/Keyboard.hpp>
#include <Graphics/Mouse.hpp>

#include <algorithm>
#include <map>
#include <string>

using namespace Graphics;

static GamePadStateTracker  g_GamePadStateTrackers[GamePad::MAX_PLAYERS];
static KeyboardStateTracker g_KeyboardStateTracker;
static MouseStateTracker    g_MouseStateTracker;

static std::map<std::string, KeyCode> g_KeyMap = {
    { "a", KeyCode::A },
    { "b", KeyCode::B },
    { "c", KeyCode::C },
    { "d", KeyCode::D },
    { "e", KeyCode::E },
    { "f", KeyCode::F },
    { "g", KeyCode::G },
    { "h", KeyCode::H },
    { "i", KeyCode::I },
    { "j", KeyCode::J },
    { "k", KeyCode::K },
    { "l", KeyCode::L },
    { "m", KeyCode::M },
    { "n", KeyCode::N },
    { "o", KeyCode::O },
    { "p", KeyCode::P },
    { "q", KeyCode::Q },
    { "r", KeyCode::R },
    { "s", KeyCode::S },
    { "t", KeyCode::T },
    { "u", KeyCode::U },
    { "v", KeyCode::V },
    { "w", KeyCode::W },
    { "x", KeyCode::X },
    { "y", KeyCode::Y },
    { "z", KeyCode::Z },
    { "1", KeyCode::D1 },
    { "2", KeyCode::D2 },
    { "3", KeyCode::D3 },
    { "4", KeyCode::D4 },
    { "5", KeyCode::D5 },
    { "6", KeyCode::D6 },
    { "7", KeyCode::D7 },
    { "8", KeyCode::D8 },
    { "9", KeyCode::D9 },
    { "0", KeyCode::D0 },
    { "up", KeyCode::Up },
    { "down", KeyCode::Down },
    { "left", KeyCode::Left },
    { "right", KeyCode::Right },
    { "[1]", KeyCode::NumPad1 },
    { "[2]", KeyCode::NumPad2 },
    { "[3]", KeyCode::NumPad3 },
    { "[4]", KeyCode::NumPad4 },
    { "[5]", KeyCode::NumPad5 },
    { "[6]", KeyCode::NumPad6 },
    { "[7]", KeyCode::NumPad7 },
    { "[8]", KeyCode::NumPad8 },
    { "[9]", KeyCode::NumPad9 },
    { "[0]", KeyCode::NumPad0 },
    { "[+]", KeyCode::Add },
    { "[-]", KeyCode::Subtract },
    { "[*]", KeyCode::Multiply },
    { "[=]", KeyCode::Separator },  // TODO: Which is the [=] key on the numpad?
    { "[/]", KeyCode::Divide },
    { "shift", KeyCode::ShiftKey },
    { "left shift", KeyCode::LeftShift },
    { "right shift", KeyCode::RightShift },
    { "ctrl", KeyCode::ControlKey },
    { "left ctrl", KeyCode::LeftControl },
    { "right ctrl", KeyCode::RightControl },
    { "alt", KeyCode::AltKey },
    { "left alt", KeyCode::LeftAlt },
    { "right alt", KeyCode::RightAlt },
    { "left win", KeyCode::LeftWindows },
    { "right win", KeyCode::RightWindows },
    { "backspace", KeyCode::Back },
    { "tab", KeyCode::Tab },
    { "enter", KeyCode::Enter },
    { "return", KeyCode::Enter },
    { "esc", KeyCode::Escape },
    { "escape", KeyCode::Escape },
    { "space", KeyCode::Space },
    { "delete", KeyCode::Delete },
    { "ins", KeyCode::Insert },
    { "insert", KeyCode::Insert },
    { "home", KeyCode::Home },
    { "end", KeyCode::End },
    { "pgup", KeyCode::PageUp },
    { "page up", KeyCode::PageUp },
    { "pgdn", KeyCode::PageDown },
    { "page down", KeyCode::PageDown },
    { "F1", KeyCode::F1 },
    { "F2", KeyCode::F2 },
    { "F3", KeyCode::F3 },
    { "F4", KeyCode::F4 },
    { "F5", KeyCode::F5 },
    { "F6", KeyCode::F6 },
    { "F7", KeyCode::F7 },
    { "F8", KeyCode::F8 },
    { "F9", KeyCode::F9 },
    { "F10", KeyCode::F10 },
    { "F11", KeyCode::F11 },
    { "F12", KeyCode::F12 },
};

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
    { "Fire1", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
         float rightTrigger = 0.0f;

         for ( auto& gamePadState: gamePadStates )
         {
             const auto state = gamePadState.getLastState();
             rightTrigger += state.triggers.right;
         }

         const float ctrl            = keyboardState.getLastState().LeftControl ? 1.0f : 0.0f;
         const float leftMouseButton = mouseState.getLastState().leftButton ? 1.0f : 0.0f;

         return std::clamp( rightTrigger + ctrl + leftMouseButton, 0.0f, 1.0f );
     } },
    { "Fire2", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
         float leftTrigger = 0.0f;

         for ( auto& gamePadState: gamePadStates )
         {
             const auto state = gamePadState.getLastState();
             leftTrigger += state.triggers.left;
         }

         const float alt              = keyboardState.getLastState().LeftAlt ? 1.0f : 0.0f;
         const float rightMouseButton = mouseState.getLastState().rightButton ? 1.0f : 0.0f;

         return std::clamp( leftTrigger + alt + rightMouseButton, 0.0f, 1.0f );
     } },
    { "Fire3", []( std::span<const GamePadStateTracker>, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
         const float shift             = keyboardState.getLastState().LeftShift ? 1.0f : 0.0f;
         const float middleMouseButton = mouseState.getLastState().middleButton ? 1.0f : 0.0f;

         return std::clamp( shift + middleMouseButton, 0.0f, 1.0f );
     } },
    { "Jump", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
         float a = 0.0f;

         for ( auto& gamePadState: gamePadStates )
         {
             const auto state = gamePadState.getLastState();
             a += state.buttons.a ? 1.0f : 0.0f;
         }

         const float space = keyboardState.getLastState().Space ? 1.0f : 0.0f;

         return std::clamp( a + space, 0.0f, 1.0f );
     } },
    { "Mouse X", []( std::span<const GamePadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker& mouseState ) {
         return static_cast<float>( mouseState.x );
     } },
    { "Mouse Y", []( std::span<const GamePadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker& mouseState ) {
         return static_cast<float>( mouseState.y );
     } },
    { "Mouse ScrollWheel", []( std::span<const GamePadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker& mouseState ) {
         return static_cast<float>( mouseState.vScrollWheel );
     } },
    { "Submit", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& ) {
         float a = 0.0f;

         for ( auto& gamePadState: gamePadStates )
         {
             const auto state = gamePadState.getLastState();
             a += state.buttons.a ? 1.0f : 0.0f;
         }

         auto        keyState = keyboardState.getLastState();
         const float enter    = keyState.Enter ? 1.0f : 0.0f;
         const float space    = keyState.Space ? 1.0f : 0.0f;

         return std::clamp( a + enter + space, 0.0f, 1.0f );
     } },
    { "Cancel", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& ) {
         float b = 0.0f;

         for ( auto& gamePadState: gamePadStates )
         {
             const auto state = gamePadState.getLastState();
             b += state.buttons.b ? 1.0f : 0.0f;
         }

         auto        keyState = keyboardState.getLastState();
         const float esc      = keyState.Escape ? 1.0f : 0.0f;

         return std::clamp( b + esc, 0.0f, 1.0f );
     } },
};

static std::map<std::string, ButtonCallback> g_ButtonMap = {
    { "win", []( std::span<const GamePadStateTracker>, const KeyboardStateTracker& keyboardState, const MouseStateTracker& ) {
         return keyboardState.getLastState().LeftWindows || keyboardState.getLastState().RightWindows;
     } },
    { "mouse 0", []( std::span<const GamePadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker& mouseState ) {
         return mouseState.getLastState().leftButton;
     } },
    { "mouse 1", []( std::span<const GamePadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker& mouseState ) {
         return mouseState.getLastState().rightButton;
     } },
    { "mouse 2", []( std::span<const GamePadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker& mouseState ) {
         return mouseState.getLastState().middleButton;
     } },
    { "mouse x1", []( std::span<const GamePadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker& mouseState ) {
         return mouseState.getLastState().xButton1;
     } },
    { "mouse x2", []( std::span<const GamePadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker& mouseState ) {
         return mouseState.getLastState().xButton2;
     } },
    { "joystick button 1", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             const auto state = gamePadState.getLastState();
             button |= state.buttons.a;
         }

         return button;
     } },
    { "joystick button 2", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             const auto state = gamePadState.getLastState();
             button |= state.buttons.b;
         }

         return button;
     } },
    { "joystick button 3", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             const auto state = gamePadState.getLastState();
             button |= state.buttons.x;
         }

         return button;
     } },
    { "joystick button 4", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             const auto state = gamePadState.getLastState();
             button |= state.buttons.y;
         }

         return button;
     } },
    { "joystick button 5", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             const auto state = gamePadState.getLastState();
             button |= state.buttons.leftShoulder;
         }

         return button;
     } },
    { "joystick button 6", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             const auto state = gamePadState.getLastState();
             button |= state.buttons.rightShoulder;
         }

         return button;
     } },
    { "joystick button 7", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             const auto state = gamePadState.getLastState();
             button |= state.buttons.view;
         }

         return button;
     } },
    { "joystick button 8", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             const auto state = gamePadState.getLastState();
             button |= state.buttons.menu;
         }

         return button;
     } },
    { "joystick button 9", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             const auto state = gamePadState.getLastState();
             button |= state.buttons.leftStick;
         }

         return button;
     } },
    { "joystick button 10", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             const auto state = gamePadState.getLastState();
             button |= state.buttons.rightStick;
         }

         return button;
     } },
    { "joystick dpad up", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             const auto state = gamePadState.getLastState();
             button |= state.dPad.up;
         }

         return button;
     } },
    { "joystick dpad down", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             const auto state = gamePadState.getLastState();
             button |= state.dPad.down;
         }

         return button;
     } },
    { "joystick dpad left", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             const auto state = gamePadState.getLastState();
             button |= state.dPad.left;
         }

         return button;
     } },
    { "joystick dpad right", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             const auto state = gamePadState.getLastState();
             button |= state.dPad.right;
         }

         return button;
     } },
    { "joystick 1 button 1", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].getLastState().buttons.a;
     } },
    { "joystick 1 button 2", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].getLastState().buttons.b;
     } },
    { "joystick 1 button 3", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].getLastState().buttons.x;
     } },
    { "joystick 1 button 4", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].getLastState().buttons.y;
     } },
    { "joystick 1 button 5", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].getLastState().buttons.leftShoulder;
     } },
    { "joystick 1 button 6", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].getLastState().buttons.rightShoulder;
     } },
    { "joystick 1 button 7", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].getLastState().buttons.view;
     } },
    { "joystick 1 button 8", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].getLastState().buttons.menu;
     } },
    { "joystick 1 button 9", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].getLastState().buttons.leftStick;
     } },
    { "joystick 1 button 10", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].getLastState().buttons.rightStick;
     } },
    { "joystick 1 dpad up", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].getLastState().dPad.up;
     } },
    { "joystick 1 dpad down", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].getLastState().dPad.down;
     } },
    { "joystick 1 dpad left", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].getLastState().dPad.left;
     } },
    { "joystick 1 dpad right", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].getLastState().dPad.right;
     } },
    { "joystick 2 button 1", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].getLastState().buttons.a;
     } },
    { "joystick 2 button 2", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].getLastState().buttons.b;
     } },
    { "joystick 2 button 3", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].getLastState().buttons.x;
     } },
    { "joystick 2 button 4", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].getLastState().buttons.y;
     } },
    { "joystick 2 button 5", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].getLastState().buttons.leftShoulder;
     } },
    { "joystick 2 button 6", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].getLastState().buttons.rightShoulder;
     } },
    { "joystick 2 button 7", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].getLastState().buttons.view;
     } },
    { "joystick 2 button 8", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].getLastState().buttons.menu;
     } },
    { "joystick 2 button 9", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].getLastState().buttons.leftStick;
     } },
    { "joystick 2 button 10", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].getLastState().buttons.rightStick;
     } },
    { "joystick 2 dpad up", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].getLastState().dPad.up;
     } },
    { "joystick 2 dpad down", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].getLastState().dPad.down;
     } },
    { "joystick 2 dpad left", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].getLastState().dPad.left;
     } },
    { "joystick 2 dpad right", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].getLastState().dPad.right;
     } },
    { "joystick 3 button 1", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].getLastState().buttons.a;
     } },
    { "joystick 3 button 2", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].getLastState().buttons.b;
     } },
    { "joystick 3 button 3", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].getLastState().buttons.x;
     } },
    { "joystick 3 button 4", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].getLastState().buttons.y;
     } },
    { "joystick 3 button 5", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].getLastState().buttons.leftShoulder;
     } },
    { "joystick 3 button 6", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].getLastState().buttons.rightShoulder;
     } },
    { "joystick 3 button 7", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].getLastState().buttons.view;
     } },
    { "joystick 3 button 8", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].getLastState().buttons.menu;
     } },
    { "joystick 3 button 9", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].getLastState().buttons.leftStick;
     } },
    { "joystick 3 button 10", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].getLastState().buttons.rightStick;
     } },
    { "joystick 3 dpad up", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].getLastState().dPad.up;
     } },
    { "joystick 3 dpad down", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].getLastState().dPad.down;
     } },
    { "joystick 3 dpad left", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].getLastState().dPad.left;
     } },
    { "joystick 3 dpad right", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].getLastState().dPad.right;
     } },
    { "joystick 4 button 1", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].getLastState().buttons.a;
     } },
    { "joystick 4 button 2", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].getLastState().buttons.b;
     } },
    { "joystick 4 button 3", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].getLastState().buttons.x;
     } },
    { "joystick 4 button 4", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].getLastState().buttons.y;
     } },
    { "joystick 4 button 5", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].getLastState().buttons.leftShoulder;
     } },
    { "joystick 4 button 6", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].getLastState().buttons.rightShoulder;
     } },
    { "joystick 4 button 7", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].getLastState().buttons.view;
     } },
    { "joystick 4 button 8", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].getLastState().buttons.menu;
     } },
    { "joystick 4 button 9", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].getLastState().buttons.leftStick;
     } },
    { "joystick 4 button 10", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].getLastState().buttons.rightStick;
     } },
    { "joystick 4 dpad up", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].getLastState().dPad.up;
     } },
    { "joystick 4 dpad down", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].getLastState().dPad.down;
     } },
    { "joystick 4 dpad left", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].getLastState().dPad.left;
     } },
    { "joystick 4 dpad right", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].getLastState().dPad.right;
     } },
};

static std::map<std::string, ButtonCallback> g_ButtonDownMap = {
    { "win", []( std::span<const GamePadStateTracker>, const KeyboardStateTracker& keyboardState, const MouseStateTracker& ) {
         return keyboardState.isKeyPressed( KeyCode::LeftWindows ) || keyboardState.isKeyPressed( KeyCode::RightWindows );
     } },
    { "mouse 0", []( std::span<const GamePadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker& mouseState ) {
         return mouseState.leftButton == ButtonState::Pressed;
     } },
    { "mouse 1", []( std::span<const GamePadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker& mouseState ) {
         return mouseState.rightButton == ButtonState::Pressed;
     } },
    { "mouse 2", []( std::span<const GamePadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker& mouseState ) {
         return mouseState.middleButton == ButtonState::Pressed;
     } },
    { "mouse x1", []( std::span<const GamePadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker& mouseState ) {
         return mouseState.xButton1 == ButtonState::Pressed;
     } },
    { "mouse x2", []( std::span<const GamePadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker& mouseState ) {
         return mouseState.xButton2 == ButtonState::Pressed;
     } },
    { "joystick button 1", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.a == ButtonState::Pressed;
         }

         return button;
     } },
    { "joystick button 2", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.b == ButtonState::Pressed;
         }

         return button;
     } },
    { "joystick button 3", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.x == ButtonState::Pressed;
         }

         return button;
     } },
    { "joystick button 4", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.y == ButtonState::Pressed;
         }

         return button;
     } },
    { "joystick button 5", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.leftShoulder == ButtonState::Pressed;
         }

         return button;
     } },
    { "joystick button 6", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.rightShoulder == ButtonState::Pressed;
         }

         return button;
     } },
    { "joystick button 7", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.view == ButtonState::Pressed;
         }

         return button;
     } },
    { "joystick button 8", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.menu == ButtonState::Pressed;
         }

         return button;
     } },
    { "joystick button 9", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.leftStick == ButtonState::Pressed;
         }

         return button;
     } },
    { "joystick button 10", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.rightStick == ButtonState::Pressed;
         }

         return button;
     } },
    { "joystick dpad up", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.dPadUp == ButtonState::Pressed;
         }

         return button;
     } },
    { "joystick dpad down", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.dPadDown == ButtonState::Pressed;
         }

         return button;
     } },
    { "joystick dpad left", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.dPadLeft == ButtonState::Pressed;
         }

         return button;
     } },
    { "joystick dpad right", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.dPadRight == ButtonState::Pressed;
         }

         return button;
     } },
    { "joystick 1 button 1", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].a == ButtonState::Pressed;
     } },
    { "joystick 1 button 2", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].b == ButtonState::Pressed;
     } },
    { "joystick 1 button 3", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].x == ButtonState::Pressed;
     } },
    { "joystick 1 button 4", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].y == ButtonState::Pressed;
     } },
    { "joystick 1 button 5", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].leftShoulder == ButtonState::Pressed;
     } },
    { "joystick 1 button 6", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].rightShoulder == ButtonState::Pressed;
     } },
    { "joystick 1 button 7", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].view == ButtonState::Pressed;
     } },
    { "joystick 1 button 8", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].menu == ButtonState::Pressed;
     } },
    { "joystick 1 button 9", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].leftStick == ButtonState::Pressed;
     } },
    { "joystick 1 button 10", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].rightStick == ButtonState::Pressed;
     } },
    { "joystick 1 dpad up", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].dPadUp == ButtonState::Pressed;
     } },
    { "joystick 1 dpad down", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].dPadDown == ButtonState::Pressed;
     } },
    { "joystick 1 dpad left", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].dPadLeft == ButtonState::Pressed;
     } },
    { "joystick 1 dpad right", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].dPadRight == ButtonState::Pressed;
     } },
    { "joystick 2 button 1", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].a == ButtonState::Pressed;
     } },
    { "joystick 2 button 2", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].b == ButtonState::Pressed;
     } },
    { "joystick 2 button 3", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].x == ButtonState::Pressed;
     } },
    { "joystick 2 button 4", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].y == ButtonState::Pressed;
     } },
    { "joystick 2 button 5", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].leftShoulder == ButtonState::Pressed;
     } },
    { "joystick 2 button 6", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].rightShoulder == ButtonState::Pressed;
     } },
    { "joystick 2 button 7", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].view == ButtonState::Pressed;
     } },
    { "joystick 2 button 8", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].menu == ButtonState::Pressed;
     } },
    { "joystick 2 button 9", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].leftStick == ButtonState::Pressed;
     } },
    { "joystick 2 button 10", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].rightStick == ButtonState::Pressed;
     } },
    { "joystick 2 dpad up", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].dPadUp == ButtonState::Pressed;
     } },
    { "joystick 2 dpad down", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].dPadDown == ButtonState::Pressed;
     } },
    { "joystick 2 dpad left", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].dPadLeft == ButtonState::Pressed;
     } },
    { "joystick 2 dpad right", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].dPadRight == ButtonState::Pressed;
     } },
    { "joystick 3 button 1", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].a == ButtonState::Pressed;
     } },
    { "joystick 3 button 2", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].b == ButtonState::Pressed;
     } },
    { "joystick 3 button 3", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].x == ButtonState::Pressed;
     } },
    { "joystick 3 button 4", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].y == ButtonState::Pressed;
     } },
    { "joystick 3 button 5", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].leftShoulder == ButtonState::Pressed;
     } },
    { "joystick 3 button 6", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].rightShoulder == ButtonState::Pressed;
     } },
    { "joystick 3 button 7", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].view == ButtonState::Pressed;
     } },
    { "joystick 3 button 8", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].menu == ButtonState::Pressed;
     } },
    { "joystick 3 button 9", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].leftStick == ButtonState::Pressed;
     } },
    { "joystick 3 button 10", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].rightStick == ButtonState::Pressed;
     } },
    { "joystick 3 dpad up", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].dPadUp == ButtonState::Pressed;
     } },
    { "joystick 3 dpad down", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].dPadDown == ButtonState::Pressed;
     } },
    { "joystick 3 dpad left", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].dPadLeft == ButtonState::Pressed;
     } },
    { "joystick 3 dpad right", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].dPadRight == ButtonState::Pressed;
     } },
    { "joystick 4 button 1", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].a == ButtonState::Pressed;
     } },
    { "joystick 4 button 2", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].b == ButtonState::Pressed;
     } },
    { "joystick 4 button 3", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].x == ButtonState::Pressed;
     } },
    { "joystick 4 button 4", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].y == ButtonState::Pressed;
     } },
    { "joystick 4 button 5", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].leftShoulder == ButtonState::Pressed;
     } },
    { "joystick 4 button 6", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].rightShoulder == ButtonState::Pressed;
     } },
    { "joystick 4 button 7", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].view == ButtonState::Pressed;
     } },
    { "joystick 4 button 8", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].menu == ButtonState::Pressed;
     } },
    { "joystick 4 button 9", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].leftStick == ButtonState::Pressed;
     } },
    { "joystick 4 button 10", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].rightStick == ButtonState::Pressed;
     } },
    { "joystick 4 dpad up", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].dPadUp == ButtonState::Pressed;
     } },
    { "joystick 4 dpad down", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].dPadDown == ButtonState::Pressed;
     } },
    { "joystick 4 dpad left", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].dPadLeft == ButtonState::Pressed;
     } },
    { "joystick 4 dpad right", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].dPadRight == ButtonState::Pressed;
     } },
};

static std::map<std::string, ButtonCallback> g_ButtonUpMap = {
    { "win", []( std::span<const GamePadStateTracker>, const KeyboardStateTracker& keyboardState, const MouseStateTracker& ) {
         return keyboardState.isKeyReleased( KeyCode::LeftWindows ) || keyboardState.isKeyReleased( KeyCode::RightWindows );
     } },
    { "mouse 0", []( std::span<const GamePadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker& mouseState ) {
         return mouseState.leftButton == ButtonState::Released;
     } },
    { "mouse 1", []( std::span<const GamePadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker& mouseState ) {
         return mouseState.rightButton == ButtonState::Released;
     } },
    { "mouse 2", []( std::span<const GamePadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker& mouseState ) {
         return mouseState.middleButton == ButtonState::Released;
     } },
    { "mouse x1", []( std::span<const GamePadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker& mouseState ) {
         return mouseState.xButton1 == ButtonState::Released;
     } },
    { "mouse x2", []( std::span<const GamePadStateTracker>, const KeyboardStateTracker&, const MouseStateTracker& mouseState ) {
         return mouseState.xButton2 == ButtonState::Released;
     } },
    { "joystick button 1", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.a == ButtonState::Released;
         }

         return button;
     } },
    { "joystick button 2", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.b == ButtonState::Released;
         }

         return button;
     } },
    { "joystick button 3", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.x == ButtonState::Released;
         }

         return button;
     } },
    { "joystick button 4", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.y == ButtonState::Released;
         }

         return button;
     } },
    { "joystick button 5", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.leftShoulder == ButtonState::Released;
         }

         return button;
     } },
    { "joystick button 6", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.rightShoulder == ButtonState::Released;
         }

         return button;
     } },
    { "joystick button 7", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.view == ButtonState::Released;
         }

         return button;
     } },
    { "joystick button 8", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.menu == ButtonState::Released;
         }

         return button;
     } },
    { "joystick button 9", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.leftStick == ButtonState::Released;
         }

         return button;
     } },
    { "joystick button 10", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.rightStick == ButtonState::Released;
         }

         return button;
     } },
    { "joystick dpad up", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.dPadUp == ButtonState::Released;
         }

         return button;
     } },
    { "joystick dpad down", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.dPadDown == ButtonState::Released;
         }

         return button;
     } },
    { "joystick dpad left", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.dPadLeft == ButtonState::Released;
         }

         return button;
     } },
    { "joystick dpad right", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         bool button = false;

         for ( auto& gamePadState: gamePadStates )
         {
             button |= gamePadState.dPadRight == ButtonState::Released;
         }

         return button;
     } },
    { "joystick 1 button 1", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].a == ButtonState::Released;
     } },
    { "joystick 1 button 2", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].b == ButtonState::Released;
     } },
    { "joystick 1 button 3", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].x == ButtonState::Released;
     } },
    { "joystick 1 button 4", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].y == ButtonState::Released;
     } },
    { "joystick 1 button 5", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].leftShoulder == ButtonState::Released;
     } },
    { "joystick 1 button 6", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].rightShoulder == ButtonState::Released;
     } },
    { "joystick 1 button 7", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].view == ButtonState::Released;
     } },
    { "joystick 1 button 8", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].menu == ButtonState::Released;
     } },
    { "joystick 1 button 9", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].leftStick == ButtonState::Released;
     } },
    { "joystick 1 button 10", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].rightStick == ButtonState::Released;
     } },
    { "joystick 1 dpad up", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].dPadUp == ButtonState::Released;
     } },
    { "joystick 1 dpad down", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].dPadDown == ButtonState::Released;
     } },
    { "joystick 1 dpad left", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].dPadLeft == ButtonState::Released;
     } },
    { "joystick 1 dpad right", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[0].dPadRight == ButtonState::Released;
     } },
    { "joystick 2 button 1", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].a == ButtonState::Released;
     } },
    { "joystick 2 button 2", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].b == ButtonState::Released;
     } },
    { "joystick 2 button 3", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].x == ButtonState::Released;
     } },
    { "joystick 2 button 4", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].y == ButtonState::Released;
     } },
    { "joystick 2 button 5", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].leftShoulder == ButtonState::Released;
     } },
    { "joystick 2 button 6", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].rightShoulder == ButtonState::Released;
     } },
    { "joystick 2 button 7", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].view == ButtonState::Released;
     } },
    { "joystick 2 button 8", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].menu == ButtonState::Released;
     } },
    { "joystick 2 button 9", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].leftStick == ButtonState::Released;
     } },
    { "joystick 2 button 10", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].rightStick == ButtonState::Released;
     } },
    { "joystick 2 dpad up", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].dPadUp == ButtonState::Released;
     } },
    { "joystick 2 dpad down", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].dPadDown == ButtonState::Released;
     } },
    { "joystick 2 dpad left", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].dPadLeft == ButtonState::Released;
     } },
    { "joystick 2 dpad right", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[1].dPadRight == ButtonState::Released;
     } },
    { "joystick 3 button 1", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].a == ButtonState::Released;
     } },
    { "joystick 3 button 2", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].b == ButtonState::Released;
     } },
    { "joystick 3 button 3", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].x == ButtonState::Released;
     } },
    { "joystick 3 button 4", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].y == ButtonState::Released;
     } },
    { "joystick 3 button 5", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].leftShoulder == ButtonState::Released;
     } },
    { "joystick 3 button 6", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].rightShoulder == ButtonState::Released;
     } },
    { "joystick 3 button 7", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].view == ButtonState::Released;
     } },
    { "joystick 3 button 8", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].menu == ButtonState::Released;
     } },
    { "joystick 3 button 9", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].leftStick == ButtonState::Released;
     } },
    { "joystick 3 button 10", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].rightStick == ButtonState::Released;
     } },
    { "joystick 3 dpad up", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].dPadUp == ButtonState::Released;
     } },
    { "joystick 3 dpad down", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].dPadDown == ButtonState::Released;
     } },
    { "joystick 3 dpad left", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].dPadLeft == ButtonState::Released;
     } },
    { "joystick 3 dpad right", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[2].dPadRight == ButtonState::Released;
     } },
    { "joystick 4 button 1", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].a == ButtonState::Released;
     } },
    { "joystick 4 button 2", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].b == ButtonState::Released;
     } },
    { "joystick 4 button 3", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].x == ButtonState::Released;
     } },
    { "joystick 4 button 4", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].y == ButtonState::Released;
     } },
    { "joystick 4 button 5", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].leftShoulder == ButtonState::Released;
     } },
    { "joystick 4 button 6", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].rightShoulder == ButtonState::Released;
     } },
    { "joystick 4 button 7", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].view == ButtonState::Released;
     } },
    { "joystick 4 button 8", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].menu == ButtonState::Released;
     } },
    { "joystick 4 button 9", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].leftStick == ButtonState::Released;
     } },
    { "joystick 4 button 10", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].rightStick == ButtonState::Released;
     } },
    { "joystick 4 dpad up", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].dPadUp == ButtonState::Released;
     } },
    { "joystick 4 dpad down", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].dPadDown == ButtonState::Released;
     } },
    { "joystick 4 dpad left", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].dPadLeft == ButtonState::Released;
     } },
    { "joystick 4 dpad right", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker&, const MouseStateTracker& ) {
         return gamePadStates[3].dPadRight == ButtonState::Released;
     } },
};

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

bool Input::getButton( std::string_view buttonName )
{
    // First check if there is a matching axis mapping.
    if ( const auto& iter = g_AxisMap.find( std::string( buttonName ) ); iter != g_AxisMap.end() )
    {
        return iter->second( g_GamePadStateTrackers, g_KeyboardStateTracker, g_MouseStateTracker ) > 0.0f;
    }

    // Then check if there is a matching button mapping.
    if ( const auto& iter = g_ButtonMap.find( std::string( buttonName ) ); iter != g_ButtonMap.end() )
    {
        return iter->second( g_GamePadStateTrackers, g_KeyboardStateTracker, g_MouseStateTracker );
    }

    return false;
}

bool Input::getButtonDown( std::string_view buttonName )
{
    // First check to see if there is a matching button in the key map:
    if ( const auto& iter = g_KeyMap.find( std::string( buttonName ) ); iter != g_KeyMap.end() )
    {
        return g_KeyboardStateTracker.isKeyPressed( iter->second );
    }

    // Otherwise, check the button down map
    if ( const auto& iter = g_ButtonDownMap.find( std::string( buttonName ) ); iter != g_ButtonDownMap.end() )
    {
        return iter->second( g_GamePadStateTrackers, g_KeyboardStateTracker, g_MouseStateTracker );
    }

    return false;
}

bool Input::getButtonUp( std::string_view buttonName )
{
    // First check to see if there is a matching button in the key map:
    if ( const auto& iter = g_KeyMap.find( std::string( buttonName ) ); iter != g_KeyMap.end() )
    {
        return g_KeyboardStateTracker.isKeyReleased( iter->second );
    }

    // Otherwise, check the button up map
    if ( const auto& iter = g_ButtonUpMap.find( std::string( buttonName ) ); iter != g_ButtonUpMap.end() )
    {
        return iter->second( g_GamePadStateTrackers, g_KeyboardStateTracker, g_MouseStateTracker );
    }

    return false;
}

bool Input::getKey( std::string_view keyName )
{
    // First check to see if there is a matching button in the key map:
    if ( const auto& iter = g_KeyMap.find( std::string( keyName ) ); iter != g_KeyMap.end() )
    {
        return g_KeyboardStateTracker.getLastState().isKeyDown( iter->second );
    }

    return false;
}

bool Input::getKeyDown( std::string_view keyName )
{
    // First check to see if there is a matching button in the key map:
    if ( const auto& iter = g_KeyMap.find( std::string( keyName ) ); iter != g_KeyMap.end() )
    {
        return g_KeyboardStateTracker.isKeyPressed( iter->second );
    }

    return false;
}

bool Input::getKeyUp( std::string_view keyName )
{
    // First check to see if there is a matching button in the key map:
    if ( const auto& iter = g_KeyMap.find( std::string( keyName ) ); iter != g_KeyMap.end() )
    {
        return g_KeyboardStateTracker.isKeyReleased( iter->second );
    }

    return false;
}

bool Input::getKey( KeyCode key )
{
    return g_KeyboardStateTracker.getLastState().isKeyDown( key );
}

bool Input::getKeyDown( KeyCode key )
{
    return g_KeyboardStateTracker.isKeyPressed( key );
}

bool Input::getKeyUp( KeyCode key )
{
    return g_KeyboardStateTracker.isKeyReleased( key );
}

bool Input::getMouseButton( MouseButton button )
{
    switch ( button )
    {
    case MouseButton::Left:
        return g_MouseStateTracker.getLastState().leftButton;
    case MouseButton::Right:
        return g_MouseStateTracker.getLastState().rightButton;
    case MouseButton::Middle:
        return g_MouseStateTracker.getLastState().middleButton;
    case MouseButton::XButton1:
        return g_MouseStateTracker.getLastState().xButton1;
    case MouseButton::XButton2:
        return g_MouseStateTracker.getLastState().xButton2;
    default:
        return false;
    }
}

bool Input::getMouseButtonDown( MouseButton button )
{
    switch ( button )
    {
    case MouseButton::Left:
        return g_MouseStateTracker.leftButton == ButtonState::Pressed;
    case MouseButton::Right:
        return g_MouseStateTracker.rightButton == ButtonState::Pressed;
    case MouseButton::Middle:
        return g_MouseStateTracker.middleButton == ButtonState::Pressed;
    case MouseButton::XButton1:
        return g_MouseStateTracker.xButton1 == ButtonState::Pressed;
    case MouseButton::XButton2:
    default:
        return false;
    }
}

bool Input::getMouseButtonUp( MouseButton button )
{
    switch ( button )
    {
    case MouseButton::Left:
        return g_MouseStateTracker.leftButton == ButtonState::Released;
    case MouseButton::Right:
        return g_MouseStateTracker.rightButton == ButtonState::Released;
    case MouseButton::Middle:
        return g_MouseStateTracker.middleButton == ButtonState::Released;
    case MouseButton::XButton1:
        return g_MouseStateTracker.xButton1 == ButtonState::Released;
    case MouseButton::XButton2:
    default:
        return false;
    }
}

void Input::mapAxis( std::string_view axisName, AxisCallback callback )
{
    g_AxisMap[std::string( axisName )] = std::move( callback );
}

void Input::mapButton( std::string_view buttonName, ButtonCallback callback )
{
    g_ButtonMap[std::string( buttonName )] = std::move( callback );
}

void Input::mapButtonDown( std::string_view buttonName, ButtonCallback callback )
{
    g_ButtonDownMap[std::string( buttonName )] = std::move( callback );
}

void Input::mapButtonUp( std::string_view buttonName, ButtonCallback callback )
{
    g_ButtonUpMap[std::string( buttonName )] = std::move( callback );
}
