#include "Font.hpp"

#include <Image.hpp>
#include <ResourceManager.hpp>
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

    Window window { L"04 - Blend Modes", WINDOW_WIDTH, WINDOW_HEIGHT };

    auto monaLisa  = ResourceManager::loadImage( "assets/textures/Mona_Lisa.jpg" );
    Sprite      sprite { monaLisa, BlendMode::AlphaBlend };
    Transform2D transform;

    // Rotate around the center of the screen.
    transform.setAnchor( { monaLisa->getWidth() / 2.0f, monaLisa->getHeight() / 2.0f } );
    transform.setPosition( { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f } );

    transform.scale( { 0.1f, 0.1f } );

    // Image to render to.
    Image image { WINDOW_WIDTH, WINDOW_HEIGHT };

    window.show();

    Timer    timer;
    double   totalTime  = 0.0;
    uint64_t frameCount = 0ull;
    std::string fps        = "FPS: 0";

    while ( window )
    {
        image.clear( Color::Black );

        // Rotate the sprite.
        transform.rotate( timer.elapsedSeconds() );
        image.drawSprite( sprite, transform );

        // Draw a semi-transparent yellow AABB.
        float      alpha = ( static_cast<float>( std::sin( timer.totalSeconds() ) + 1.0f ) / 2.0f );
        const AABB aabb { { WINDOW_WIDTH * 0.25f, WINDOW_HEIGHT * 0.25f, 0.0f }, { WINDOW_WIDTH * 0.75f, WINDOW_HEIGHT * 0.75f, 0.0f } };
        image.drawAABB( aabb, Color::Yellow.withAlpha( alpha ), BlendMode::AlphaBlend );
        // Draw an outline.
        image.drawAABB( aabb, Color::Blue.withAlpha( alpha ), BlendMode::AlphaBlend, FillMode::WireFrame );

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
            fps = std::format( "FPS: {:.3f}", static_cast<double>(frameCount) / totalTime );

            std::cout << fps << std::endl;

            frameCount = 0;
            totalTime  = 0.0;
        }
    }
}