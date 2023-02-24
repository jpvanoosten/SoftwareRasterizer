#pragma once

#include "Ball.hpp"
#include "Paddle.hpp"
#include "State.hpp"

class PlayState : public State
{
public:
    PlayState( int screenWidth, int screenHeight );

    void update( float deltaTime ) override;
    void draw( sr::Image& image ) override;
    void processEvent( const sr::Event& event ) override;

private:
    // Check collisions with the ball.
    void checkCollisions( Ball& ball );

    // This sprite sheet contains all of the sprites for the game
    // and is shared between all of the game objects.
    std::shared_ptr<sr::SpriteSheet> spriteSheet;

    sr::Image backgroundImage;
    Ball      ball;
    Paddle    paddle;

    int screenWidth;
    int screenHeight;

    float paddleSpeed = 10.0f;  // pixels per second
};
