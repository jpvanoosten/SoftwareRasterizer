#include <Graphics/Mesh.hpp>

using namespace Graphics;

Mesh::Mesh( std::span<const Vertex3D> vertices, std::span<int> indices, std::shared_ptr<Material> material )
: vertexBuffer { vertices.begin(), vertices.end() }
, indexBuffer { indices.begin(), indices.end() }
, material { std::move(material) }
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

std::shared_ptr<Material> Mesh::getMaterial() const noexcept
{
    return material;
}

void Mesh::setMaterial( std::shared_ptr<Material> _material )
{
    material = std::move( _material );
}
