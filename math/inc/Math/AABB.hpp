#pragma once

#include "Circle.hpp"
#include "Line.hpp"
#include "OutCodes.hpp"
#include "Rect.hpp"
#include "Sphere.hpp"

#include <glm/common.hpp>
#include <glm/vec3.hpp>

namespace Math
{

/// <summary>
/// Axis-aligned bounding box.
/// </summary>
struct AABB
{
    AABB()
    : min { std::numeric_limits<float>::max() }
    , max { std::numeric_limits<float>::lowest() }
    {}

    AABB( const AABB& copy )
    : min { glm::min( copy.min, copy.max ) }
    , max { glm::max( copy.min, copy.max ) }
    {}

    /// <summary>
    /// Construct an axis-aligned bounding box from 2 points.
    /// </summary>
    /// <param name="a">The first point.</param>
    /// <param name="b">The second point.</param>
    AABB( const glm::vec3& a, const glm::vec3& b )
    noexcept
    {
        min = glm::min( a, b );
        max = glm::max( a, b );
    }

    /// <summary>
    /// Construct an axis-aligned bounding box from 3 points.
    /// </summary>
    /// <param name="a">The first point.</param>
    /// <param name="b">The second point.</param>
    /// <param name="c">The third point.</param>
    AABB( const glm::vec3& a, const glm::vec3& b, const glm::vec3& c )
    noexcept
    {
        min = glm::min( a, glm::min( b, c ) );
        max = glm::max( a, glm::max( b, c ) );
    }

    /// <summary>
    /// Construct an axis-aligned bounding box from 4 points.
    /// </summary>
    /// <param name="a">The first point.</param>
    /// <param name="b">The second point.</param>
    /// <param name="c">The third point.</param>
    /// <param name="d">The fourth point.</param>
    /// <returns></returns>
    AABB( const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d )
    noexcept
    {
        min = glm::min( glm::min( a, b ), glm::min( c, d ) );
        max = glm::max( glm::max( a, b ), glm::max( c, d ) );
    }

    /// <summary>
    /// Translate this AABB.
    /// </summary>
    /// <param name="rhs">The amount to translate this AABB by.</param>
    /// <returns>The translated AABB.</returns>
    AABB operator+( const glm::vec3& rhs ) const noexcept
    {
        return { min + rhs, max + rhs };
    }

    /// <summary>
    /// Translate this AABB.
    /// </summary>
    /// <param name="rhs">The amount to translate this AABB by.</param>
    /// <returns>A reference to this AABB after translation.</returns>
    AABB& operator+=( const glm::vec3& rhs ) noexcept
    {
        min += rhs;
        max += rhs;

        return *this;
    }

    /// <summary>
    /// Translate this AABB.
    /// </summary>
    /// <param name="rhs">The amount to translate this AABB by.</param>
    /// <returns>The translated AABB.</returns>
    AABB operator-( const glm::vec3& rhs ) const noexcept
    {
        return { min - rhs, max - rhs };
    }

    /// <summary>
    /// Translate this AABB.
    /// </summary>
    /// <param name="rhs">The amount to translate this AABB by.</param>
    /// <returns>A reference to this AABB after translation.</returns>
    AABB& operator-=( const glm::vec3& rhs ) noexcept
    {
        min -= rhs;
        max -= rhs;

        return *this;
    }

    /// <summary>
    /// Compute the center point of the AABB.
    /// </summary>
    /// <returns>The center point of the AABB.</returns>
    glm::vec3 center() const noexcept
    {
        return ( min + max ) * 0.5f;
    }

    /// <summary>
    /// Get the width (along the x-axis) of the AABB.
    /// </summary>
    /// <returns>The width of the AABB.</returns>
    float width() const noexcept
    {
        return max.x - min.x;
    }

    /// <summary>
    /// Get the height (along the y-axis) of the AABB.
    /// </summary>
    /// <returns>The height of the AABB.</returns>
    float height() const noexcept
    {
        return max.y - min.y;
    }

