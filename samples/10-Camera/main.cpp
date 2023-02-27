#include <Font.hpp>
#include <Image.hpp>
#include <Input.hpp>
#include <Timer.hpp>
#include <Window.hpp>

#include <Math/Camera2D.hpp>

#include <iostream>

using namespace sr;
using namespace Math;

std::vector<Vertex> transformVerts( const Camera2D& camera, std::span<const Vertex> verts )
{
    std::vector<Vertex> transformedVerts { verts.begin(), verts.end() };
    std::ranges::for_each( transformedVerts, [&camera]( Vertex& v ) { v.position = camera * v.position; } );
    return transformedVerts;
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

    constexpr int WINDOW_WIDTH  = 800;
    constexpr int WINDOW_HEIGHT = 600;

    constexpr glm::vec2 WINDOW_CENTER { WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f };

    Window   window { L"10 - Camera", WINDOW_WIDTH, WINDOW_HEIGHT };
    Image    image { WINDOW_WIDTH, WINDOW_HEIGHT };
    Camera2D camera;

    window.show();

    Timer       timer;
    double      totalTime  = 0.0;
    uint64_t    frameCount = 0ull;
    std::string fps        = "FPS: 0";

    const Vertex verts[] = {
        { { 0, 0 }, { -4, -3 } },                       // Top-left.
        { { WINDOW_WIDTH, 0 }, { 4, -3 } },             // Top-right.
        { { WINDOW_WIDTH, WINDOW_HEIGHT }, { 4, 3 } },  // Bottom-right.
        { { 0, WINDOW_HEIGHT }, { -4, 3 } }             // Bottom-left.
    };

    // Load a smiley face image.
    Image smiley( "assets/textures/Smiley.png" );

    while ( window )
    {
        timer.tick();
        Input::update();

        const float elapsedTime = static_cast<float>( timer.elapsedSeconds() );

        camera.translate( glm::vec2 { Input::getAxis( "Horizontal" ), Input::getAxis( "Vertical" ) } * elapsedTime * 100.0f );
        // Keep the origin in the center of the screen (relative to the position of the camera).
        camera.setOrigin( camera.getPosition() + WINDOW_CENTER );
        camera.rotate( Input::getAxis( "Rotate" ) * elapsedTime );
        camera.zoom( Input::getAxis( "Zoom" ) * elapsedTime );

        image.clear( Color::Black );

        // Transform the quad.
        auto newVerts = transformVerts( camera, verts );

        // Draw smiley faces.
        image.drawQuad( newVerts[0], newVerts[1], newVerts[2], newVerts[3], smiley );

        // Draw a crosshair in the center of the screen.
        image.drawCircle( WINDOW_CENTER, 10.0f, Color::Yellow, {}, FillMode::WireFrame );
        image.drawLine( WINDOW_CENTER - glm::vec2 { 10, 0 }, WINDOW_CENTER + glm::vec2 { 10, 0 }, Color::Red );
        image.drawLine( WINDOW_CENTER - glm::vec2 { 0, 10 }, WINDOW_CENTER + glm::vec2 { 0, 10 }, Color::Green );

        image.drawText( Font::Default, 10, 10, fps, Color::White );

        float zoom     = camera.getZoom();
        float rotation = camera.getRotation();
        auto  position = camera.getPosition();
        auto  origin   = camera.getOrigin();

        image.drawText( Font::Default, 10, 30, std::format( "Zoom    : {}", zoom ), Color::White );
        image.drawText( Font::Default, 10, 45, std::format( "Rotation: {}", rotation ), Color::White );
        image.drawText( Font::Default, 10, 60, std::format( "Position: {}, {}", position.x, position.y ), Color::White );
        image.drawText( Font::Default, 10, 75, std::format( "Origin  : {}, {}", origin.x, origin.y ), Color::White );

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