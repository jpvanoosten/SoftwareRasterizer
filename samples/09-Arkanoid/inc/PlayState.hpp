#pragma once

#include "Ball.hpp"
#include "Field.hpp"
#include "State.hpp"
#include "Vaus.hpp"
#include "Level.hpp"

#include <Math/Camera2D.hpp>

class Game;

class PlayState : public State
{
public:
    enum class State
    {
        Ready,
        Appear,
        Start,
        Playing,
        Dead,
        GameOver,
    };

    PlayState( Game& game );

    void update( float deltaTime ) override;
    void draw( Graphics::Image& image ) override;

    void setState( State newState );
    State getState() const noexcept;

private:
    void startState( State newState );
    void endState( State oldState );

    void doReady( float deltaTime );
    void doAppear( float deltaTime );
    void doStart( float deltaTime );
    void doPlaying( float deltaTime );
    void doDead( float deltaTime );

    // Check collisions with the ball.
    void checkCollisions( Ball& ball );

    void drawText( Graphics::Image& image, std::string_view text, int x, int y );

    Game& game;
    Graphics::Font arcadeN;

    State state = State::Dead;

    Field field;
    Ball  ball;
    Vaus  vaus;
    Level level;

    // Size of the play area.
    int screenWidth;
    int screenHeight;

    // Number of lives.
    int numLives = 3;
    // The current level.
    int levelId = 0;

    float time = 0.0f;

    static inline const float ballSpeed   = 200.0f;  // pixels per second
};
