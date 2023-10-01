#include <Graphics/Font.hpp>
#include <Graphics/Image.hpp>
#include <Graphics/Input.hpp>
#include <Graphics/Model.hpp>
#include <Graphics/Timer.hpp>
#include <Graphics/Window.hpp>
#include <Math/Camera3D.hpp>

#include <glm/gtc/random.hpp>
#include <glm/gtx/transform.hpp>

#include <fmt/core.h>
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
    glm::mat4 modelViewProjectionMatrix;
    glm::vec4 viewport;

    uint16_t instanceId;

    VertexOut operator()( const glm::vec3& position, uint16_t vertexId ) const
    {
        VertexOut out {};

        glm::vec4 pos = modelViewProjectionMatrix * glm::vec4 { position, 1 };
        pos           = pos / pos.w;  // Perspective divide.

        // NDC -> screen space
        pos   = pos * 0.5f + 0.5f;
        pos.x = pos.x * viewport.z + viewport.x;
        pos.y = pos.y * viewport.w + viewport.y;

        out.position    = pos;
        out.instanceId  = instanceId;
        out.primitiveId = vertexId / 3;

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

struct Instance
{
    std::shared_ptr<Mesh> mesh;
    glm::mat4             modelMatrix;
};

// Generate some random colors for debugging.
std::vector<Color> GenerateColors( int numColors )
{
    std::vector<Color> colors( numColors );

    for ( auto& color: colors )
    {
        color = Color::fromHSV( glm::linearRand( 0.0f, 360.0f ), glm::linearRand( 0.0f, 1.0f ), 1.0f );
    }

    return colors;
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

    const int WINDOW_WIDTH  = 800;
    const int WINDOW_HEIGHT = 600;

    // Setup matrices.
    glm::vec4 viewport { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    Camera camera;
    camera.setProjection( glm::radians( 60.0f ), static_cast<float>( WINDOW_WIDTH ) / WINDOW_HEIGHT, 1.0f, 100.0f );
    camera.lookAt( { 0, 50, 25 }, { 0, 0, 0 }, { 0, -1, 0 } );

    // Setup vertex shader.
    VertexShader vertexShader {};
    vertexShader.viewport = viewport;

    // Setup fragment shader.
    FragmentShader fragmentShader {};

    // Setup some random colors for debugging.
    auto randomColors = GenerateColors( 0xffff );

    Window window { L"11 - Cube", WINDOW_WIDTH, WINDOW_HEIGHT };
    Image  colorBuffer { WINDOW_WIDTH, WINDOW_HEIGHT };
    Image  depthBuffer { WINDOW_WIDTH, WINDOW_HEIGHT };
    Image  visibilityBuffer { WINDOW_WIDTH, WINDOW_HEIGHT };
    Image  barycentricCoords { WINDOW_WIDTH, WINDOW_HEIGHT };

    Model cube { "assets/models/sponza.obj" };

    window.show();

    Timer       timer;
    double      totalTime  = 0.0;
    uint64_t    frameCount = 0ull;
    std::string fps        = "FPS: 0";

    float angle = 90.0f;

    std::vector<VertexOut> transformedVerts;
    std::vector<Instance>  instanceBuffer;

    while ( window )
    {
        timer.tick();
        Input::update();

        camera.translate( glm::vec3 { Input::getAxis( "Horizontal" ), 0, -Input::getAxis( "Vertical" ) } * static_cast<float>( timer.elapsedSeconds() ) * 5.0f );
        if ( Input::getMouseButton( MouseButton::Right ) )
        {
            camera.rotateX( glm::radians( -Input::getAxis( "Mouse Y" ) * timer.elapsedSeconds() ) );
            camera.rotateY( glm::radians( Input::getAxis( "Mouse X" ) * timer.elapsedSeconds() ) );
        }
        float roll = Input::getKey( "q" ) ? -1.0f : 0.0f;
        roll += Input::getKey( "e" ) ? 1.0f : 0.0f;

        camera.rotateZ( glm::radians( roll ) * timer.elapsedSeconds() * 15.0f );

        colorBuffer.clear( Color::Black );
        visibilityBuffer.clear( { 0xffffu, 0xffffu } );
        instanceBuffer.clear();
        depthBuffer.clear( Color { 1.0f } );
        transformedVerts.clear();

        const glm::mat4 modelMatrix            = glm::scale( glm::vec3 { 0.01f } );
        vertexShader.modelViewProjectionMatrix = camera.getViewProjectionMatrix() * modelMatrix;
        vertexShader.instanceId                = 0;
        for ( const auto& mesh: cube.getMeshes() )
        {
            instanceBuffer.emplace_back( mesh, modelMatrix );
            for ( uint16_t vertexId = 0; const auto& p: mesh->getPositions() )
            {
                auto vout = vertexShader( p, vertexId );
                transformedVerts.emplace_back( vout );
                ++vertexId;
            }
            vertexShader.instanceId++;
        }

        assert( transformedVerts.size() % 3 == 0 );  // Must be triangles.

        for ( int i = 0; i < transformedVerts.size(); i += 3 )
        {
            auto v0 = transformedVerts[i + 0];
            auto v1 = transformedVerts[i + 1];
            auto v2 = transformedVerts[i + 2];

            auto aabb = AABB::fromTriangle( v0.position, v1.position, v2.position );
            // Clamp the triangle AABB to the AABB of the visibility buffer.
            aabb.clamp( visibilityBuffer.getAABB() );
            if ( aabb.min.x > aabb.max.x || aabb.min.y > aabb.max.y || aabb.min.z > 1.0f || aabb.max.z < 0.0f )
                continue;

            for ( int y = static_cast<int>( aabb.min.y ); y <= static_cast<int>( aabb.max.y ); ++y )
            {
                for ( int x = static_cast<int>( aabb.min.x ); x <= static_cast<int>( aabb.max.x ); ++x )
                {
                    auto b = barycentric( v0.position, v1.position, v2.position, { x, y } );
                    if ( barycentricInside( b ) )
                    {
                        // Compute depth
                        float  z = v0.position.z * b.x + v1.position.z * b.y + v2.position.z * b.z;
                        Color& d = depthBuffer( x, y );
                        if ( z > 0.0f && z < 1.0f && z < d.depth )
                        {
                            visibilityBuffer( x, y )  = Color { v2.instanceId, v2.primitiveId };
                            barycentricCoords( x, y ) = Color { static_cast<uint16_t>( b.x * 65535.0f ), static_cast<uint16_t>( b.y * 65535.0f ) };
                            d.depth                   = z;
                        }
                    }
                }
            }
        }

        // TODO: Cluster by instance ID/sort by primitive ID.
        for ( int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; ++i )
        {
            if ( auto c = visibilityBuffer[i]; c.primitiveId < 0xffff )
            {
                auto mesh           = instanceBuffer[c.instanceId].mesh;
                auto mat            = mesh->getMaterial();
                auto diffuseTexture = mat->diffuseTexture;

                auto textureCoords = mesh->getTexCoords().data();

                auto t0 = textureCoords[c.primitiveId * 3 + 0];
                auto t1 = textureCoords[c.primitiveId * 3 + 1];
                auto t2 = textureCoords[c.primitiveId * 3 + 2];

                // Unpack barycentric coords.
                Color       bc = barycentricCoords[i];
                const float u  = static_cast<float>( bc.u ) / 65535.0f;
                const float v  = static_cast<float>( bc.v ) / 65535.0f;
                const float w  = 1.0f - u - v;
                // Compute UV
                const glm::vec2 uv = t0 * u + t1 * v + t2 * w;
                // Sample diffuse texture.
                colorBuffer[i] = mat->diffuseTexture->sample( uv );
            }
        }

        colorBuffer.drawText( Font::Default, fps, 10, 10, Color::White );

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
                case KeyCode::R:
                    camera.lookAt( { 0, 50, 25 }, { 0, 0, 0 }, { 0, -1, 0 } );
                    break;
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