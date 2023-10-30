#pragma once

namespace Math
{
struct Viewport
{
    Viewport() = default;

    Viewport( float x, float y, float width, float height )
    : x { x }
    , y { y }
    , width { width }
    , height { height }
    {}

    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
};
}  // namespace Math
