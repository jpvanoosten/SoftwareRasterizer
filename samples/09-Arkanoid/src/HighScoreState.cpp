#include <HighScoreState.hpp>

#include <Graphics/Input.hpp>

#include <numbers>

using namespace Graphics;

constexpr float PI = std::numbers::pi_v<float>;

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
            right = right || gamePadState.dPadRight == ButtonState::Pressed;
        }

        right        = right || keyboardState.isKeyPressed( KeyCode::Right );
        const bool d = keyboardState.isKeyPressed( KeyCode::D );

        return right || d;
    } );

    Input::mapButtonDown( "Back", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool b = false;

        for ( auto& gamePadState: gamePadStates )
        {
            b = b || gamePadState.b == ButtonState::Pressed;
        }

        const bool backspace = keyboardState.isKeyPressed( KeyCode::Back );

        return b || backspace;
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
    timer += deltaTime;

    // The number of characters in the character array.
    const int numChars = static_cast<int>( std::size( chars ) );

    if ( Input::getButtonDown( "Up" ) )
    {
        character[initial] = ( character[initial] + 1 ) % numChars;
    }
    if ( Input::getButtonDown( "Down" ) )
    {
        // This is a trick to make sure it doesn't become negative, but still wraps around to the previous character.
        character[initial] = ( character[initial] - 1 + numChars ) % numChars;
    }
    if ( Input::getButtonDown( "Left" ) || Input::getButtonDown( "Back" ) )
    {
        initial = std::max( 0, initial - 1 );
    }
    if ( Input::getButtonDown( "Right" )  )
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

    highScore.name[initial] = chars[character[initial]];
}

void HighScoreState::draw( Graphics::Image& image )
{
    const auto& font = game.getFont();

    image.clear( Color::Black );

    image.drawText( font, "ENTER YOUR INITIALS !", 31, 79, Color::Red );
    image.drawText( font, "SCORE ROUND   NAME", 40, 104, Color::Yellow );
    image.drawText( font, std::format( "{:8d}", highScore.score), 20, 120, Color::White );
    image.drawText( font, std::format( "{:>3}", highScore.round ), 95, 120, Color::Yellow );

    for ( int i = 0; i < 3; ++i )
    {

        auto color = Color::Yellow;
        if (i == initial)
        {
            color = std::sin( timer * PI * 12.0f ) > 0.0f ? Color::Red : Color::White;
        }

        image.drawText( font, std::format( "{}", highScore.name[i] ), 144 + i * 7, 120, color );
    }


}

void HighScoreState::processEvent( const Graphics::Event& event )
{
}
