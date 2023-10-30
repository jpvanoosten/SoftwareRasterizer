#include "Graphics/Keyboard.hpp"
#include "Graphics/Mouse.hpp"

#include <CameraController.hpp>

#include <Graphics/Font.hpp>
#include <Graphics/Image.hpp>
#include <Graphics/Input.hpp>
#include <Graphics/Model.hpp>
#include <Graphics/Timer.hpp>
#include <Graphics/Window.hpp>
#include <Math/Camera3D.hpp>
#include <Math/Plane.hpp>

#include <glm/gtc/random.hpp>
#include <glm/gtx/transform.hpp>

#include <fmt/core.h>
#include <iostream>

using namespace Graphics;
using namespace Math;

struct VertexOut
{
    glm::vec4 position;     // Vertex position in screen space.
    glm::vec2 uv;           // Vertex texture coordinates.
    uint16_t  instanceId;   // Object instance ID.
    uint16_t  primitiveId;  // Triangle primitive ID.
};

struct VertexShader
{
    glm::mat4 modelViewProjectionMatrix;
    glm::vec4 viewport;

    uint16_t instanceId;

    VertexOut operator()( const glm::vec3& position, const glm::vec2& uv, uint16_t vertexId ) const
    {
        VertexOut out {};

        out.position    = modelViewProjectionMatrix * glm::vec4 { position, 1 };
        out.uv          = uv;
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
    Mesh*     mesh;
    glm::mat4 modelMatrix;
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

    CameraController camera { { 0, 3, 0 }, 0.0f, 90.0f };
    camera.setPerspective( 60.0f, static_cast<float>( WINDOW_WIDTH ) / WINDOW_HEIGHT, 0.1f, 100.0f );

    // Setup vertex shader.
    VertexShader vertexShader {};
    vertexShader.viewport = viewport;

    // Setup fragment shader.
    FragmentShader fragmentShader {};

    // Setup some random colors for debugging.
    auto randomColors = GenerateColors( 0xffff );

    Window window { "11 - Cube", WINDOW_WIDTH, WINDOW_HEIGHT };
    Image  colorBuffer { WINDOW_WIDTH, WINDOW_HEIGHT };
    Image  depthBuffer { WINDOW_WIDTH, WINDOW_HEIGHT };
    Image  visibilityBuffer { WINDOW_WIDTH, WINDOW_HEIGHT };
    Image  textureCoordinates { WINDOW_WIDTH, WINDOW_HEIGHT };

    Model cube { "assets/models/sponza.obj" };

    window.show();
    window.setFullscreen( true );

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

        camera.update( static_cast<float>( timer.elapsedSeconds() ) );

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
            instanceBuffer.emplace_back( mesh.get(), modelMatrix );
            auto indices   = mesh->getIndices().data();
            auto positions = mesh->getPositions().data();
            auto texCoords = mesh->getTexCoords().data();

            for ( size_t i = 0; i < mesh->getNumIndices(); ++i )
            {
                auto vertexId = static_cast<uint16_t>( indices[i] );
                auto p        = positions[vertexId];
                auto uv       = texCoords[vertexId];

                auto vout = vertexShader( p, uv, vertexId );
                transformedVerts.emplace_back( vout );
            }
            vertexShader.instanceId++;
        }

        assert( transformedVerts.size() % 3 == 0 );  // Must be triangles.

        for ( int i = 0; i < transformedVerts.size(); i += 3 )
        {
            auto v0 = transformedVerts[i + 0];
            auto v1 = transformedVerts[i + 1];
            auto v2 = transformedVerts[i + 2];

            // TODO: Better clipping.
            if ( v0.position.z < -v0.position.w || v1.position.z < -v1.position.w || v2.position.z < -v2.position.w )
                continue;

            // Store the w component before perspective divide.
            float w0 = 1.0f / v0.position.w;
            float w1 = 1.0f / v1.position.w;
            float w2 = 1.0f / v2.position.w;

            for ( auto v: { std::ref( v0 ), std::ref( v1 ), std::ref( v2 ) } )
            {
                auto& pos = v.get().position;
                pos       = pos / pos.w;  // Perspective divide.

                // NDC -> screen space
                pos   = pos * 0.5f + 0.5f;
                pos.x = pos.x * viewport.z + viewport.x;
                pos.y = ( 1.0f - pos.y ) * viewport.w + viewport.y;  // Flip Y
            }

            // Backface culling.
            // Compute the area of the triangle in screen space.
            // Source: OpenGL 4.6 Specification, 2022 (pp. 477)
            auto a = -( ( v0.position.x * v1.position.y - v1.position.x * v0.position.y ) + ( v1.position.x * v2.position.y - v2.position.x * v1.position.y ) + ( v2.position.x * v0.position.y - v0.position.x * v2.position.y ) );
            if ( a < 0.0f )
                continue;

            auto aabb = AABB::fromTriangle( v0.position, v1.position, v2.position );
            // Clamp the triangle AABB to the AABB of the visibility buffer.
            aabb.clamp( visibilityBuffer.getAABB() );
            if ( aabb.min.x > aabb.max.x || aabb.min.y > aabb.max.y || aabb.min.z > 1.0f || aabb.max.z < 0.0f )
                continue;

            for ( int y = static_cast<int>( aabb.min.y ); y <= static_cast<int>( aabb.max.y ); ++y )
            {
                for ( int x = static_cast<int>( aabb.min.x ); x <= static_cast<int>( aabb.max.x ); ++x )
                {
                    // Barycentric coordinates in screen space.
                    auto bc = barycentric( v0.position, v1.position, v2.position, { static_cast<float>( x ) + 0.5f, static_cast<float>( y ) + 0.5f } );
                    if ( barycentricInside( bc ) )
                    {
                        // Compute depth
                        float  z = v0.position.z * bc.x + v1.position.z * bc.y + v2.position.z * bc.z;
                        Color& d = depthBuffer( x, y );
                        if ( z < d.depth )
                        {
                            bc = bc * glm::vec3 { w0, w1, w2 };
                            // Compute the perspective correct UV coordinates.
                            // Source: OpenGL 4.6 Specification, 2022 (pp. 479).
                            float correction = 1.0f / ( bc.x + bc.y + bc.z );
                            auto  uv         = ( v0.uv * bc.x + v1.uv * bc.y + v2.uv * bc.z ) * correction;

                            visibilityBuffer( x, y )   = Color { v2.instanceId, v2.primitiveId };
                            textureCoordinates( x, y ) = Color { static_cast<uint16_t>( uv.x * 65535.0f ), static_cast<uint16_t>( uv.y * 65535.0f ) };
                            d.depth                    = z;
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

                if ( !diffuseTexture )
                    continue;

                auto textureCoords = mesh->getTexCoords().data();

                auto t0 = textureCoords[c.primitiveId * 3 + 0];
                auto t1 = textureCoords[c.primitiveId * 3 + 1];
                auto t2 = textureCoords[c.primitiveId * 3 + 2];

                // Unpack uv coords.
                Color       uv = textureCoordinates[i];
                const float u  = static_cast<float>( uv.u ) / 65535.0f;
                const float v  = static_cast<float>( uv.v ) / 65535.0f;
                // Sample diffuse texture.
                colorBuffer[i] = mat->diffuseTexture->sample( glm::vec2 { u, v } );
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