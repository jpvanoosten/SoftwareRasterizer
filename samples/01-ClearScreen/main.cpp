#include <Graphics/Font.hpp>

#include <Graphics/Image.hpp>
#include <Graphics/Timer.hpp>
#include <Graphics/Window.hpp>

#include <iostream>

using namespace Graphics;

int main( int argc, char* argv[] )
{
    const int WINDOW_WIDTH  = 800;
    const int WINDOW_HEIGHT = 600;

    Window window { L"01-ClearScreen", WINDOW_WIDTH, WINDOW_HEIGHT };

    Image image { WINDOW_WIDTH, WINDOW_HEIGHT };

    window.show();

    Timer       timer;
    double      totalTime  = 0.0;
    uint64_t    frameCount = 0ull;
    std::string fps        = "FPS: 0";

    while ( window )
    {
        image.clear( Color::Black );

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
                case KeyCode::V:
                    window.toggleVSync();
                    break;
                case KeyCode::Escape:
                    window.destroy();
                    break;
                }
                break;
            case Event::EndResize:
                std::cout << std::format( "Resize: {},{}\n", e.resize.width, e.resize.height );
                image.resize( e.resize.width, e.resize.height );
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