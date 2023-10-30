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
        VertexOutput tri[3];
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

        // TODO: Clip triangle.
    }
}

int Rasterizer::clipTriangle( const VertexOutput* in, int n_in, VertexOutput* out, const Math::Plane& plane )
{
    // Number of output vertices.
    int n_out = 0;

    // Start vertex.
    VertexOutput S = in[n_in - 1];

    for ( int i = 0; i < n_in; ++i )
    {
        // Terminal vertex.
        VertexOutput P = in[i];

        // Compute the signed distance to the plane.
        float dS = plane.distance( S.position );
        float dP = plane.distance( P.position );

        if ( dP >= 0.0f )
        {
            if ( dS >= 0.0f )
            {
                // Case 1. Both p and s are on visible side of plane.
                // If the edge is entirely on the visible side if the clipping plane,
                // only its terminal vertex P, need be output since its initial vertex S
                // will already have been output.

                out[n_out++] = P;
            }
            else
            {
                // Case 4. Edge enters visible region (p is visible, s is not).
                // If the edge enters the visible side of the clipping plane with P visible
                // and S not, then two output vertices must be generated: the intersection
                // of the edge and the clipping plane, followed by the terminal vertex P.

                float a = dS / ( dS - dP );

                out[n_out++] = a * S + ( 1.0f - a ) * P;
                out[n_out++] = P;
            }
        }
        else if ( dS >= 0.0f )
        {
            // Case 3. Edge leaves visible region (s is visible, p is not).
            // If the edge leaves the visible side of the clipping plane, with S
            // visible and P not, then only the intersection of the edge and the clipping
            // plane need be output because the initial vertex S, will already have
            // been output.

            float a = dS / ( dS - dP );

            out[n_out++] = a * S + ( 1.0f - a ) * P;
        }

        // Next edge...
        S = P;
    }

    return n_out;
}

int Rasterizer::clipTriangle( const VertexOutput* in, VertexOutput* out )
{

}
