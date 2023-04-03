#include <Physics.hpp>

#include <array>

using namespace Math;

namespace Physics
{
std::optional<HitInfo> collidesWith( const Math::Circle& c1, const Math::Circle& c2 )
{
    const float dx = c2.center.x - c1.center.x;
    const float dy = c2.center.y - c1.center.y;
    const float d  = dx * dx + dy * dy;
    const float r  = c1.radius + c2.radius;

    if (d < r * r)
    {
        // The normal is in the direction of c1 to c2.
        const glm::vec2 n = glm::normalize( glm::vec2 { dx, dy } );
        // The contact point is on c1.
        const glm::vec2 p = c1.center + n * c1.radius;

        return HitInfo { n, p };
    }

    // No intersection found.
    return {};
}

std::optional<HitInfo> collidesWith( const Math::Line& line, const Math::Circle& c )
{
    // Get the closest point on the line to the circle's center.
    const auto p = line.closestPoint( glm::vec3 { c.center, 0 } );
    // Compute the distance between the circle's center and the closest point on the line.
    const float dx = c.center.x - p.x;
    const float dy = c.center.y - p.y;
    // Squared distance from the closest point to the circle's center.
    const float d = dx * dx + dy * dy;

    // Circle is intersecting with the line.
    if ( d < c.radius * c.radius )
    {
        // TODO: Compute the normal vector that is perpendicular to the line in the direction of the center of the circle.
        // n = p - dot( p, line.p1 - line.p0 ); // Something like this...
        const glm::vec2 n = glm::normalize( glm::vec2 { dx, dy } );
        return HitInfo { n, p };
    }

    // Circle is not intersecting with the line.
    return {};
}

std::optional<HitInfo> collidesWith( const AABB& aabb, const Circle& circle, const glm::vec2& velocity )
{
    enum Side
    {
        Left,
        Right,
        Top,
        Bottom
    };

    std::array<Side, 2> sides {};

    // The velocity of the circle determines the order of the sides to check for collision.
    if ( glm::abs( velocity.y ) > glm::abs( velocity.x ) )  // The circle is moving faster in the vertical direction.
        if ( velocity.y > 0.0f )                            // The circle is moving down.
            if ( velocity.x > 0.0f )                        // The circle is moving down and to the right.
                sides = { Top, Left };
            else  // The circle is moving down and to the left.
                sides = { Top, Right };
        else                          // The circle is moving up.
            if ( velocity.x > 0.0f )  // The circle is moving up and to the right.
                sides = { Bottom, Left };
            else  // The circle is moving up and to the left.
                sides = { Bottom, Right };
    else                              // The circle is moving faster in the horizontal direction.
        if ( velocity.x > 0.0f )      // The circle is moving right.
            if ( velocity.y > 0.0f )  // The circle is moving right and down.
                sides = { Left, Top };
            else  // The circle is moving right and up.
                sides = { Left, Bottom };
        else                          // The circle is moving left.
            if ( velocity.y > 0.0f )  // The circle is moving left and down.
                sides = { Right, Top };
            else  // The circle is moving left and up.
                sides = { Right, Bottom };

    // Check the sides in the priority order.
    for ( auto side: sides )
    {
        switch ( side )
        {
        case Left:
        {
            Line leftEdge { { aabb.min.x, aabb.min.y, 0 }, { aabb.min.x, aabb.max.y, 0 } };
            if ( auto hit = collidesWith( leftEdge, circle ) )
            {
                hit->normal = glm::vec2 { -1, 0 };
                return hit;
            }
        }
        break;
        case Right:
        {
            Line rightEdge { { aabb.max.x, aabb.min.y, 0 }, { aabb.max.x, aabb.max.y, 0 } };
            if ( auto hit = collidesWith( rightEdge, circle ) )
            {
                hit->normal = glm::vec2 { 1, 0 };
                return hit;
            }
        }
        break;
        case Top:
        {
            Line topEdge { { aabb.min.x, aabb.min.y, 0 }, { aabb.max.x, aabb.min.y, 0 } };
            if ( auto hit = collidesWith( topEdge, circle ) )
            {
                hit->normal = glm::vec2 { 0, -1 };
                return hit;
            }
        }
        break;
        case Bottom:
        {
            Line bottomEdge { { aabb.min.x, aabb.max.y, 0 }, { aabb.max.x, aabb.max.y, 0 } };
            if ( auto hit = collidesWith( bottomEdge, circle ) )
            {
                hit->normal = glm::vec2 { 0, 1 };
                return hit;
            }
        }
        break;
        }
    }

    // No collision occurred.
    return {};
}

}  // namespace Physics