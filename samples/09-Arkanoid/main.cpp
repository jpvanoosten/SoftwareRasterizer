#include "Game.hpp"

#include <Graphics/Input.hpp>
#include <Graphics/Timer.hpp>
#include <Graphics/Window.hpp>

#include <fmt/core.h>
#include <iostream>

using namespace Graphics;

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

    constexpr int WINDOW_WIDTH  = 224;
    constexpr int WINDOW_HEIGHT = 256;

    Game game { WINDOW_WIDTH, WINDOW_HEIGHT };

    Window window { "09 - Breakout", WINDOW_WIDTH, WINDOW_HEIGHT };
    window.show();
    window.setFullscreen( true );

    Timer       timer;
    float       totalTime  = 0.0f;
    uint64_t    frameCount = 0ull;
    std::string fps        = "FPS: 0";

    // Maximum tick time for physics.
    constexpr float physicsTick = 1.0f / 200.0f;

    while ( window )
    {
        timer.tick();
        auto elapsedTime = static_cast<float>( timer.elapsedSeconds() );
        totalTime += elapsedTime;
        ++frameCount;

        do
        {
            Input::update();
            game.update( std::min( elapsedTime, physicsTick ) );
            elapsedTime -= physicsTick;
        } while ( elapsedTime > 0.0f );

        window.clear( Color::Black );
        window.present( game.getImage() );

        Event e;
        while ( window.popEvent( e ) )
        {
            // Allow the game to process events.
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

        if ( totalTime > 1.0f )
        {
            fps = fmt::format( "FPS: {:.3f}", static_cast<float>( frameCount ) / totalTime );

            std::cout << fps << std::endl;

            frameCount = 0;
            totalTime  = 0.0f;
        }

//        timer.limitFPS( 30 );
    }
}