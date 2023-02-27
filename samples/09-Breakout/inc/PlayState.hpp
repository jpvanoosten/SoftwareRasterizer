#pragma once

#include "Ball.hpp"
#include "Paddle.hpp"
#include "State.hpp"

#include <Math/Camera2D.hpp>

class PlayState : public State
{
public:
    enum class State
    {
        Start,
        Playing,
        Dead,
    };

    PlayState( int screenWidth, int screenHeight );

    void update( float deltaTime ) override;
    void draw( Graphics::Image& image ) override;

    void setState( State newState );
    void startState(State newState );
    void endState(State oldState );

private:
    void doStart( float deltaTime );
    void doPlaying( float deltaTime );

    void updatePaddle( float deltaTime );

    // Check collisions with the ball.
    void checkCollisions( Ball& ball );

    State state = State::Start;

    // This sprite sheet contains all of the sprites for the game
    // and is shared between all of the game objects.
    std::shared_ptr<Graphics::SpriteSheet> spriteSheet;

    // Camera for scaling the objects on the screen.
    Math::Camera2D camera;

    Graphics::Image backgroundImage;
    Ball      ball;
    Paddle    paddle;

    // Size of the play area.
    int width;
    int height;

    const float paddleSpeed = 2000.0f;  // pixels per second
    const float ballSpeed = 1000.0f;    // pixels per second
};
