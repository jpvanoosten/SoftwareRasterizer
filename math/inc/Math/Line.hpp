#pragma once

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

namespace Math
{
    struct Line
    {
        explicit Line( const glm::vec3& p0 = glm::vec3 { 0 }, const glm::vec3& p1 = glm::vec3 { 0 } )
            : p0{p0}
            , p1{p1}
        {}

        /// <summary>
        /// Get the length of the line.
        /// </summary>
        /// <returns>The length of the line.</returns>
        [[nodiscard]] float length() const noexcept
        {
            return distance( p0, p1 );
        }

        glm::vec3 p0{0};
        glm::vec3 p1{0};
    };
}
