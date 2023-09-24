#include <Graphics/Font.hpp>
#include <Graphics/Image.hpp>
#include <Graphics/Model.hpp>
#include <Graphics/Timer.hpp>
#include <Graphics/Window.hpp>

#include <fmt/core.h>
#include <glm/ext/matrix_projection.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

using namespace Graphics;
using namespace Math;

struct VertexOut
{
    glm::vec3 position;     // Vertex position in screen space.
    uint16_t  instanceId;   // Object instance ID.
    uint16_t  primitiveId;  // Triangle primitive ID.
};

struct VertexShader
{
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::vec4 viewport;

    uint16_t instanceId;

    VertexOut operator()( const Vertex3D& in, uint16_t primitiveId ) const
    {
        VertexOut out {};

        out.position    = glm::project( in.position, viewMatrix * modelMatrix, projectionMatrix, viewport );
        out.instanceId  = instanceId;
        out.primitiveId = primitiveId;

        return out;
    }
};

struct FragmentShader
{
    Color operator()( const VertexOut& in ) const
    {
        return { in.instanceId, in.primitiveId };
    }
};

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

    // Setup matrices.
    glm::vec4 viewport { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    glm::mat4 projectionMatrix = glm::perspectiveFov( glm::radians( 60.0f ), static_cast<float>( WINDOW_WIDTH ), static_cast<float>( WINDOW_HEIGHT ), 0.1f, 100.0f );
    glm::mat4 viewMatrix       = glm::lookAt( glm::vec3 { 0, 0, 10 }, glm::vec3 { 0, 0, 0 }, glm::vec3 { 0, 1, 0 } );

    // Setup vertex shader.
    VertexShader vertexShader {};
    vertexShader.viewMatrix       = viewMatrix;
    vertexShader.projectionMatrix = projectionMatrix;
    vertexShader.viewport         = viewport;

    // Setup fragment shader.
    FragmentShader fragmentShader {};

    Window window { L"11 - Cube", WINDOW_WIDTH, WINDOW_HEIGHT };
    Image  colorBuffer { WINDOW_WIDTH, WINDOW_HEIGHT };
    Image  depthBuffer { WINDOW_WIDTH, WINDOW_HEIGHT };
    Image  visibilityBuffer { WINDOW_WIDTH, WINDOW_HEIGHT };
    Model  cube { "assets/models/cube.obj" };

    window.show();

    Timer       timer;
    double      totalTime  = 0.0;
    uint64_t    frameCount = 0ull;
    std::string fps        = "FPS: 0";

    float angle = 0.0f;

    std::vector<VertexOut> transformedVerts;

    while ( window )
    {
        colorBuffer.clear( Color::White );
        visibilityBuffer.clear( { 0xffffu, 0xffffu } );
        depthBuffer.clear( Color { 1.0f } );
        transformedVerts.clear();

        // Draw the cube.
        angle += static_cast<float>( timer.elapsedSeconds() * 15.0 );  // Rotate the cube.
        vertexShader.modelMatrix = glm::rotate( glm::radians( angle ), glm::vec3 { 0, 1, 0 } );
        vertexShader.instanceId  = 0;
        for ( const auto& mesh: cube.getMeshes() )
        {
            for ( uint16_t vertexId = 0; const auto& v: mesh->getVertices() )
            {
                auto vout = vertexShader( v, vertexId / 3 );
                transformedVerts.emplace_back( vout );
                ++vertexId;
            }
            vertexShader.instanceId++;
        }

        assert( transformedVerts.size() % 3 == 0 );  // Must be triangles.
        for ( size_t i = 0; i < transformedVerts.size(); i += 3 )
        {
            auto v0 = transformedVerts[i + 0];
            auto v1 = transformedVerts[i + 1];
            auto v2 = transformedVerts[i + 2];

            auto aabb = AABB::fromTriangle( v0.position, v1.position, v2.position );
            // Clamp the triangle AABB to the AABB of the visibility buffer.
            aabb.clamp( visibilityBuffer.getAABB() );

            for ( int y = static_cast<int>( aabb.min.y ); y <= static_cast<int>( aabb.max.y ); ++y )
            {
                for ( int x = static_cast<int>( aabb.min.x ); x <= static_cast<int>( aabb.max.x ); ++x )
                {
                    auto b = barycentric( v0.position, v1.position, v2.position, { x, y } );
                    if ( barycentricInside( b ) )
                    {
                        // Compute depth
                        float z     = v0.position.z * b.x + v1.position.z * b.y + v2.position.z * b.z;
                        float depth = depthBuffer( x, y ).depth;
                        if ( z < depth )
                        {
                            visibilityBuffer( x, y ) = Color { v2.primitiveId, v2.instanceId };
                            depthBuffer( x, y )      = Color { z };
                        }
                    }
                }
            }
        }

        // Visualize the triangle IDs
        for ( int y = 0; y < WINDOW_HEIGHT; ++y )
        {
            for ( int x = 0; x < WINDOW_WIDTH; ++x )
            {
                if ( auto c = visibilityBuffer( x, y ); c.primitiveId < 0xffff )
                    colorBuffer( x, y ) = c;
            }
        }

        colorBuffer.drawText( Font::Default, fps, 10, 10, Color::Black );

        window.present( colorBuffer );

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
                case KeyCode::V:
                    window.toggleVSync();
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
            fps = fmt::format( "FPS: {:.3f}", static_cast<double>( frameCount ) / totalTime );

            std::cout << fps << std::endl;

            frameCount = 0;
            totalTime  = 0.0;
        }
    }
}