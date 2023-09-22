#include <Graphics/Mesh.hpp>

using namespace Graphics;

Mesh::Mesh( std::span<const Vertex3D> vertices, std::span<int> indices )
: vertexBuffer { vertices.begin(), vertices.end() }
, indexBuffer { indices.begin(), indices.end() }
{}

Mesh::Mesh()                             = default;
Mesh::Mesh( const Mesh& )                = default;
Mesh::Mesh( Mesh&& ) noexcept            = default;
Mesh::~Mesh()                            = default;
Mesh& Mesh::operator=( const Mesh& )     = default;
Mesh& Mesh::operator=( Mesh&& ) noexcept = default;

const std::vector<Vertex3D>& Mesh::getVertices() const noexcept
{
    return vertexBuffer;
}

const std::vector<int>& Mesh::getIndices() const noexcept
{
    return indexBuffer;
}
