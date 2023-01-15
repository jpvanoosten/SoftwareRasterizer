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
    /// Source: glm::gtx::intersectLineSphere
    /// </summary>
    /// <param name="p0">The beginning of the line.</param>
    /// <param name="p1">The end of the line.</param>
    /// <returns>`true` if the line intersects with this sphere, `false` otherwise.</returns>
    bool intersect( const glm::vec3& p0, const glm::vec3& p1 ) const
    {
        const auto dir  = glm::normalize( p1 - p0 );
        const auto diff = center - p0;
        const auto t0   = glm::dot( diff, dir );
        const auto d2   = glm::dot( diff, diff ) - t0 * t0;
        if ( d2 > radius * radius )
            return false;

        return true;
    }

    bool intersect( const Line& line) const
    {
        return intersect( line.p0, line.p1 );
    }

    glm::vec3 center { 0.0f };
    float     radius { 0.0f };
};
}  // namespace Math
