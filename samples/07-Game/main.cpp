#include <Image.hpp>
#include <Sprite.hpp>
#include <Timer.hpp>
#include <Window.hpp>
#include <Game.hpp>

#include <iostream>

using namespace sr;
using namespace Math;

// Copy a source image to the destination image while maintaining the aspect ratio
// of the source image.
void CopyImage( Image& dst, const Image& src )
{
    // Center the image on screen and maintain the aspect ratio of the original image.
    const float aspectRatio = static_cast<float>( src.getWidth() ) / static_cast<float>( src.getHeight() );
    const float scaleWidth  = static_cast<float>( dst.getWidth() ) / static_cast<float>( src.getWidth() );
    const float scaleHeight = static_cast<float>( dst.getHeight() ) / static_cast<float>( src.getHeight() );

    int width;
    int height;

    if ( scaleWidth < scaleHeight )
    {
        // Size according to the width.
        width  = static_cast<int>( dst.getWidth() );
        height = static_cast<int>( static_cast<float>( width ) / aspectRatio );
    }
    else
    {
        // Size according to the height.
        height = static_cast<int>( dst.getHeight() );
        width  = static_cast<int>( static_cast<float>( height ) * aspectRatio );
    }

    RectI dstRect = { 0, 0, width, height };
    // Center on screen.
    dstRect.left = ( static_cast<int>( dst.getWidth() ) - width ) / 2;
    dstRect.top  = ( static_cast<int>( dst.getHeight() ) - height ) / 2;

    dst.copy( src, {}, dstRect );
}

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

    constexpr int SCREEN_WIDTH  = 800;
    constexpr int SCREEN_HEIGHT = 600;

    Window window { L"07 - Game", SCREEN_WIDTH, SCREEN_HEIGHT };

    // Image to render the final image to.
    Image image { static_cast<uint32_t>( window.getWidth() ), static_cast<uint32_t>( window.getHeight() ) };
    image.clear( Color::Black );

    // The game class.
    Game game { SCREEN_WIDTH, SCREEN_HEIGHT };

    window.show();

    while ( window )
    {
        // Update the game.
        game.Update();

        // Copy the game screen to the window's image.
        CopyImage( image, game.getImage() );

        // Present.
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
            case Event::Resize:
                image.resize( window.getWidth(), window.getHeight() );
                image.clear( Color::Black );
                break;
            }
        }
    }
}