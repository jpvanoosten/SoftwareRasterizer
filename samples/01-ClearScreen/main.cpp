#include <Image.hpp>
#include <Timer.hpp>
#include <Window.hpp>

#include <iostream>

using namespace sr;

int main( int argc, char* argv[] )
{
    const int WINDOW_WIDTH  = 800;
    const int WINDOW_HEIGHT = 600;

    Window window { L"01-ClearScreen", WINDOW_WIDTH, WINDOW_HEIGHT };

    Image image { WINDOW_WIDTH, WINDOW_HEIGHT };
    image.clear( Color::Black );

    window.show();

    Timer    timer;
    double   totalTime  = 0.0;
    uint64_t frameCount = 0ull;

    while ( window )
    {
        window.present( image );

        Event e;
        while ( window.popEvent( e ) )
        {
            switch ( e.type )
            {
            case Event::Close:
                window.destroy();
                break;
            case Event::Resize:
                image.resize( e.resize.width, e.resize.height );
                image.clear( Color::Black );
                break;
            }
        }

        timer.tick();
        ++frameCount;

        totalTime += timer.elapsedSeconds();
        if ( totalTime > 1.0 )
        {
            double fps = frameCount / totalTime;
            std::cout << "FPS: " << fps << std::endl;

            frameCount = 0;
            totalTime  = 0.0;
        }
    }
}