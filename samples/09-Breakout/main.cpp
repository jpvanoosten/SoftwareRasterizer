#include "Game.hpp"

#include <Graphics/Input.hpp>
#include <Graphics/Timer.hpp>
#include <Graphics/Window.hpp>

#include <iostream>

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

    constexpr int WINDOW_WIDTH  = 1920;
    constexpr int WINDOW_HEIGHT = 1080;

    Game game { WINDOW_WIDTH, WINDOW_HEIGHT };

    sr::Window window { L"09 - Breakout", WINDOW_WIDTH, WINDOW_HEIGHT };
    window.show();
    window.setFullscreen( true );

    sr::Timer timer;

    // Maximum tick time for physics.
    constexpr float physicsTick = 1.0f / 60.0f;

    while ( window )
    {
        timer.tick();
        auto elapsedTime = static_cast<float>( timer.elapsedSeconds() );

        do
        {
            sr::Input::update();
            game.update( std::min( elapsedTime, physicsTick ) );
            elapsedTime -= physicsTick;
        } while ( elapsedTime > 0.0f );

        window.clear( sr::Color::Black );
        window.present( game.getImage() );

        sr::Event e;
        while ( window.popEvent( e ) )
        {
            // Allow the game to process events.
            game.processEvent( e );

            switch ( e.type )
            {
            case sr::Event::Close:
                window.destroy();
                break;
            case sr::Event::KeyPressed:
                switch ( e.key.code )
                {
                case sr::KeyCode::V:
                    window.setVSync( !window.isVSync() );
                    std::cout << "Vsync: " << window.isVSync() << std::endl;
                    break;
                case sr::KeyCode::Enter:
                    if ( e.key.alt )
                    {
                        [[fallthrough]];
                    case sr::KeyCode::F11:
                        window.toggleFullscreen();
                    }
                    break;
                case sr::KeyCode::Escape:
                    window.destroy();
                    break;
                }
                break;
            }
        }
    }
}