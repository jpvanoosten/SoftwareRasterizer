#pragma once

#include "State.hpp"
#include "Game.hpp"

class GameOverState : public State
{
public:
    explicit GameOverState( Game& game );

    void         update( float deltaTime ) override;
    void draw( Graphics::Image& image ) override;

private:
    Game& game;

    float timer = 0.0f;
    // How long to transition to the game to the title state.
    static constexpr inline float timeout = 5.0f;
};