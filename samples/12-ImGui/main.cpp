#include <Graphics/Font.hpp>

#include <Graphics/Image.hpp>
#include <Graphics/Timer.hpp>
#include <Graphics/Window.hpp>

#include <fmt/core.h>
#include <iostream>

#include <imgui.h>

using namespace Graphics;

int main( int argc, char* argv[] )
{
    constexpr int WINDOW_WIDTH  = 800;
    constexpr int WINDOW_HEIGHT = 600;

    Window window { "12-ImGui", WINDOW_WIDTH, WINDOW_HEIGHT };

    Image image { WINDOW_WIDTH, WINDOW_HEIGHT };
    constexpr Color clearColor = Color::fromFloats( 0.45f, 0.55f, 0.6f );
    window.setClearColor( clearColor );

    window.show();

    Timer       timer;
    double      totalTime  = 0.0;
    uint64_t    frameCount = 0ull;
    std::string fps        = "FPS: 0";

    while ( window )
    {
        image.clear( clearColor );

        image.drawText( Font::Default, fps, 10, 10, Color::White );

        ImGui::ShowDemoWindow();

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
                std::cout << fmt::format( "Resize: {},{}\n", e.resize.width, e.resize.height );
                image.resize( e.resize.width, e.resize.height );
                break;
            }
        }

        timer.tick();
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