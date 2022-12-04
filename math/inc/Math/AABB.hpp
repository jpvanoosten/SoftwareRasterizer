#pragma once

#include <glm/common.hpp>
#include <glm/vec3.hpp>

namespace Math
{
    /// <summary>
    /// Axis-aligned bounding box.
    /// </summary>
    struct AABB
    {
        AABB() = default;

        AABB(const AABB& copy)
        : min{copy.min}
        , max{copy.max}
        {}

        /// <summary>
        /// Construct an axis-aligned bounding box from 2 points.
        /// </summary>
        /// <param name="a">The first point.</param>
        /// <param name="b">The second point.</param>
        AABB(const glm::vec3& a, const glm::vec3& b) noexcept
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
        AABB(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) noexcept
        {
            min = glm::min( a, glm::min( b, c ) );
            max = glm::max( a, glm::max( b, c ) );
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
        void expand(const glm::vec3& p) noexcept
        {
            min = glm::min( min, p );
            max = glm::max( max, p );
        }

        /// <summary>
        /// Expand this AABB by another AABB.
        /// </summary>
        /// <param name="aabb">The other ABB to expand this one.</param>
        void expand(const AABB& aabb) noexcept
        {
            min = glm::min( min, aabb.min );
            max = glm::max( max, aabb.max );
        }

        /// <summary>
        /// Clamp this AABB by another AABB.
        /// Warning: This could result in invalid AABB if the min point becomes larger than the max point.
        /// Use the <see cref="AABB::isValid"/> method to test if the AABB is still valid after clamping.
        /// </summary>
        /// <param name="aabb">The AABB to clamp this one to.</param>
        void clamp(const AABB& aabb) noexcept
        {
            min = glm::max( min, aabb.min );
            max = glm::min( max, aabb.max );
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
        bool contains(const glm::vec3& p) const noexcept
        {
            return all( greaterThanEqual( p, min ) ) && all( lessThanEqual( p, max ) ); 
        }

        /// <summary>
        /// Construct an AABB from min & max points.
        /// </summary>
        /// <param name="min">The min point.</param>
        /// <param name="max">The max point.</param>
        /// <returns></returns>
        static AABB fromMinMax(const glm::vec3& min, const glm::vec3& max )
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
        static AABB fromTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
        {
            return { a, b, c };
        }

        /// <summary>
        /// Construct an AABB that is the union of two AABBs.
        /// Note: union is a keyword in C++, so I had to name this function unionAABB.
        /// </summary>
        /// <param name="a">The first AABB.</param>
        /// <param name="b">The second AABB.</param>
        /// <returns>The AABB that is the union of a and b.</returns>
        static AABB unionAABB(const AABB& a, const AABB& b )
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
        static AABB intersect( const AABB& a, const AABB& b)
        {
            return fromMinMax( glm::max( a.min, b.min ), glm::min( a.max, b.max ) );
        }

        /// <summary>
        /// The minimum point in the AABB.
        /// </summary>
        glm::vec3 min{0};

        /// <summary>
        /// The maximum point in the AABB.
        /// </summary>
        glm::vec3 max{0};
    };
}
