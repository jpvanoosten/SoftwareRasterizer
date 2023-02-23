#pragma once

#include "Image.hpp"

#include <Math/AABB.hpp>

#include <glm/vec2.hpp>

class Paddle
{
public:
    Paddle( const glm::vec2& pos );

    void update( float deltaTime );

    void draw( sr::Image& image );

private:
    Math::AABB aabb;
    glm::vec2 pos;
};
