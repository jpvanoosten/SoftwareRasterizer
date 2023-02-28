#pragma once

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

namespace Math
{
struct Line
{
    explicit Line( const glm::vec3& p0 = glm::vec3 { 0 }, const glm::vec3& p1 = glm::vec3 { 0 } )
    : p0 { p0 }
    , p1 { p1 }
    {}

    /// <summary>
    /// Get the length of the line.
    /// </summary>
    /// <returns>The length of the line.</returns>
    [[nodiscard]] float length() const noexcept
    {
        return distance( p0, p1 );
    }

    /// <summary>
    /// Compute the closest point on this line to a point.
    /// </summary>
    /// <param name="p">The point to test.</param>
    /// <returns>The closest point on this line to `p`.</returns>
    glm::vec3 closestPoint( const glm::vec3& p ) const
    {
        const glm::vec3 ab = p1 - p0;  // ab
        // Project p onto ab, computing parameterized position d(t) = a + t * (b - a)
        float t = glm::dot( p - p0, ab ) / glm::dot( ab, ab );
        // Clamp t to the closest endpoint.
        t = glm::clamp( t, 0.0f, 1.0f );
        // Compute the projected position from the clamped t value.
        return p0 + t * ab;
    }

    /// <summary>
    /// Compute the squared distance between this line and a point.
    /// Source: Real-time Collision Detection, Christer Ericson (2005).
    /// </summary>
    /// <param name="p">The point the compute the distance to.</param>
    /// <returns>The squared distance between a point and this line.</returns>
    [[nodiscard]] float squareDistance( const glm::vec3& p ) const
    {
        const glm::vec3 dir   = p1 - p0;  // ab
        const glm::vec3 diff0 = p - p0;   // ac
        const glm::vec3 diff1 = p - p1;   // bc

        const float e = glm::dot( diff0, dir );
        if ( e <= 0.0f )
            return glm::dot( diff0, diff0 );

        const float f = glm::dot( dir, dir );
        if ( e >= f )
            return glm::dot( diff1, diff1 );

        return glm::dot( diff0, diff0 ) - e * e / f;
    }

    glm::vec3 p0 { 0 };
    glm::vec3 p1 { 0 };
};
}  // namespace Math