    /// <summary>
    /// Get the depth (along the z-axis) of the AABB.
    /// </summary>
    /// <returns></returns>
    float depth() const noexcept
    {
        return max.z - min.z;
    }

    /// <summary>
    /// Compute the 2D area of the AABB (width x height).
    /// </summary>
    /// <returns>The 2D area of the AABB.</returns>
    float area() const noexcept
    {
        return width() * height();
    }

    /// <summary>
    /// Compute the 3D volume of the AABB.
    /// </summary>
    /// <returns>The 2D volume of the </returns>
    float volume() const noexcept
    {
        return width() * height() * depth();
    }

    /// <summary>
    /// Compute the size of the AABB.
    /// </summary>
    /// <returns>Returns the vector from the min to the max point.</returns>
    glm::vec3 size() const noexcept
    {
        return max - min;
    }

    /// <summary>
    /// Compute the extent of the AABB.
    /// The extent is 1/2 the size of the AABB.
    /// </summary>
    /// <returns></returns>
    glm::vec3 extent() const noexcept
    {
        return size() * 0.5f;
    }

    /// <summary>
    /// Expand the AABB to include a given point.
    /// </summary>
    /// <param name="p"></param>
    AABB& expand( const glm::vec3& p ) noexcept
    {
        min = glm::min( min, p );
        max = glm::max( max, p );

        return *this;
    }

    /// <summary>
    /// Expand this AABB by another AABB.
    /// </summary>
    /// <param name="aabb">The other ABB to expand this one.</param>
    AABB& expand( const AABB& aabb ) noexcept
    {
        min = glm::min( min, aabb.min );
        max = glm::max( max, aabb.max );

        return *this;
    }

    /// <summary>
    /// Clamp this AABB by another AABB.
    /// Warning: This could result in invalid AABB if the min point becomes larger than the max point.
    /// Use the <see cref="AABB::isValid"/> method to test if the AABB is still valid after clamping.
    /// </summary>
    /// <param name="aabb">The AABB to clamp this one to.</param>
    AABB& clamp( const AABB& aabb ) noexcept
    {
        min = glm::max( min, aabb.min );
        max = glm::min( max, aabb.max );

        return *this;
    }

    /// <summary>
    /// Return this AABB clamped to another.
    /// </summary>
    AABB clamped( const AABB& aabb ) const noexcept
    {
        return fromMinMax( glm::max( min, aabb.min ), glm::min( max, aabb.max ) );
    }

    /// <summary>
    /// Check to see if another AABB intersects with this one.
    /// </summary>
    /// <param name="aabb">The other AABB to check for intersection.</param>
    /// <returns>`true` if the AABBs intersect, `false` otherwise.</returns>
    bool intersect( const AABB& aabb ) const noexcept
    {
        return all( lessThanEqual( min, aabb.max ) ) && all( greaterThanEqual( max, aabb.min ) );
    }

    /// <summary>
    /// Check to see if this is a valid AABB.
    /// The min point of a valid AABB is less than the max point.
    /// </summary>
    /// <returns>true if the min point is less than the max point.</returns>
    bool isValid() const noexcept
    {
        return all( lessThanEqual( min, max ) );
    }

    /// <summary>
    /// Test whether a point is contained in this AABB.
    /// </summary>
    /// <param name="p">The point to test for containment.</param>
    /// <returns>true if the point is contained inside this AABB, false otherwise.</returns>
    bool contains( const glm::vec3& p ) const noexcept
    {
        return all( greaterThanEqual( p, min ) ) && all( lessThanEqual( p, max ) );
    }

    /// <summary>
    /// Compute the outcode of a point.
    /// Source: https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
    /// </summary>
    /// <param name="x">The x-component to test.</param>
    /// <param name="y">The y-component to test.</param>
    /// <param name="z">The z-component to test.</param>
    /// <returns>The OutCode of the point relative to this AABB.</returns>
    OutCode computeOutCode( float x, float y, float z ) const noexcept
    {
        OutCode code = OutCode::Inside;

        if ( x < min.x )
            code |= OutCode::Left;
        else if ( x > max.x )
            code |= OutCode::Right;

        if ( y < min.y )
            code |= OutCode::Bottom;
        else if ( y > max.y )
            code |= OutCode::Top;

        if ( z < min.z )
            code |= OutCode::Near;
        else if ( z > max.z )
            code |= OutCode::Far;

        return code;
    }

