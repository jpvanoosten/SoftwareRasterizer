#pragma once

#include "Ball.hpp"
#include "Paddle.hpp"

#include <Font.hpp>

class Game
{
public:
    Game( uint32_t screenWidth, uint32_t screenHeight );

    // Delete copy and move constructors and assign operators
    Game( const Game& )            = delete;
    Game( Game&& )                 = delete;
    Game& operator=( const Game& ) = delete;
    Game& operator=( Game&& )      = delete;

    void update( double deltaTime );

    const sr::Image& getImage() const;

private:
    // Check collisions with the ball.
    void checkCollisions(Ball& ball);

    sr::Image image;
    // Fonts.
    sr::Font arial20;
    sr::Font arial24;

    sr::Image backgroundImage;
    Ball      ball;
    Paddle    paddle;
};