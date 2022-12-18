#include "Font.hpp"

#include <Image.hpp>
#include <Sprite.hpp>
#include <Timer.hpp>
#include <Window.hpp>

#include <iostream>

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

    const int WINDOW_WIDTH  = 800;
    const int WINDOW_HEIGHT = 600;

    Window window { L"06 - Fonts", WINDOW_WIDTH, WINDOW_HEIGHT };

    // Image to render to.
    Image image { static_cast<uint32_t>( window.getWidth() ), static_cast<uint32_t>( window.getHeight() ) };

    // Load a font.
    Font font24 { "assets/fonts/arial.ttf", 24 };
    Font font56 { "assets/fonts/arial.ttf", 56 };

    window.show();

    Timer       timer;
    double      totalTime  = 0.0;
    uint64_t    frameCount = 0ull;
    std::string fps        = "FPS: 0";

    while ( window )
    {
        image.resize( window.getWidth(), window.getHeight() );
        image.clear( Color::Black );

        image.drawText( font24, 10, 24, fps, Color::White );

        // Draw some text centered on the screen.
        {
            const std::string text = "The quick brown fox jumps\nover the lazy dog.";
            const auto        size = font56.getSize( text );
            int               x    = ( static_cast<int>( image.getWidth() ) - size.x ) / 2;
            int               y    = ( static_cast<int>( image.getHeight() ) - size.y ) / 2;
            image.drawText( font56, x, y, text, Color::White );
        }

        window.present( image );

        Event e;
        while ( window.popEvent( e ) )
        {
            switch ( e.type )
            {
            case Event::Close:
                window.destroy();
                break;
            case Event::KeyPressed:
                switch ( e.key.code )
                {
                case KeyCode::Escape:
                    window.destroy();
                    break;
                }
                break;
            }
        }

        timer.tick();
        ++frameCount;

        totalTime += timer.elapsedSeconds();
        if ( totalTime > 1.0 )
        {
            fps = std::format( "FPS: {:.3f}", static_cast<double>( frameCount ) / totalTime );

            std::cout << fps << std::endl;

            frameCount = 0;
            totalTime  = 0.0;
        }
    }
}