#pragma once

#include "Ball.hpp"
#include "Paddle.hpp"
#include "State.hpp"

#include <Math/Camera2D.hpp>

class Game;

class PlayState : public State
{
public:
    enum class State
    {
        Start,
        Playing,
        Dead,
    };

    PlayState( Game& game );

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

    Game& game;

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
    int screenWidth;
    int screenHeight;

    const float paddleSpeed = 200.0f;  // pixels per second
    const float ballSpeed = 100.0f;    // pixels per second
};
