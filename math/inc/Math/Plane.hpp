#pragma once

#include "Line.hpp"

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

namespace Math
{
struct Plane
{
    Plane() = default;

    /// <summary>
    /// Construct a plane.
    /// </summary>
    /// <param name="n">The plane normal.</param>
    /// <param name="d">d = dot(n, p) for a point p in the plane.</param>
    constexpr Plane( const glm::vec3& n, float d )
    : n { n }
    , d { d }
    {}

    /// <summary>
    /// Compute a plane from 3 noncollinear points (ccw).
    /// Source: Real-time Collision Detection, Christer Ericson (2005)
    /// </summary>
    /// <param name="v0">The first point.</param>
    /// <param name="v1">The second point.</param>
    /// <param name="v2">The third point.</param>
    Plane( const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2 ) noexcept
    {
        n = glm::normalize( glm::cross( v1 - v0, v2 - v0 ) );
        d = glm::dot( n, v0 );
    }

    /// <summary>
    /// Compute the distance from a point to the plane.
    /// Source: Real-time Collision Detection, Christer Ericson (2005)
    /// </summary>
    /// <param name="p">The point to compute the distance to.</param>
    /// <returns>The (signed) distance from the point to the plane.</returns>
    float distance( const glm::vec3& p ) const noexcept
    {
        return glm::dot( p, n ) - d;
    }

    /// <summary>
    /// Compute the intersection of the line segment (v1 - v0) with the plane.
    /// Source: Real-time Collision Detection, Christer Ericson (2005)
    /// </summary>
    /// <param name="v0">The beginning of the line segment.</param>
    /// <param name="v1">The end of the line segment.</param>
    /// <returns>The normalized intersection point (t) along the line segment from v0 to v1.</returns>
    float intersect( const glm::vec3& v0, const glm::vec3& v1 ) const noexcept
    {
        const auto e01 = v1 - v0;
        return ( d - glm::dot( n, v0 ) ) / glm::dot( n, e01 );
    }

    /// <summary>
    /// Computes the intersection of the line segment with the plane.
    /// </summary>
    /// <param name="l">The line segment to test for intersection.</param>
    /// <returns>The normalized intersection point (t) along the line segment.</returns>
    float intersect(const Line& l) const noexcept
    {
        return intersect( l.p0, l.p1 );
    }

    /// <summary>
    /// Plane normal.
    /// </summary>
    glm::vec3 n = { 0, 0, 1 };

    /// <summary>
    /// Distance from the origin.
    /// d = dot(n, p) for a point p in the plane.
    /// </summary>
    float d = 0.0f;
};
}  // namespace Math
