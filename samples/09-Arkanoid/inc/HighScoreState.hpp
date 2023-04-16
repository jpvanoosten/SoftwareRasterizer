#pragma once

#include "Game.hpp"
#include "State.hpp"

class HighScoreState : public State
{
public:
    HighScoreState( Game& game );

    void update( float deltaTime ) override;
    void draw( Graphics::Image& image ) override;
    void processEvent( const Graphics::Event& event ) override;

private:
    Game& game;
};