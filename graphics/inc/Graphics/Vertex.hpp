#pragma once

#include "Color.hpp"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace Graphics
{
struct Vertex2D
{
    constexpr Vertex2D( const glm::vec2& position = glm::vec2 { 0 }, const glm::vec2& texCoord = glm::vec2 { 0 }, const Color& color = Color::White )
    : position { position }
    , texCoord { texCoord }
    , color { color }
    {}

    glm::vec2 position { 0 };
    glm::vec2 texCoord { 0 };
    Color     color { Color::White };
};

struct Vertex3D final
{
    constexpr explicit Vertex3D(
        const glm::vec3& position  = glm::vec3 { 0 },
        const glm::vec3& normal    = glm::vec3 { 0 },
        const glm::vec3& tangent   = glm::vec3 { 0 },
        const glm::vec3& bitangent = glm::vec3 { 0 },
        const glm::vec3& texCoord  = glm::vec3 { 0 },
        const Color&     color     = Color::White )
    : position { position }
    , normal { normal }
    , tangent { tangent }
    , bitangent { bitangent }
    , texCoord { texCoord }
    , color { color }
    {}

    Vertex3D( const Vertex3D& )            = default;
    Vertex3D( Vertex3D&& )                 = default;
    ~Vertex3D()                            = default;
    Vertex3D& operator=( const Vertex3D& ) = default;
    Vertex3D& operator=( Vertex3D&& )      = default;

    glm::vec3 position { 0 };
    glm::vec3 normal { 0 };
    glm::vec3 tangent { 0 };
    glm::vec3 bitangent { 0 };
    glm::vec3 texCoord { 0 };
    Color     color { Color::White };
};

}  // namespace Graphics
