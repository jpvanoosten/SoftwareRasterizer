#include "Input.hpp"

#include <Game.hpp>
#include <Image.hpp>
#include <Window.hpp>

using namespace sr;
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

    Window window { L"07 - Game", SCREEN_WIDTH, SCREEN_HEIGHT };

    // The game class.
    Game game { SCREEN_WIDTH, SCREEN_HEIGHT };

    window.show();

    while ( window )
    {
        // Update the state of the input.
        Input::update();

        // Update the game.
        game.Update();

        window.clear( Color { 0x211f30 } );

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
                case KeyCode::Escape:
                    window.destroy();
                    break;
                }
                break;
            }
        }
    }
}