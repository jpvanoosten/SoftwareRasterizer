#pragma once

#include "Image.hpp"

#include <Math/AABB.hpp>

#include <glm/vec2.hpp>

class Paddle
{
public:
    Paddle();

    Paddle( const glm::vec2& pos );

    void update( float deltaTime );

    void draw( sr::Image& image );

    void setPosition( const glm::vec2& pos );
    const glm::vec2& getPosition() const;

private:
    Math::AABB aabb;
    glm::vec2 position{0};
};
