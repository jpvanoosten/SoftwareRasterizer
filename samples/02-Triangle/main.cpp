#include <Font.hpp>
#include <Image.hpp>
#include <Timer.hpp>
#include <Window.hpp>

#include <iostream>

using namespace sr;

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

    Window window { L"02 - Triangle", WINDOW_WIDTH, WINDOW_HEIGHT };
    Image  image { WINDOW_WIDTH, WINDOW_HEIGHT };

    window.show();

    Timer       timer;
    double      totalTime  = 0.0;
    uint64_t    frameCount = 0ull;
    std::string fps        = "FPS: 0";

    while ( window )
    {
        image.clear( Color::Black );

        float dx = std::sinf( timer.totalSeconds() * 0.5 ) * static_cast<float>( WINDOW_WIDTH / 2 );

        // Draw a red triangle in the middle of the screen.
        image.drawTriangle( { WINDOW_WIDTH * 0.5f + dx, WINDOW_HEIGHT * 0.25 }, { WINDOW_WIDTH * 0.25f + dx, WINDOW_HEIGHT * 0.75f }, { WINDOW_WIDTH * 0.75f + dx, WINDOW_HEIGHT * 0.75f }, Color::Red );
        // Draw a blue outline for the triangle.
        image.drawTriangle( { WINDOW_WIDTH * 0.5f + dx, WINDOW_HEIGHT * 0.25 }, { WINDOW_WIDTH * 0.25f + dx, WINDOW_HEIGHT * 0.75f }, { WINDOW_WIDTH * 0.75f + dx, WINDOW_HEIGHT * 0.75f }, Color::Blue, {}, FillMode::WireFrame );

        image.drawText( Font::Default, 10, 10, fps, Color::White );

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