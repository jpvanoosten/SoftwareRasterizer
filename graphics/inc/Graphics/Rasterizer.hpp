#pragma once

#include "Buffer.hpp"
#include "Config.hpp"
#include "Mesh.hpp"

#include <Math/Camera3D.hpp>
#include <Math/Plane.hpp>
#include <Math/Viewport.hpp>

#include <cstddef>

namespace Graphics
{
class SR_API Rasterizer
{
public:
    enum class Plane
    {
        Left,
        Right,
        Top,
        Bottom,
        Near,
        Far
    };

    /// <summary>
    /// The input to the vertex shader.
    /// </summary>
    struct VertexInput
    {
        glm::vec3 position;  // Position in object space.
        glm::vec3 normal;    // Normal in object space.
        glm::vec2 uv;        // Texture UV coordinates.
    };

    /// <summary>
    /// The output from the vertex shader.
    /// </summary>
    struct VertexOutput
    {
        glm::vec4 position;  // Position in clip-space.
        glm::vec3 normal;    // Normal in world-space.
        glm::vec2 uv;        // Texture coordinates.
    };

    Rasterizer();

    Rasterizer( std::size_t width, std::size_t height );

    /// <summary>
    /// Clear the contents of the color and depth buffers.
    /// </summary>
    /// <param name="color">The color to clear the color buffer to.</param>
    /// <param name="depth">The color to clear the depth buffer to.</param>
    void clear( const Color& color, float depth = 1.0f );

    /// <summary>
    /// Draw a mesh onto the image owned by the rasterizer.
    /// </summary>
    /// <param name="mesh"></param>
    /// <param name="modelMatrix"></param>
    void draw( const Mesh& mesh, const glm::mat4& modelMatrix );

    void setCamera( const Math::Camera* camera ) noexcept;
    void setViewport( const Math::Viewport& viewport ) noexcept;

    /// <summary>
    /// Get the color render target.
    /// </summary>
    /// <returns>The rasterizers render target.</returns>
    const Image& getImage() const noexcept;

    /// <summary>
    /// Get the depth buffer.
    /// </summary>
    /// <returns>The depth buffer.</returns>
    const Buffer<float>& getDepthBuffer() const noexcept;

protected:
    /// <summary>
    /// Take the vertex data from the mesh and convert it into triangles.
    /// </summary>
    /// <param name="mesh">The mesh to be drawn.</param>
    /// <param name="modelMatrix">The model transformation matrix.</param>
    void triangleAssembly( const Mesh& mesh, const glm::mat4& modelMatrix );

    /// <summary>
    /// Transform the vertex by the model-view-projection matrix.
    /// </summary>
    /// <param name="in">The incoming vertex position.</param>
    /// <param name="modelMatrix">The model matrix to transform the vertex normal.</param>
    /// <param name="modelViewProjectionMatrix">The model-view-projection matrix to transform the vertex position.</param>
    /// <returns>The transformed vertex.</returns>
    VertexOutput vertexShader( const VertexInput& in, const glm::mat4& modelMatrix, const glm::mat4& modelViewProjectionMatrix );

    /// <summary>
    /// Compute the distance from the point to one of the clipping planes.
    /// </summary>
    /// <param name="p">The clipping plane.</param>
    /// <param name="plane"></param>
    /// <returns>The signed distance from p to the plane.</returns>
    static float distance( const glm::vec4& p, Plane plane );

    /// <summary>
    /// Clip a triangle against a single clipping plane.
    /// </summary>
    /// <param name="in">The input vertices.</param>
    /// <param name="n_in">The number of input vertices.</param>
    /// <param name="out">The clipped triangles(s).</param>
    /// <param name="plane">The plane to clip the triangle against.</param>
    /// <returns>The number of resulting triangles.</returns>
    static int clipTriangle( const VertexOutput* in, int n_in, VertexOutput* out, Plane plane );

    /// <summary>
    /// Clip a triangle against the clipping planes.
    /// </summary>
    /// <param name="in">The triangle to be clipped.</param>
    /// <param name="out">The clipped triangle(s).</param>
    /// <returns>The number of resulting triangles.</returns>
    static int clipTriangle( const VertexOutput* in, VertexOutput* out );

private:
    std::size_t width  = 0u;
    std::size_t height = 0u;

    const Math::Camera* camera = nullptr;

    Image         renderTarget;
    Buffer<float> depthBuffer;

    Math::Viewport viewport;
};

inline Rasterizer::VertexOutput operator*( float lhs, const Rasterizer::VertexOutput& rhs )
{
    return {
        lhs * rhs.position,
        lhs * rhs.normal,
        lhs * rhs.uv,
    };
}

inline Rasterizer::VertexOutput operator*( const Rasterizer::VertexOutput& lhs, float rhs )
{
    return {
        lhs.position * rhs,
        lhs.normal * rhs,
        lhs.uv * rhs,
    };
}

inline Rasterizer::VertexOutput operator+( const Rasterizer::VertexOutput& lhs, const Rasterizer::VertexOutput& rhs )
{
    return {
        lhs.position + rhs.position,
        lhs.normal + rhs.normal,
        lhs.uv + rhs.uv,
    };
}

inline Rasterizer::VertexOutput operator-( const Rasterizer::VertexOutput& lhs, const Rasterizer::VertexOutput& rhs )
{
    return {
        lhs.position - rhs.position,
        lhs.normal - rhs.normal,
        lhs.uv - rhs.uv,
    };
}

}  // namespace Graphics
