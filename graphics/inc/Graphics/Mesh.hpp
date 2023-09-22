#pragma once

#include "Config.hpp"
#include "Vertex.hpp"

#include <span>
#include <vector>

namespace Graphics
{
class SR_API Mesh final
{
    explicit Mesh( std::span<const Vertex3D> vertices, std::span<int> indices = {} );

    Mesh();
    Mesh( const Mesh& );
    Mesh( Mesh&& ) noexcept;
    ~Mesh();

    Mesh& operator=( const Mesh& );
    Mesh& operator=( Mesh&& ) noexcept;

    const std::vector<Vertex3D>& getVertices() const noexcept;
    const std::vector<int>&      getIndices() const noexcept;

    /// <summary>
    /// Check to see if this mesh has an index buffer.
    /// </summary>
    /// <returns>`true` if this mesh has indices, `false` otherwise.</returns>
    bool hasIndices() const noexcept
    {
        return !indexBuffer.empty();
    }

public:
    std::vector<Vertex3D> vertexBuffer;
    std::vector<int>      indexBuffer;
};
}  // namespace Graphics
