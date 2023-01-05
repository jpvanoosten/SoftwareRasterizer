#pragma once

#include <glm/vec3.hpp>

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

    glm::vec3 center { 0.0f };
    float     radius { 0.0f };
};
}  // namespace Math
