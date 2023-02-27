#pragma once

#include <glm/vec2.hpp>

namespace Math
{
struct Circle
{
    Circle() = default;

    Circle( const glm::vec2& center, float radius )
    : center { center }
    , radius { radius }
    {}

    /// <summary>
    /// Get the diameter of the circle.
    /// </summary>
    /// <returns></returns>
    float diameter() const noexcept
    {
        return radius * 2.0f;
    }

    /// <summary>
    /// Get the top edge of the circle.
    /// </summary>
    /// <returns>The top edge of the circle.</returns>
    float top() const noexcept
    {
        return center.y - radius;
    }

    /// <summary>
    /// Get the left edge of the circle.
    /// </summary>
    /// <returns>The left edge of the circle.</returns>
    float left() const noexcept
    {
        return center.x - radius;
    }

    /// <summary>
    /// Get the bottom edge of the circle.
    /// </summary>
    /// <returns>The bottom edge of the circle.</returns>
    float bottom() const noexcept
    {
        return center.y + radius;
    }

    /// <summary>
    /// Get the right edge of the circle.
    /// </summary>
    /// <returns>The right edge of the circle.</returns>
    float right() const noexcept
    {
        return center.x + radius;
    }

    /// <summary>
    /// Get the minimum point of the circle.
    /// </summary>
    /// <returns></returns>
    glm::vec2 min() const noexcept
    {
        return { center.x - radius, center.y - radius };
    }

    /// <summary>
    /// Get the maximum point of the circle.
    /// </summary>
    /// <returns></returns>
    glm::vec2 max() const noexcept
    {
        return { center.x + radius, center.y + radius };
    }

    /// <summary>
    /// Circle-circle intersection test.
    /// </summary>
    /// <param name="other">The other circle to test for intersection with.</param>
    /// <returns>`true` if the circles are overlapping, `false` otherwise.</returns>
    bool intersect( const Circle& other ) const noexcept
    {
        float dx = center.x - other.center.x;
        float dy = center.y - other.center.y;
        float d = dx * dx + dy * dy;
        float r = radius + other.radius;

        return d < r * r;
    }

    glm::vec2 center { 0 };
    float     radius { 0 };
};
}  // namespace Math
