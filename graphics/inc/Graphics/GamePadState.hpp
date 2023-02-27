#pragma once

#include <cstdint>

namespace Graphics
{
struct GamePadState
{
    struct Buttons
    {
        bool a;
        bool b;
        bool x;
        bool y;
        bool leftStick;
        bool rightStick;
        bool leftShoulder;
        bool rightShoulder;

        union
        {
            bool back;
            bool view;
        };

        union
        {
            bool start;
            bool menu;
        };
    } buttons;

    struct DPad
    {
        bool up;
        bool down;
        bool right;
        bool left;
    } dPad;

    struct ThumbSticks
    {
        float leftX;
        float leftY;
        float rightX;
        float rightY;
    } thumbSticks;

    struct Triggers
    {
        float left;
        float right;
    } triggers;

    bool     connected;
    uint64_t packet;

    bool isConnected() const noexcept;

    bool isAPressed() const noexcept;
    bool isBPressed() const noexcept;
    bool isXPressed() const noexcept;
    bool isYPressed() const noexcept;

    bool isLeftStickPressed() const noexcept;
    bool isRightStickPressed() const noexcept;

    bool isLeftShoulderPressed() const noexcept;
    bool isRightShoulderPressed() const noexcept;

    bool isBackPressed() const noexcept;
    bool isViewPressed() const noexcept;
    bool isStartPressed() const noexcept;
    bool isMenuPressed() const noexcept;

    bool isDPadDownPressed() const noexcept;
    bool isDPadUpPressed() const noexcept;
    bool isDPadLeftPressed() const noexcept;
    bool isDPadRightPressed() const noexcept;

    bool isLeftThumbStickUp() const noexcept;
    bool isLeftThumbStickDown() const noexcept;
    bool isLeftThumbStickLeft() const noexcept;
    bool isLeftThumbStickRight() const noexcept;

    bool isRightThumbStickUp() const noexcept;
    bool isRightThumbStickDown() const noexcept;
    bool isRightThumbStickLeft() const noexcept;
    bool isRightThumbStickRight() const noexcept;

    bool isLeftTriggerPressed() const noexcept;
    bool isRightTriggerPressed() const noexcept;
};
inline bool GamePadState::isConnected() const noexcept
{
    return connected;
}

inline bool GamePadState::isAPressed() const noexcept
{
    return buttons.a;
}

inline bool GamePadState::isBPressed() const noexcept
{
    return buttons.b;
}

inline bool GamePadState::isXPressed() const noexcept
{
    return buttons.x;
}

inline bool GamePadState::isYPressed() const noexcept
{
    return buttons.y;
}

inline bool GamePadState::isLeftStickPressed() const noexcept
{
    return buttons.leftStick;
}

inline bool GamePadState::isRightStickPressed() const noexcept
{
    return buttons.rightStick;
}

inline bool GamePadState::isLeftShoulderPressed() const noexcept
{
    return buttons.leftShoulder;
}

inline bool GamePadState::isRightShoulderPressed() const noexcept
{
    return buttons.rightShoulder;
}

inline bool GamePadState::isBackPressed() const noexcept
{
    return buttons.back;
}

inline bool GamePadState::isViewPressed() const noexcept
{
    return buttons.view;
}

inline bool GamePadState::isStartPressed() const noexcept
{
    return buttons.start;
}

inline bool GamePadState::isMenuPressed() const noexcept
{
    return buttons.menu;
}

inline bool GamePadState::isDPadDownPressed() const noexcept
{
    return dPad.down;
}
inline bool GamePadState::isDPadUpPressed() const noexcept
{
    return dPad.up;
}
inline bool GamePadState::isDPadLeftPressed() const noexcept
{
    return dPad.left;
}
inline bool GamePadState::isDPadRightPressed() const noexcept
{
    return dPad.right;
}

inline bool GamePadState::isLeftThumbStickUp() const noexcept
{
    return thumbSticks.leftY > 0.5f;
}

inline bool GamePadState::isLeftThumbStickDown() const noexcept
{
    return thumbSticks.leftY < -0.5f;
}

inline bool GamePadState::isLeftThumbStickLeft() const noexcept
{
    return thumbSticks.leftX < -0.5f;
}

inline bool GamePadState::isLeftThumbStickRight() const noexcept
{
    return thumbSticks.leftX > 0.5f;
}

inline bool GamePadState::isRightThumbStickUp() const noexcept
{
    return thumbSticks.rightY > 0.5f;
}

inline bool GamePadState::isRightThumbStickDown() const noexcept
{
    return thumbSticks.rightY < -0.5f;
}

inline bool GamePadState::isRightThumbStickLeft() const noexcept
{
    return thumbSticks.rightX < -0.5f;
}

inline bool GamePadState::isRightThumbStickRight() const noexcept
{
    return thumbSticks.rightX > 0.5f;
}

inline bool GamePadState::isLeftTriggerPressed() const noexcept
{
    return triggers.left > 0.5f;
}

inline bool GamePadState::isRightTriggerPressed() const noexcept
{
    return triggers.right > 0.5f;
}

inline bool operator==( const GamePadState::Buttons& lhs, const GamePadState::Buttons& rhs )
{
    return lhs.a == rhs.a &&                          //
           lhs.b == rhs.b &&                          //
           lhs.x == rhs.x &&                          //
           lhs.y == rhs.y &&                          //
           lhs.leftStick == rhs.leftStick &&          //
           lhs.rightStick == rhs.rightStick &&        //
           lhs.leftShoulder == rhs.leftShoulder &&    //
           lhs.rightShoulder == rhs.rightShoulder &&  //
           lhs.back == rhs.back &&                    //
           lhs.start == rhs.start;
}

inline bool operator!=( const GamePadState::Buttons& lhs, const GamePadState::Buttons& rhs )
{
    return !( lhs == rhs );
}

inline bool operator==( const GamePadState::DPad& lhs, const GamePadState::DPad& rhs )
{
    return lhs.left == rhs.left &&    //
           lhs.right == rhs.right &&  //
           lhs.up == rhs.up &&        //
           lhs.down == rhs.down;
}

inline bool operator!=( const GamePadState::DPad& lhs, const GamePadState::DPad& rhs )
{
    return !( lhs == rhs );
}

inline bool operator==( const GamePadState::ThumbSticks& lhs,
                        const GamePadState::ThumbSticks& rhs )
{
    return lhs.leftX == rhs.leftX &&    //
           lhs.leftY == rhs.leftY &&    //
           lhs.rightX == rhs.rightX &&  //
           lhs.rightY == rhs.rightY;
}

inline bool operator!=( const GamePadState::ThumbSticks& lhs,
                        const GamePadState::ThumbSticks& rhs )
{
    return !( lhs == rhs );
}

inline bool operator==( const GamePadState::Triggers& lhs,
                        const GamePadState::Triggers& rhs )
{
    return lhs.left == rhs.left &&  //
           lhs.right == rhs.right;
}

inline bool operator!=( const GamePadState::Triggers& lhs,
                        const GamePadState::Triggers& rhs )
{
    return !( lhs == rhs );
}

inline bool operator==( const GamePadState& lhs, const GamePadState& rhs )
{
    return lhs.connected == rhs.connected &&      //
           lhs.buttons == rhs.buttons &&          //
           lhs.dPad == rhs.dPad &&                //
           lhs.thumbSticks == rhs.thumbSticks &&  //
           lhs.triggers == rhs.triggers;
}

inline bool operator!=( const GamePadState& lhs, const GamePadState& rhs )
{
    return !( lhs == rhs );
}

}  // namespace Graphics
