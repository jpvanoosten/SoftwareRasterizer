#include "tinyxml2.h"

#include <Game.hpp>
#include <Timer.hpp>
#include <Window.hpp>

#include <iostream>

// Parse the XML file and return a list of rects containing the sprites in the texture atlas.
std::vector<Math::RectI> parseSpriteRects( const std::filesystem::path& xmlFile )
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile( xmlFile.string().c_str() );

    tinyxml2::XMLElement* root = doc.RootElement();

    std::vector<Math::RectI> rects;

    for ( tinyxml2::XMLElement* subTexture = root->FirstChildElement( "SubTexture" ); subTexture != nullptr; subTexture = subTexture->NextSiblingElement( "SubTexture" ) )
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

    constexpr int WINDOW_WIDTH  = 1920;
    constexpr int WINDOW_HEIGHT = 1080;

    Game game { WINDOW_WIDTH, WINDOW_HEIGHT };

    sr::Window window { L"09 - Breakout", WINDOW_WIDTH, WINDOW_HEIGHT };
    window.show();
    window.setFullscreen( true );

    sr::Timer timer;

    while ( window )
    {
        timer.tick();

        game.update( timer.elapsedSeconds() );

        window.clear( sr::Color::Black );
        window.present( game.getImage() );

        sr::Event e;
        while ( window.popEvent( e ) )
        {
            switch ( e.type )
            {
            case sr::Event::Close:
                window.destroy();
                break;
            case sr::Event::KeyPressed:
                switch ( e.key.code )
                {
                case sr::KeyCode::V:
                    window.setVSync( !window.isVSync() );
                    std::cout << "Vsync: " << window.isVSync() << std::endl;
                    break;
                case sr::KeyCode::Enter:
                    if ( e.key.alt )
                    {
                        [[fallthrough]];
                    case sr::KeyCode::F11:
                        window.toggleFullscreen();
                    }
                    break;
                case sr::KeyCode::Escape:
                    window.destroy();
                    break;
                }
                break;
            }
        }
    }
}