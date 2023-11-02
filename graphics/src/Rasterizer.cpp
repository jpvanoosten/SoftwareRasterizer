#include <Graphics/Rasterizer.hpp>

using namespace Graphics;
using namespace Math;

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
    glm::mat4 modelViewProjectionMatrix = modelMatrix;
    if ( camera )
    {
        modelViewProjectionMatrix = camera->getViewProjectionMatrix() * modelViewProjectionMatrix;
    }

    Material* material       = mesh.getMaterial().get();
    Image*    diffuseTexture = material ? material->diffuseTexture.get() : nullptr;
    Color     diffuseColor   = material ? material->diffuseColor : Color::Magenta;

    AABB viewportAABB = AABB::fromViewport( viewport );

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
        if ( tri[0].position.z < -tri[0].position.w || tri[1].position.z < -tri[1].position.w || tri[2].position.z < -tri[2].position.w )
            continue;

        // Store the w component before perspective divide.
        float w0 = 1.0f / tri[0].position.w;
        float w1 = 1.0f / tri[1].position.w;
        float w2 = 1.0f / tri[2].position.w;

        for ( auto& v: tri )
        {
            auto& pos = v.position;
            pos       = pos / pos.w;  // Perspective divide.

            // NDC -> screen space
            pos   = pos * 0.5f + 0.5f;
            pos.x = pos.x * viewport.width + viewport.x;
            pos.y = ( 1.0f - pos.y ) * viewport.height + viewport.y;  // Flip Y
        }

        // Backface culling.
        // Compute the area of the triangle in screen space.
        // Source: OpenGL 4.6 Specification, 2022 (pp. 477)
        auto a = -( ( tri[0].position.x * tri[1].position.y - tri[1].position.x * tri[0].position.y ) + ( tri[1].position.x * tri[2].position.y - tri[2].position.x * tri[1].position.y ) + ( tri[2].position.x * tri[0].position.y - tri[0].position.x * tri[2].position.y ) );
        if ( a < 0.0f )
            continue;

        auto aabb = Math::AABB::fromTriangle( tri[0].position, tri[1].position, tri[2].position );

        // Clamp the triangle AABB to the AABB of the viewport.
        aabb.clamp( viewportAABB );

        for ( int y = static_cast<int>( aabb.min.y ); y <= static_cast<int>( aabb.max.y ); ++y )
        {
            for ( int x = static_cast<int>( aabb.min.x ); x <= static_cast<int>( aabb.max.x ); ++x )
            {
                // Barycentric coordinates in screen space.
                auto bc = barycentric( tri[0].position, tri[1].position, tri[2].position, { static_cast<float>( x ) + 0.5f, static_cast<float>( y ) + 0.5f } );
                if ( barycentricInside( bc ) )
                {
                    // Compute depth
                    float  z = tri[0].position.z * bc.x + tri[1].position.z * bc.y + tri[2].position.z * bc.z;
                    float& d = depthBuffer( x, y );
                    if ( z < d )
                    {
                        bc = bc * glm::vec3 { w0, w1, w2 };
                        // Compute the perspective correct UV coordinates.
                        // Source: OpenGL 4.6 Specification, 2022 (pp. 479).
                        float correction = 1.0f / ( bc.x + bc.y + bc.z );
                        auto  uv         = ( tri[0].uv * bc.x + tri[1].uv * bc.y + tri[2].uv * bc.z ) * correction;

                        auto srcColor = diffuseTexture ? diffuseTexture->sample( uv ) : diffuseColor;

                        renderTarget( x, y ) = srcColor;

                        // Update the depth buffer.
                        d = z;
                    }
                }
            }
        }
    }
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
    out.color    = glm::vec4( static_cast<float>( in.color.r ) / 255.0f, static_cast<float>( in.color.g ) / 255.0f, static_cast<float>( in.color.b ) / 255.0f, static_cast<float>( in.color.a ) / 255.0f );

    return out;
}

float Rasterizer::distance( const glm::vec4& p, Plane plane )
{
    switch ( plane )
    {
    case Plane::Left:
        return p.x - p.w;
    case Plane::Right:
        return p.x + p.w;
    case Plane::Top:
        return p.y + p.w;
    case Plane::Bottom:
        return p.y - p.w;
    case Plane::Near:
        return p.z + p.w;
    case Plane::Far:
        return p.z - p.w;
    }

    return 0.0f;  // This shouldn't happen.
}

int Rasterizer::clipTriangle( const VertexOutput* in, int n_in, VertexOutput* out, Plane plane )
{
    // Number of output vertices.
    int n_out = 0;

    // Start vertex.
    VertexOutput S = in[0];

    for ( int i = 0; i < n_in; ++i )
    {
        // Terminal vertex.
        VertexOutput P = in[( i + 1 ) % n_in];

        // Compute the signed distance to the plane.
        float dS = distance( S.position, plane );
        float dP = distance( P.position, plane );

        if ( dS >= 0.0f && dP >= 0.0f )
        {
            // Case 1. Both s and p are on visible side of plane.
            // If the edge is entirely on the visible side if the clipping plane,
            // only its terminal vertex P, need be output since its initial vertex S
            // will already have been output.

            out[n_out++] = P;
        }
        else if ( dS < 0.0f && dP >= 0.0f )
        {
            // Case 4. Edge enters visible region (p is visible, s is not).
            // If the edge enters the visible side of the clipping plane with P visible
            // and S not, then two output vertices must be generated: the intersection
            // of the edge and the clipping plane, followed by the terminal vertex P.

            float a = dS / ( dS - dP );

            out[n_out++] = S + a * ( P - S );
            out[n_out++] = P;
        }
        else if ( dS >= 0.0f && dP < 0.0f )
        {
            // Case 3. Edge leaves visible region (s is visible, p is not).
            // If the edge leaves the visible side of the clipping plane, with S
            // visible and P not, then only the intersection of the edge and the clipping
            // plane need be output because the initial vertex S, will already have
            // been output.

            float a = dS / ( dS - dP );

            out[n_out++] = S + a * ( P - S );
        }

        // Next edge...
        S = P;
    }

    return n_out;
}

int Rasterizer::clipTriangle( const VertexOutput* in, VertexOutput* out )
{
}
