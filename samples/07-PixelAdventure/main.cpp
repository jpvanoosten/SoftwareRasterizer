#include <Game.hpp>

#include <Graphics/Image.hpp>
#include <Graphics/Input.hpp>
#include <Graphics/Window.hpp>

using namespace Graphics;
using namespace Math;

int main( int argc, char* argv[] )
{
    // Parse command-line arguments.
    if ( argc > 1 )
    {
        for ( int i = 0; i < argc; ++i )
        {
            if ( strcmp( argv[i], "-cwd" ) == 0 )
            {
                std::string workingDirectory = argv[++i];
                std::filesystem::current_path( workingDirectory );
            }
        }
    }

    constexpr int SCREEN_WIDTH  = 480;
    constexpr int SCREEN_HEIGHT = 256;

    // The game class.
    Game game { SCREEN_WIDTH, SCREEN_HEIGHT };

    Window window { "07 - Game", SCREEN_WIDTH, SCREEN_HEIGHT };

    window.show();
    window.setFullscreen( true );

    while ( window )
    {
        window.clear( Color { 0x211f30u } );

        // Update the game.
        game.Update();

        // Present.
        window.present( game.getImage() );

        // Handle events.
        Event e;
        while ( window.popEvent( e ) )
        {
            // Let the game process it's events.
            game.processEvent( e );

            switch ( e.type )
            {
            case Event::Close:
                window.destroy();
                break;
            case Event::KeyPressed:
                switch ( e.key.code )
                {
                case KeyCode::V:
                    window.setVSync( !window.isVSync() );
                    std::cout << "Vsync: " << window.isVSync() << std::endl;
                    break;
                case KeyCode::Enter:
                    if ( e.key.alt )
                    {
                        [[fallthrough]];
                    case KeyCode::F11:
                        window.toggleFullscreen();
                    }
                    break;
                case KeyCode::Escape:
                    window.destroy();
                    break;
                }
                break;
            }
        }
    }
}