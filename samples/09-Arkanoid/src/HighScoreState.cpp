#include <HighScoreState.hpp>

#include <Graphics/Input.hpp>

using namespace Graphics;

static const std::string chars    = "ABCDEFGHIJKLMNOPQRSTUVWXYZ.! ";  // I don't know what the possible characters are...
static const std::string palace[] = {
    "1ST",
    "2ND",
    "3RD",
    "4TH",
    "5TH"
};

HighScoreState::HighScoreState( Game& _game, int score, int round )
: game { _game }
, highScore { score, round, "   " }
{
    // Map controls for entering the initials for a high score.
    Input::mapButtonDown( "Up", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool up = false;

        for ( auto& gamePadState: gamePadStates )
        {
            up = up || gamePadState.dPadUp == ButtonState::Pressed;
        }

        up           = up || keyboardState.isKeyPressed( KeyCode::Up );
        const bool w = keyboardState.isKeyPressed( KeyCode::W );

        return up || w;
    } );

    Input::mapButtonDown( "Down", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool down = false;

        for ( auto& gamePadState: gamePadStates )
        {
            down = down || gamePadState.dPadDown == ButtonState::Pressed;
        }

        down         = down || keyboardState.isKeyPressed( KeyCode::Down );
        const bool s = keyboardState.isKeyPressed( KeyCode::S );

        return down || s;
    } );

    Input::mapButtonDown( "Left", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool left = false;

        for ( auto& gamePadState: gamePadStates )
        {
            left = left || gamePadState.dPadLeft == ButtonState::Pressed;
        }

        left         = left || keyboardState.isKeyPressed( KeyCode::Left );
        const bool a = keyboardState.isKeyPressed( KeyCode::A );

        return left || a;
    } );

    Input::mapButtonDown( "Right", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool right = false;

        for ( auto& gamePadState: gamePadStates )
        {
            right = right || gamePadState.dPadLeft == ButtonState::Pressed;
        }

        right        = right || keyboardState.isKeyPressed( KeyCode::Left );
        const bool d = keyboardState.isKeyPressed( KeyCode::D );

        return right || d;
    } );

    Input::mapButtonDown( "Enter", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool a = false;

        for ( auto& gamePadState: gamePadStates )
        {
            a = a || gamePadState.a == ButtonState::Pressed;
        }

        const bool enter = keyboardState.isKeyPressed( KeyCode::Enter );

        return a || enter;
    } );
}

void HighScoreState::update( float deltaTime )
{
    // The number of characters in the character array.
    const int numChars = static_cast<int>( std::size( chars ) );

    if ( Input::getButtonDown( "Up" ) )
    {
        character = ( character + 1 ) % numChars;
    }
    if ( Input::getButtonDown( "Down" ) )
    {
        // This is a trick to make sure it doesn't become negative, but still wraps around to the previous character.
        character = ( character + numChars + 1 ) % numChars;
    }
    if ( Input::getButtonDown( "Left" ) )
    {
        initial = std::max( 0, initial - 1 );
    }
    if ( Input::getButtonDown( "Right" ) )
    {
        initial = std::min( 2, initial + 1 );
    }
    if ( Input::getButtonDown( "Enter" ) )
    {
        if ( initial == 2 )
        {
            game.addHighScore( highScore );
            game.setNextState( Game::GameState::GameOver );
        }
        else
        {
            ++initial;
        }
    }

    highScore.name[initial] = chars[character];
}

void HighScoreState::draw( Graphics::Image& image )
{
    const auto& font = game.getFont();

    image.clear( Color::Black );

    image.drawText( font, "ENTER YOUR INITIALS !", 31, 79, Color::Red );
    image.drawText( font, "SCORE ROUND   NAME", 40, 104, Color::Yellow );
    image.drawText( font, std::format( "{}", highScore.score ), 24, 120, Color::White );
    image.drawText( font, std::format( "{}", highScore.round ), 104, 120, Color::Yellow );
    // TODO: Blink the current initial between red and white.
    image.drawText( font, std::format( "{}", highScore.name[0] ), 150, 120, Color::Yellow );
    image.drawText( font, std::format( "{}", highScore.name[1] ), 160, 120, Color::Yellow );
    image.drawText( font, std::format( "{}", highScore.name[2] ), 170, 120, Color::Yellow );
}

void HighScoreState::processEvent( const Graphics::Event& event )
{
}
