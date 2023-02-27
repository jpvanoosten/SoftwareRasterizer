#pragma once

#include "Ball.hpp"
#include "Paddle.hpp"
#include "State.hpp"

class PlayState : public State
{
public:
    PlayState( int screenWidth, int screenHeight );

    void update( float deltaTime ) override;
    void draw( Graphics::Image& image ) override;
    void processEvent( const Graphics::Event& event ) override;

private:
    // Check collisions with the ball.
    void checkCollisions( Ball& ball );

    // This sprite sheet contains all of the sprites for the game
    // and is shared between all of the game objects.
    std::shared_ptr<Graphics::SpriteSheet> spriteSheet;

    Graphics::Image backgroundImage;
    Ball      ball;
    Paddle    paddle;

    int screenWidth;
    int screenHeight;

    float paddleSpeed = 10.0f;  // pixels per second
};
