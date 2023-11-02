#pragma once

namespace Math
{
struct Viewport
{
    Viewport() = default;

    Viewport( float x, float y, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f )
    : x { x }
    , y { y }
    , width { width }
    , height { height }
    , minDepth { minDepth }
    , maxDepth { maxDepth }
    {}

    float x        = 0.0f;
    float y        = 0.0f;
    float width    = 0.0f;
    float height   = 0.0f;
    float minDepth = 0.0f;
    float maxDepth = 1.0f;
};
}  // namespace Math