    /// <summary>
    /// Compute the outcode of a point.
    /// Source: https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
    /// </summary>
    /// <param name="p">The point to test.</param>
    /// <returns>The OutCode of the point relative to this AABB.</returns>
    OutCode computeOutCode( const glm::vec3& p ) const noexcept
    {
        return computeOutCode( p.x, p.y, p.z );
    }

    /// <summary>
    /// Compute the outcode of a point.
    /// Source: https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
    /// </summary>
    /// <returns>The OutCode of the point relative to this AABB.</returns>
    OutCode computeOutCode( float x, float y ) const noexcept
    {
        OutCode code = OutCode::Inside;

        if ( x < min.x )
            code |= OutCode::Left;
        else if ( x > max.x )
            code |= OutCode::Right;

        if ( y < min.y )
            code |= OutCode::Bottom;
        else if ( y > max.y )
            code |= OutCode::Top;

        return code;
    }

    /// <summary>
    /// Compute the outcode of a point.
    /// Source: https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
    /// </summary>
    /// <param name="p">The point to test.</param>
    /// <returns>The OutCode of the point relative to this AABB.</returns>
    OutCode computeOutCode( const glm::vec2& p ) const noexcept
    {
        return computeOutCode( p.x, p.y );
    }

    /// <summary>
    /// Clip a 2D line to this AABB.
    /// </summary>
    /// <param name="x0">The x-coordinate of the first point of the line.</param>
    /// <param name="y0">The y-coordinate of the first point of the line.</param>
    /// <param name="x1">The x-coordinate of the second point of the line.</param>
    /// <param name="y1">The y-coordinate of the second point fo the line.</param>
    /// <seealso href="https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm"/>
    /// <returns>`true` if the line crosses the AABB, `false` if the line is completely outside of this AABB.</returns>
    bool clip( float& x0, float& y0, float& x1, float& y1 ) const
    {
        bool accept = false;

        do
        {
            OutCode oc0 = computeOutCode( x0, y0 );
            OutCode oc1 = computeOutCode( x1, y1 );

            if ( ( oc0 | oc1 ) == 0 )
            {
                // Both points are inside the image; trivially accept and exit loop.
                accept = true;
                break;
            }
            if ( ( oc0 & oc1 ) != 0 )
            {
                // Both points are outside the image and share an outside zone.
                // So the entire line is outside the image.
                break;
            }

            // Calculate the line segment to clip from an outside
            // point to an intersection with the edge of the image.
            const OutCode oc = oc1 > oc0 ? oc1 : oc0;

            float x = 0.0f, y = 0.0f;

            // Now find the intersection point.
            if ( oc == OutCode::Top )  // Point is above the image.
            {
                x = x0 + ( x1 - x0 ) * ( max.y - y0 ) / ( y1 - y0 );
                y = max.y;
            }
            else if ( oc == OutCode::Bottom )  // Point is below the image.
            {
                x = x0 + ( x1 - x0 ) * ( min.y - y0 ) / ( y1 - y0 );
                y = min.y;
            }
            else if ( oc == OutCode::Right )  // Point is to the right of the image.
            {
                y = y0 + ( y1 - y0 ) * ( max.x - x0 ) / ( x1 - x0 );
                x = max.x;
            }
            else if ( oc == OutCode::Left )  // Point is to the left of the image.
            {
                y = y0 + ( y1 - y0 ) * ( min.x - x0 ) / ( x1 - x0 );
                x = min.x;
            }

            // Now we move the outside point to the intersection point to clip
            // and get ready for the next pass.
            if ( oc == oc0 )
            {
                x0 = x;
                y0 = y;
            }
            else
            {
                x1 = x;
                y1 = y;
            }
        } while ( true );

        return accept;
    }

