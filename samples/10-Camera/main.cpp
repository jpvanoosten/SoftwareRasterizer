#include <Font.hpp>
#include <Image.hpp>
#include <Input.hpp>
#include <Timer.hpp>
#include <Window.hpp>

#include <Math/Camera2D.hpp>

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

    // Input that controls the horizontal movement of the camera.
    Input::mapAxis( "Horizontal", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        float leftX = 0.0f;

        for ( auto& gamePadState: gamePadStates )
        {
            const auto state = gamePadState.getLastState();

            leftX += state.thumbSticks.leftX;
        }

        const auto keyState = keyboardState.getLastState();

        const float a = keyState.A ? 1.0f : 0.0f;
        const float d = keyState.D ? 1.0f : 0.0f;

        return std::clamp( leftX - a + d, -1.0f, 1.0f );
    } );

    // Input that controls the vertical movement of the camera.
    Input::mapAxis( "Vertical", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        float leftY = 0.0f;

        for ( auto& gamePadState: gamePadStates )
        {
            const auto state = gamePadState.getLastState();

            leftY += state.thumbSticks.leftY;
        }

        const auto keyState = keyboardState.getLastState();

        const float w = keyState.W ? 1.0f : 0.0f;
        const float s = keyState.S ? 1.0f : 0.0f;

        return std::clamp( -leftY - w + s, -1.0f, 1.0f );
    } );

    // Input that controls the Zooming of the camera.
    Input::mapAxis( "Zoom", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        float rightY = 0.0f;

        for ( auto& gamePadState: gamePadStates )
        {
            const auto state = gamePadState.getLastState();

            rightY += state.thumbSticks.rightY;
        }

        const auto keyState = keyboardState.getLastState();

        const float up   = keyState.Up ? 1.0f : 0.0f;
        const float down = keyState.Down ? 1.0f : 0.0f;

        return std::clamp( rightY + up - down, -1.0f, 1.0f );
    } );

    // Input that controls the rotation of the camera.
    Input::mapAxis( "Rotate", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        float rightX = 0.0f;

        for ( auto& gamePadState: gamePadStates )
        {
            const auto state = gamePadState.getLastState();

            rightX += state.thumbSticks.rightX;
        }

        const auto keyState = keyboardState.getLastState();

        const float left  = keyState.Left ? 1.0f : 0.0f;
        const float right = keyState.Right ? 1.0f : 0.0f;

        return std::clamp( rightX - left + right, -1.0f, 1.0f );
    } );

    const int WINDOW_WIDTH  = 800;
    const int WINDOW_HEIGHT = 600;

    Window window { L"10 - Camera", WINDOW_WIDTH, WINDOW_HEIGHT };
    Image  image { WINDOW_WIDTH, WINDOW_HEIGHT };
    Camera2D camera;
    camera.setOrigin( { WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f } );

    window.show();

    Timer       timer;
    double      totalTime  = 0.0;
    uint64_t    frameCount = 0ull;
    std::string fps        = "FPS: 0";

    constexpr glm::vec2 verts[] = {
        { WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.25 },
        { WINDOW_WIDTH * 0.25f, WINDOW_HEIGHT * 0.75f },
        { WINDOW_WIDTH * 0.75f, WINDOW_HEIGHT * 0.75f },
    };

    while ( window )
    {
        timer.tick();
        Input::update();

        const float elapsedTime = static_cast<float>( timer.elapsedSeconds() );

        camera.translate( glm::vec2{ Input::getAxis( "Horizontal" ), Input::getAxis( "Vertical" ) } * elapsedTime * 100.0f );
        camera.rotate( Input::getAxis( "Rotate" ) * elapsedTime );
        camera.zoom( Input::getAxis( "Zoom" ) * elapsedTime );

        image.clear( Color::White );

        // Transform the triangle.
        glm::vec2 p0 = camera.transformPoint( verts[0] );
        glm::vec2 p1 = camera.transformPoint( verts[1] );
        glm::vec2 p2 = camera.transformPoint( verts[2] );

        // Draw a red triangle in the middle of the screen.
        image.drawTriangle( p0, p1, p2, Color::Red );
        // Draw a blue outline for the triangle.
        image.drawTriangle( p0, p1, p2, Color::Blue, {}, FillMode::WireFrame );

        image.drawText( Font::Default, 10, 10, fps, Color::Black );

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