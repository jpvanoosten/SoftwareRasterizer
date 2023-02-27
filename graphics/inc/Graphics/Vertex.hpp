#pragma once

#include "Color.hpp"
#include <glm/vec2.hpp>

namespace Graphics
{
struct Vertex
{
    constexpr Vertex( const glm::vec2& position = glm::vec2 { 0 }, const glm::vec2& texCoord = glm::vec2 { 0 }, const Color& color = Color::White )
    : position { position }
    , texCoord { texCoord }
    , color { color }
    {}

    glm::vec2 position { 0 };
    glm::vec2 texCoord { 0 };
    Color     color { Color::White };
};
}  // namespace Graphics
