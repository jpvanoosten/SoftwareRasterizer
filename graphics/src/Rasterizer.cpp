#include <Graphics/Rasterizer.hpp>

using namespace Graphics;

Rasterizer::Rasterizer() = default;

Rasterizer::Rasterizer( std::size_t width, std::size_t height )
: width { width }
, height { height }
, renderTarget { static_cast<uint32_t>( width ), static_cast<uint32_t>( height ) }
, depthBuffer { width, height }
, viewport { 0.0f, 0.0f, static_cast<float>( width ), static_cast<float>( height ) }
{}

void Rasterizer::clear( const Color& color, float depth )
{
    renderTarget.clear( color );
    depthBuffer.clear( depth );
}

void Rasterizer::draw( const Mesh& mesh, const glm::mat4& modelMatrix )
{
    triangleAssembly( mesh, modelMatrix );
}

void Rasterizer::setCamera( const Math::Camera* _camera ) noexcept
{
    camera = _camera;
}

void Rasterizer::setViewport( const Math::Viewport& _viewport ) noexcept
{
    viewport = _viewport;
}

const Image& Rasterizer::getImage() const noexcept
{
    return renderTarget;
}

const Buffer<float>& Rasterizer::getDepthBuffer() const noexcept
{
    return depthBuffer;
}

inline Rasterizer::VertexOutput Rasterizer::vertexShader( const VertexInput& in, const glm::mat4& modelMatrix, const glm::mat4& modelViewProjectionMatrix )
{
    VertexOutput out {};

    out.position = modelViewProjectionMatrix * glm::vec4 { in.position, 1.0f };
    out.normal   = modelMatrix * glm::vec4 { in.normal, 0.0f };
    out.uv       = in.uv;
    out.color    = in.color;

    return out;
}

void Rasterizer::triangleAssembly( const Mesh& mesh, const glm::mat4& modelMatrix )
{
    glm::mat4 modelViewProjectionMatrix = modelMatrix;
    if ( camera )
    {
        modelViewProjectionMatrix = camera->getViewProjectionMatrix() * modelViewProjectionMatrix;
    }

    // Mesh must be triangulated.
    // TODO: Topology?
    assert( mesh.getNumIndices() % 3 == 0 );
    std::size_t numTris = mesh.getNumIndices() / 3;

    auto* indices = mesh.getIndices().data();

    auto* positions = mesh.getPositions().data();
    auto* normals   = mesh.getNormals().data();
    auto* uvs       = mesh.getTexCoords().data();
    auto* colors    = mesh.getColors().data();

    for ( std::size_t i = 0; i < numTris; ++i )
    {
        Triangle tri;
        for ( std::size_t v = 0; v < 3; ++v )
        {
            int vertexId = indices[i * 3 + v];

            VertexInput in;

            in.position = positions[vertexId];
            in.normal   = normals[vertexId];
            in.uv       = uvs[vertexId];
            in.color    = colors[vertexId];

            tri[v] = vertexShader( in, modelMatrix, modelViewProjectionMatrix );
        }
    }
}

Rasterizer::VertexOutput Rasterizer::getIntersection( const VertexOutput& v0, const VertexOutput& v1, const Math::Plane& plane )
{
    const float t = plane.intersect( v0.position, v1.position );
    return t * v0 + ( 1.0f - t ) * v1;
}

int Rasterizer::clipTriangle( const Triangle& in, const Math::Plane& plane, Triangle out[2] )
{
    int i = 0;
    int n_tris = 0; // Number of resulting triangles.
    int n_in = 0; // Number of vertices inside the plane.

    // TODO: Triangle clipping algorithm.


    return n_tris;
}

int Rasterizer::clipTriangle( const Triangle& in, Triangle out[12] )
{
    return 0;
}
