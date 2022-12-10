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

    Window window { L"05 - Copy Image", WINDOW_WIDTH, WINDOW_HEIGHT };

    Image monaLisa   = Image::fromFile( "assets/textures/Mona_Lisa.jpg" );

    // Image to render to.
    Image image { static_cast<uint32_t>( window.getWidth() ), static_cast<uint32_t>( window.getHeight() ) };

    window.show();

    Timer    timer;
    double   totalTime  = 0.0;
    uint64_t frameCount = 0ull;

    while ( window )
    {
        image.resize( window.getWidth(), window.getHeight() );
        image.clear( Color::Black );

        // Center the image on screen and maintain the aspect ratio.
        float scaleRatio;
        if (image.getWidth() > image.getHeight())
        {
            scaleRatio = static_cast<float>( image.getHeight() ) / static_cast<float>( monaLisa.getHeight() );
        }
        else
        {
            scaleRatio = static_cast<float>( image.getWidth() ) / static_cast<float>( monaLisa.getWidth() );
        }

        RectI dstRect = { 0, 0, static_cast<int>( monaLisa.getWidth() * scaleRatio ), static_cast<int>( monaLisa.getHeight() * scaleRatio ) };
        dstRect.left  = ( static_cast<int>( image.getWidth() ) - dstRect.width ) / 2;
        dstRect.top   = ( static_cast<int>( image.getHeight() ) - dstRect.height ) / 2;

        image.copy( monaLisa, {}, dstRect );
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
            std::cout << std::format( "FPS: {:.3f}\n", frameCount / totalTime );

            frameCount = 0;
            totalTime  = 0.0;
        }
    }
}