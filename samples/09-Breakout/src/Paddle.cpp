#include <Paddle.hpp>

Paddle::Paddle() {}

Paddle::Paddle( const glm::vec2& pos )
    : position{pos}
{}

void Paddle::update( float deltaTime )
{
    // TODO: 
}

void Paddle::draw( sr::Image& image )
{
    // TODO: 
}

void Paddle::setPosition( const glm::vec2& pos )
{
    position = pos;
}
