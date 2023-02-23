#include <Ball.hpp>

#include <Font.hpp>
#include <Image.hpp>
#include <Math/Rect.hpp>
#include <ResourceManager.hpp>
#include <Timer.hpp>
#include <Window.hpp>

#include <tinyxml2.h>

#include <iostream>

using namespace sr;
using namespace Math;
using namespace tinyxml2;

// Parse the XML file and return a list of rects containing the sprites in the texture atlas.
std::vector<RectI> parseSpriteRects( const std::filesystem::path& xmlFile )
{
    XMLDocument doc;
    doc.LoadFile( xmlFile.string().c_str() );

    XMLElement* root = doc.RootElement();

    std::vector<RectI> rects;

    for ( XMLElement* subTexture = root->FirstChildElement( "SubTexture" ); subTexture != nullptr; subTexture = subTexture->NextSiblingElement( "SubTexture" ) )
    {
        int x      = subTexture->IntAttribute( "x" );
        int y      = subTexture->IntAttribute( "y" );
        int width  = subTexture->IntAttribute( "width" );
        int height = subTexture->IntAttribute( "height" );
        rects.emplace_back( x, y, width, height );
    }

    return rects;
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

    const int WINDOW_WIDTH  = 1920;
    const int WINDOW_HEIGHT = 1080;

    Window window { L"09 - Breakout", WINDOW_WIDTH, WINDOW_HEIGHT };

    // Parse the sprite rectangles.
    auto spriteRects = parseSpriteRects( "assets/Breakout/Sprite Sheet/Breakout_Tile_Free.xml" );

    // Load the sprite sheet.
    auto spriteSheet = SpriteSheet( "assets/Breakout/Sprite Sheet/Breakout_Tile_Free.png", spriteRects, BlendMode::AlphaBlend );

    Ball ball { { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f } };
    ball.setVelocity( glm::vec2 { 1, -1 } * 200.0f );

    Image image { WINDOW_WIDTH, WINDOW_HEIGHT };

    window.show();

    Timer       timer;
    double      totalTime  = 0.0;
    uint64_t    frameCount = 0ull;
    std::string fps        = "FPS: 0";

    while ( window )
    {
        timer.tick();

        ball.update( static_cast<float>( timer.elapsedSeconds() ) );
        

        image.resize( window.getWidth(), window.getHeight() );
        image.clear( Color::Black );

        ball.draw( image );

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