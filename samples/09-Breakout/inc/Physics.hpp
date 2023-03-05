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
/// Circle-circle collision.
/// Note: When there is contact, the resulting normal is from c1 to c2 and the contact point is on c1.
/// <code>
/// normal = glm::normalize( c2.center - c1.center );
/// point = c1.center + n * c1.radius;
/// </code>
/// </summary>
/// <param name="c1">The first circle to check.</param>
/// <param name="c2">The second circle to check.</param>
/// <returns>An optional `HitInfo` structure if the circles collided.</returns>
std::optional<HitInfo> collidesWith( const Math::Circle& c1, const Math::Circle& c2 );

/// <summary>
/// Check for collision with a line and a moving circle.
/// </summary>
/// <param name="line">The line to check for collision.</param>
/// <param name="circle">The circle to check.</param>
/// <returns>An optional `HitInfo` structure if the circle collided with the line.</returns>
std::optional<HitInfo> collidesWith( const Math::Line& line, const Math::Circle& circle );

/// <summary>
/// Test for collision with an AABB and a moving circle.
/// </summary>
/// <param name="aabb">The AABB to check for collision with.</param>
/// <param name="circle">The circle to check for.</param>
/// <param name="velocity">The velocity of the circle.</param>
/// <returns>An optional `HitInfo` structure if the circle collided with the AABB.</returns>
std::optional<HitInfo> collidesWith( const Math::AABB& aabb, const Math::Circle& circle, const glm::vec2& velocity );
}  // namespace Physics