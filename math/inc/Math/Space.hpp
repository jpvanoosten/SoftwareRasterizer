#pragma once

namespace Math
{
enum class Space
{
    Local,  ///< Translation is applied in the camera's local space.
    World,  ///< Translation is applied in screen world coordinates.
};
}
