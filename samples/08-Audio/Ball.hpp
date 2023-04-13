#pragma once

#include <Graphics/Sprite.hpp>
#include <Math/Transform2D.hpp>

#include <glm/vec2.hpp>

class Ball
{
public:
    explicit Ball( const glm::vec2& pos = glm::vec2 { 0 } );

    void update( float dt );

    void draw( Graphics::Image& image ) const;

    void                setCircle( const Math::Circle& circle );
    const Math::Circle& getCircle() const;

    void             setPosition( const glm::vec2& position );
    const glm::vec2& getPosition() const;

    void             setVelocity( const glm::vec2& velocity );
    const glm::vec2& getVelocity() const;

private:
    Graphics::Sprite  sprite;
    Math::Transform2D transform;
    Math::Circle      circle;
    glm::vec2         velocity { 0 };
};