#include <CameraController.hpp>

#include <Graphics/Font.hpp>
#include <Graphics/Image.hpp>
#include <Graphics/Input.hpp>
#include <Graphics/Model.hpp>
#include <Graphics/Rasterizer.hpp>
#include <Graphics/Timer.hpp>
#include <Graphics/Window.hpp>

#include <Math/Camera3D.hpp>

#include <glm/gtc/random.hpp>
#include <glm/gtx/transform.hpp>

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

    const int WINDOW_WIDTH  = 1920;
    const int WINDOW_HEIGHT = 1080;

    Viewport viewport { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    Image    image { WINDOW_WIDTH, WINDOW_HEIGHT };

    CameraController camera { { 0, 3, 0 }, 0.0f, 90.0f };
    camera.setPerspective( 60.0f, static_cast<float>( WINDOW_WIDTH ) / WINDOW_HEIGHT, 0.1f, 100.0f );

    Rasterizer rasterizer( WINDOW_WIDTH, WINDOW_HEIGHT );
    rasterizer.setCamera( &camera.getCamera() );
    rasterizer.setViewport( viewport );

    Model model { "assets/models/sponza.obj" };

    Window window { "11 - Rasterizer", WINDOW_WIDTH, WINDOW_HEIGHT };

    window.show();
    window.setFullscreen( true );

    Timer       timer;
    double      totalTime  = 0.0;
    uint64_t    frameCount = 0ull;
    std::string fps        = "FPS: 0";

    float angle = 90.0f;

    while ( window )
    {
        timer.tick();
        Input::update();

        camera.update( static_cast<float>( timer.elapsedSeconds() ) );

        rasterizer.clear( Color::Black, 1.0f );

        const glm::mat4 modelMatrix = glm::scale( glm::vec3 { 0.01f } );

        for ( const auto& mesh: model.getMeshes() )
        {
            rasterizer.draw( *mesh, modelMatrix );
        }

        image.copy( rasterizer.getImage() );
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
                case KeyCode::R:
                    camera.reset();
                    break;
                case KeyCode::Escape:
                    window.destroy();
                    break;
                case KeyCode::V:
                    window.toggleVSync();
                    break;
                case KeyCode::F11:
                    window.toggleFullscreen();
                    break;
                }
                break;
            }
        }

        ++frameCount;

        totalTime += timer.elapsedSeconds();
        if ( totalTime > 1.0 )
        {
            fps = fmt::format( "FPS: {:.3f}", static_cast<double>( frameCount ) / totalTime );

            std::cout << fps << std::endl;

            frameCount = 0;
            totalTime  = 0.0;
        }
    }
}