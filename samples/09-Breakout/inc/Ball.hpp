#pragma once

#include <Graphics/Image.hpp>
#include <Graphics/Sprite.hpp>

#include <Math/Circle.hpp>

#include <glm/vec2.hpp>

class Ball
{
public:
    Ball();

    explicit Ball( const glm::vec2& position );

    void update( float deltaTime );

    void draw( Graphics::Image& image );

    void setPosition( const glm::vec2& pos );
    const glm::vec2& getPosition() const;

    void setVelocity( const glm::vec2& vel );
    const glm::vec2& getVelocity() const;

    void setCircle( const Math::Circle& circle );
    const Math::Circle& getCircle() const;
    
private:
    glm::vec2 position{0};
    glm::vec2 velocity{0};

    Graphics::Sprite sprite;
    Math::Circle circle;
};