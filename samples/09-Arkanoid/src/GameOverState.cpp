#include "GameOverState.hpp"

#include <Math/Rect.hpp>

using namespace Graphics;
using namespace Math;

GameOverState::GameOverState( Game& _game )
: game { _game }
{}

void GameOverState::update( float deltaTime )
{
    timer += deltaTime;

    if ( timer > timeout )
        game.setNextState( Game::GameState::MainMenu );
}

void GameOverState::draw( Graphics::Image& image )
{
    const auto& font = game.getFont();

    image.clear( Color::Black );
    image.drawText( font, "GAME OVER", 77, 123, Graphics::Color::White );
}
