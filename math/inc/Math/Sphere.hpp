#pragma once

#include "Line.hpp"

namespace Math
{
struct Sphere
{
    Sphere() = default;
    constexpr Sphere( const glm::vec3& center, float radius )
    : center { center }
    , radius { radius }
    {}

    /// <summary>
    /// Get the diameter of the sphere.
    /// </summary>
    /// <returns>The sphere diameter.</returns>
    constexpr float diameter() const noexcept
    {
        return 2.0f * radius;
    }

    /// <summary>
    /// The sphere min point.
    /// </summary>
    /// <returns>The minimum point of the sphere.</returns>
    constexpr glm::vec3 min() const noexcept
    {
        return center - glm::vec3{ radius };
    }

    /// <summary>
    /// The sphere max point.
    /// </summary>
    /// <returns>The maximum point of the sphere.</returns>
    constexpr glm::vec3 max() const noexcept
    {
        return center + glm::vec3 { radius };
    }

    /// <summary>
    /// Line/sphere intersection test.
    /// </summary>
    /// <param name="p0">The beginning of the line.</param>
    /// <param name="p1">The end of the line.</param>
    /// <returns>`true` if the line intersects with this sphere, `false` otherwise.</returns>
    bool intersect( const glm::vec3& p0, const glm::vec3& p1 ) const
    {
        return intersect( Line { p0, p1 } );
    }

    /// <summary>
    /// Line/sphere intersection test.
    /// </summary>
    /// <param name="line">The line to test for intersection.</param>
    /// <returns>`true` if the line intersects with this sphere, `false` otherwise.</returns>
    bool intersect( const Line& line ) const
    {
        return line.squareDistance( center ) < radius * radius;
    }

    glm::vec3 center { 0.0f };
    float     radius { 0.0f };
};
}  // namespace Math
