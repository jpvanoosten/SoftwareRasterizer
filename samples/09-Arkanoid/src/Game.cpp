#include <Game.hpp>
#include <TitleState.hpp>
#include <PlayState.hpp>

#include <Graphics/Input.hpp>
#include <Graphics/KeyCodes.hpp>

using namespace Graphics;
using namespace Math;

Game::Game( uint32_t screenWidth, uint32_t screenHeight )
: image { screenWidth, screenHeight }
, arcadeN { "assets/fonts/ARCADE_N.ttf", 7 }
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

    // Input that controls adding coins.
    Input::mapButtonDown( "Coin", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool back = false;

        for ( auto& gamePadState: gamePadStates )
        {
            back = back || gamePadState.back == ButtonState::Pressed;
        }

        const bool enter = keyboardState.isKeyPressed( KeyCode::Enter );

        return back || enter;
    } );

    // Player 1 start.
    Input::mapButtonDown( "Start 1", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        const bool start = gamePadStates[0].start == ButtonState::Pressed;
        const bool _1    = keyboardState.isKeyPressed( KeyCode::D1 );

        return start || _1;
    } );
    // Player 2 start.
    Input::mapButtonDown( "Start 2", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        const bool start = gamePadStates[1].start == ButtonState::Pressed;
        const bool _2    = keyboardState.isKeyPressed( KeyCode::D2 );

        return start || _2;
    } );

    setState( GameState::Playing );
}

void Game::processEvent( const Graphics::Event& event )
{
    state->processEvent( event );
}

void Game::update( float deltaTime )
{
    // Check if the coin button was pressed.
    if ( Input::getButtonDown( "Coin" ) )
    {
        coins = std::min( 99, coins + 1 );
    }

    state->update( deltaTime );
    state->draw( image );

    // Draw the score board.
    {
        // Player 1
        image.drawText( arcadeN, "1UP", 26, 7, Color::Red );
        // Draw P1 score right-aligned.
        const auto score = std::format( "{:6d}", score1 );
        image.drawText( arcadeN, score, 15, 15, Color::White );
    }
    {
        // High score
        image.drawText( arcadeN, "HIGH SCORE", 73, 7, Color::Red );
        const auto score = std::format( "{:6d}", highScore );
        image.drawText( arcadeN, score, 87, 15, Color::White );
    }
    if ( numPlayers > 1 )
    {
        // Player 2
        image.drawText( arcadeN, "2UP", 177, 7, Color::Red );
        // Draw P2 score right-aligned.
        const auto score = std::format( "{:6d}", score2 );
        image.drawText( arcadeN, score, 164, 15, Color::White );
    }
}

Graphics::Image& Game::getImage() noexcept
{
    return image;
}

void Game::setState( GameState newState )
{
    if ( currentState != newState )
    {
        currentState = newState;

        switch ( newState )
        {
        case GameState::MainMenu:
            state = std::make_unique<TitleState>( *this );
            break;
        case GameState::Playing:
            state = std::make_unique<PlayState>( *this );
            break;
        }
    }
}