    /// <summary>
    /// Clip a 2D line to this AABB.
    /// </summary>
    /// <param name="x0">The x-coordinate of the first point of the line.</param>
    /// <param name="y0">The y-coordinate of the first point of the line.</param>
    /// <param name="x1">The x-coordinate of the second point of the line.</param>
    /// <param name="y1">The y-coordinate of the second point fo the line.</param>
    /// <returns>`true` if the line crosses the AABB, `false` if the line is completely outside of this AABB.</returns>
    bool clip( int& x0, int& y0, int& x1, int& y1 ) const
    {
        float fx0 = static_cast<float>( x0 );
        float fy0 = static_cast<float>( y0 );
        float fx1 = static_cast<float>( x1 );
        float fy1 = static_cast<float>( y1 );

        if ( clip( fx0, fy0, fx1, fy1 ) )
        {
            x0 = static_cast<int>( fx0 );
            y0 = static_cast<int>( fy0 );
            x1 = static_cast<int>( fx1 );
            y1 = static_cast<int>( fy1 );

            return true;
        }

        return false;
    }

    /// <summary>
    /// Clip a 2D line to this AABB.
    /// </summary>
    /// <param name="p0">The first point of the line.</param>
    /// <param name="p1">The second point of the line.</param>
    /// <returns>`true` if the line crosses the AABB, `false` if the line is completely outside of this AABB.</returns>
    bool clip( glm::vec2& p0, glm::vec2& p1 ) const
    {
        return clip( p0.x, p0.y, p1.x, p1.y );
    }

    /// <summary>
    /// Test if a line intersects this AABB.
    /// Source: Real-time Collision Detection, Christer Ericson (2005)
    /// </summary>
    /// <param name="p0">The beginning of the line.</param>
    /// <param name="p1">The end of the line.</param>
    /// <returns>`true` if the line intersects this AABB.</returns>
    [[nodiscard]] bool intersect( const glm::vec3& p0, const glm::vec3& p1 ) const noexcept
    {
        const glm::vec3 c = center();            // AABB center point.
        const glm::vec3 e = max - c;             // AABB extents.
        glm::vec3       m = ( p0 + p1 ) * 0.5f;  // Line half-point.
        const glm::vec3 d = p1 - m;              // Line direction.

        // Translate to origin.
        m = m - c;

        const float adx = std::abs( d.x );
        if ( std::abs( m.x ) > e.x + adx )
            return false;

        const float ady = std::abs( d.y );
        if ( std::abs( m.y ) > e.y + ady )
            return false;

        const float adz = std::abs( d.z );
        if ( std::abs( m.z ) > e.z + adz )
            return false;

        // Add some small epsilon to counteract arithmetic errors when
        // the line is (near) parallel to one of the coordinate axis.
        // constexpr float epsilon = std::numeric_limits<float>::epsilon();
        // adx += epsilon;
        // ady += epsilon;
        // adz += epsilon;

        if ( std::abs( m.y * d.z - m.z * d.y ) > e.y * adz + e.z * ady )
            return false;

        if ( std::abs( m.z * d.x - m.x * d.z ) > e.x * adz + e.z * adx )
            return false;

        if ( std::abs( m.x * d.y - m.y * d.x ) > e.x * ady + e.y * adx )
            return false;

        // No separating axis found, line must be overlapping AABB.
        return true;
    }

    /// <summary>
    /// Test if a line intersects this AABB.
    /// Source: Real-time Collision Detection, Christer Ericson (2005)
    /// </summary>
    /// <param name="line">The line to test for intersection.</param>
    /// <returns>`true` if the line intersects this AABB.</returns>
    [[nodiscard]] bool intersect( const Line& line ) const noexcept
    {
        return intersect( line.p0, line.p1 );
    }

