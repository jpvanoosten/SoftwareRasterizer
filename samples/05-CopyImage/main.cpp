#include <Graphics/Font.hpp>
#include <Graphics/Image.hpp>
#include <Graphics/ResourceManager.hpp>
#include <Graphics/Timer.hpp>
#include <Graphics/Window.hpp>

#include <fmt/core.h>
#include <iostream>

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

    const int WINDOW_WIDTH  = 800;
    const int WINDOW_HEIGHT = 600;

    Window window { L"05 - Copy Image", WINDOW_WIDTH, WINDOW_HEIGHT };

    auto monaLisa = ResourceManager::loadImage( "assets/textures/Mona_Lisa.jpg" );

    // Image to render to.
    Image image { static_cast<uint32_t>( window.getWidth() ), static_cast<uint32_t>( window.getHeight() ) };

    window.show();

    Timer    timer;
    double   totalTime  = 0.0;
    uint64_t frameCount = 0ull;
    std::string fps        = "FPS: 0";

    while ( window )
    {
        image.resize( window.getWidth(), window.getHeight() );
        image.clear( Color::Black );

        // Center the image on screen and maintain the aspect ratio of the original image.
        float aspectRatio = static_cast<float>( monaLisa->getWidth() ) / static_cast<float>( monaLisa->getHeight() );
        float scaleWidth  = static_cast<float>( image.getWidth() ) / static_cast<float>( monaLisa->getWidth() );
        float scaleHeight = static_cast<float>( image.getHeight() ) / static_cast<float>( monaLisa->getHeight() );

        int width;
        int height;

        if ( scaleWidth < scaleHeight )
        {
            // Size according to the width.
            width  = image.getWidth();
            height = width / aspectRatio;
        }
        else
        {
            // Size according to the height.
            height = image.getHeight();
            width  = height * aspectRatio;
        }

        RectI dstRect = { 0, 0, width, height };
        // Center on screen.
        dstRect.left = ( static_cast<int>( image.getWidth() ) - width ) / 2;
        dstRect.top  = ( static_cast<int>( image.getHeight() ) - height ) / 2;

        image.copy( *monaLisa, {}, dstRect );

        image.drawText( Font::Default, fps, 10, 10, Color::White );

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
            fps = fmt::format( "FPS: {:.3f}", static_cast<double>(frameCount) / totalTime );

            std::cout << fps << std::endl;

            frameCount = 0;
            totalTime  = 0.0;
        }
    }
}