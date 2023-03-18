#pragma once

#include "Ball.hpp"
#include "Field.hpp"
#include "State.hpp"
#include "Vaus.hpp"

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
    void startState( State newState );
    void endState( State oldState );

private:
    void doStart( float deltaTime );
    void doPlaying( float deltaTime );

    void updatePaddle( float deltaTime );

    // Check collisions with the ball.
    void checkCollisions( Ball& ball );

    Game& game;

    State state = State::Start;

    Field field;
    Ball  ball;
    Vaus  vaus;

    // Size of the play area.
    int screenWidth;
    int screenHeight;

    const float paddleSpeed = 200.0f;  // pixels per second
    const float ballSpeed   = 100.0f;  // pixels per second
};
