#pragma once

#include "Game.hpp"
#include "State.hpp"
#include "HighScores.hpp"

class HighScoreState : public State
{
public:
    HighScoreState( Game& game, int score, int round );

    void update( float deltaTime ) override;
    void draw( Graphics::Image& image ) override;
    void processEvent( const Graphics::Event& event ) override;

private:
    Game& game;

    // The currently selected initial.
    int initial = 0;
    // The currently selected character.
    int character = 0;

    HighScore highScore;
};