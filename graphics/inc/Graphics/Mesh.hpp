#pragma once

#include "Config.hpp"
#include "Material.hpp"
#include "Vertex.hpp"

#include <span>
#include <vector>
#include <filesystem>

namespace Graphics
{
class SR_API Mesh final
{
public:
    /// <summary>
    /// Load a mesh from a given set of vertices and (optionally) a set of vertices.
    /// </summary>
    /// <param name="vertices">The vertices to load into the vertex buffer.</param>
    /// <param name="indices">The indices to load into the index buffer.</param>
    /// <param name="material">The material to use when rendering this mesh.</param>
    explicit Mesh( std::span<const Vertex3D> vertices, std::span<int> indices = {}, std::shared_ptr<Material> material = nullptr );

    Mesh();
    Mesh( const Mesh& );
    Mesh( Mesh&& ) noexcept;
    ~Mesh();

    Mesh& operator=( const Mesh& );
    Mesh& operator=( Mesh&& ) noexcept;

    const std::vector<Vertex3D>& getVertices() const noexcept;
    const std::vector<int>&      getIndices() const noexcept;
    std::shared_ptr<Material>    getMaterial() const noexcept;
    void                         setMaterial( std::shared_ptr<Material> material );

    /// <summary>
    /// Check to see if this mesh has an index buffer.
    /// </summary>
    /// <returns>`true` if this mesh has indices, `false` otherwise.</returns>
    bool hasIndices() const noexcept
    {
        return !indexBuffer.empty();
    }

private:
    std::vector<Vertex3D> vertexBuffer;
    std::vector<int>      indexBuffer;
    std::shared_ptr<Material> material;
};
}  // namespace Graphics