    /// <summary>
    /// Test if a sphere is colliding with this AABB.
    /// </summary>
    /// <param name="sphere">The sphere to test.</param>
    /// <returns>`true` if the sphere is colliding with this AABB, `false` otherwise.</returns>
    [[nodiscard]] bool intersect( const Sphere& sphere ) const noexcept
    {
        // Expand the AABB by the radius of the sphere.
        const AABB e { min - glm::vec3 { sphere.radius }, max + glm::vec3 { sphere.radius } };

        // Test if the center point of the sphere is in the expanded AABB.
        return e.contains( sphere.center );
    }

    /// <summary>
    /// Test if a circle is colliding with this AABB.
    /// </summary>
    /// <param name="circle">The circle to test.</param>
    /// <returns>`true` if the circle is colliding with this AABB, `false` otherwise.</returns>
    [[nodiscard]] bool intersect( const Circle& circle ) const noexcept
    {
        // Expand the AABB by the radius of the circle.
        const AABB e { min - glm::vec3 { circle.radius }, max + glm::vec3 { circle.radius } };

        // Test if the center point of the circle is in the expanded AABB.
        return e.contains( glm::vec3 { circle.center, 0 } );
    }

    /// <summary>
    /// Construct an AABB from min & max points.
    /// </summary>
    /// <param name="min">The min point.</param>
    /// <param name="max">The max point.</param>
    /// <returns></returns>
    static AABB fromMinMax( const glm::vec3& min, const glm::vec3& max )
    {
        AABB aabb;

        aabb.min = min;
        aabb.max = max;

        return aabb;
    }

    /// <summary>
    /// Construct an AABB from the three vertices of a triangle.
    /// </summary>
    /// <param name="a">The first vertex.</param>
    /// <param name="b">The second vertex.</param>
    /// <param name="c">The third vertex.</param>
    /// <returns></returns>
    static AABB fromTriangle( const glm::vec3& a, const glm::vec3& b, const glm::vec3& c )
    {
        return { a, b, c };
    }

    static AABB fromQuad( const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d )
    {
        return { a, b, c, d };
    }

    /// <summary>
    /// Construct an AABB from a rectangle.
    /// </summary>
    /// <typeparam name="T">The rectangle type.</typeparam>
    /// <param name="rect">The rectangle to use to construct an AABB.</param>
    /// <returns>The AABB that contains the rectangle.</returns>
    template<typename T>
    static AABB fromRect( const Rect<T>& rect )
    {
        return fromMinMax( { rect.topLeft(), 0.0f }, { rect.bottomRight(), 0.0f } );
    }

    static AABB fromSphere( const Sphere& sphere )
    {
        return fromMinMax( sphere.min(), sphere.max() );
    }

    static AABB fromCircle( const Circle& circle )
    {
        return fromMinMax( glm::vec3 { circle.min(), 0 }, glm::vec3 { circle.max(), 0 } );
    }

    /// <summary>
    /// Construct an AABB that is the union of two AABBs.
    /// </summary>
    /// <param name="a">The first AABB.</param>
    /// <param name="b">The second AABB.</param>
    /// <returns>The AABB that is the union of a and b.</returns>
    static AABB fromUnion( const AABB& a, const AABB& b )
    {
        return fromMinMax( glm::min( a.min, b.min ), glm::max( a.max, b.max ) );
    }

    /// <summary>
    /// Construct an AABB that is the intersection of two AABBs.
    /// Note: This can result in an invalid AABB (when the min point is larger than the max point)
    /// if there is no intersection between the AABBs. Use the <see cref="AABB::isValid"/> to test
    /// if the AABB resulting from this function is valid.
    /// </summary>
    /// <param name="a">The first AABB.</param>
    /// <param name="b">The second AABB.</param>
    /// <returns>The intersection of the two AABBs.</returns>
    static AABB fromIntersect( const AABB& a, const AABB& b )
    {
        return fromMinMax( glm::max( a.min, b.min ), glm::min( a.max, b.max ) );
    }

    /// <summary>
    /// The minimum point in the AABB.
    /// </summary>
    glm::vec3 min { 0 };

    /// <summary>
    /// The maximum point in the AABB.
    /// </summary>
    glm::vec3 max { 0 };
};
}  // namespace Math
