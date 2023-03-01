#include <Game.hpp>
#include <MainMenuState.hpp>
#include <PlayState.hpp>

#include <Graphics/Input.hpp>
#include <Graphics/KeyCodes.hpp>

using namespace Graphics;
using namespace Math;

Game::Game( uint32_t screenWidth, uint32_t screenHeight )
: image { screenWidth, screenHeight }
, arial20 { "assets/fonts/arial.ttf", 20 }
, arial24 { "assets/fonts/arial.ttf", 24 }
{
    // Input that controls the horizontal movement of the paddle.
    Input::mapAxis( "Horizontal", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        float leftX = 0.0f;

        for ( auto& gamePadState: gamePadStates )
        {
            const auto state = gamePadState.getLastState();

            leftX += state.thumbSticks.leftX;
        }

        const auto keyState = keyboardState.getLastState();

        const float a     = keyState.A ? 1.0f : 0.0f;
        const float d     = keyState.D ? 1.0f : 0.0f;
        const float left  = keyState.Left ? 1.0f : 0.0f;
        const float right = keyState.Right ? 1.0f : 0.0f;

        return std::clamp( leftX - a + d - left + right, -1.0f, 1.0f );
    } );

    // Input that controls shooting.
    Input::mapButtonDown( "Fire", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool a = false;

        for ( auto& gamePadState: gamePadStates )
        {
            a = a || gamePadState.a == ButtonState::Pressed;
        }

        const bool space = keyboardState.isKeyPressed( KeyCode::Space );
        const bool up    = keyboardState.isKeyPressed( KeyCode::Up );
        const bool w     = keyboardState.isKeyPressed( Graphics::KeyCode::W );

        return a || space || up || w;
    } );

    setState( GameState::Playing );
}

void Game::processEvent( const Graphics::Event& event )
{
    state->processEvent( event );
}

void Game::update( float deltaTime )
{
    state->update( deltaTime );
    state->draw( image );
}

Graphics::Image& Game::getImage()
{
    return image;
}

void Game::setState( GameState newState )
{
    if ( currentState != newState )
    {
        switch ( newState )
        {
        case GameState::MainMenu:
            state = std::make_unique<MainMenuState>( image.getWidth(), image.getHeight() );
            break;
        case GameState::Playing:
            state = std::make_unique<PlayState>( image.getWidth(), image.getHeight() );
            break;
        }
    }
}
