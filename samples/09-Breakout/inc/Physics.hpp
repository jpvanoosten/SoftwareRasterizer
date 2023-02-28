#pragma once

#include <Math/AABB.hpp>
#include <Math/Circle.hpp>

#include <glm/vec2.hpp>

#include <optional>

namespace Physics
{
struct HitInfo
{
    glm::vec2 normal;  // Normal of the collision.
    glm::vec2 point;   // Point of collision.
};

/// <summary>
/// Check for collision with a line and a moving circle.
/// </summary>
/// <param name="line">The line to check for collision.</param>
/// <param name="circle">The circle to check.</param>
/// <returns></returns>
std::optional<HitInfo> collidesWith( const Math::Line& line, const Math::Circle& circle );

/// <summary>
/// Test for collision with an AABB and a moving circle.
/// </summary>
/// <param name="aabb">The AABB to check for collision with.</param>
/// <param name="circle">The circle to check for.</param>
/// <param name="velocity">The velocity of the circle.</param>
/// <returns></returns>
std::optional<HitInfo> collidesWith( const Math::AABB& aabb, const Math::Circle& circle, const glm::vec2& velocity );
}  // namespace Physics