#include <Graphics/Mesh.hpp>

using namespace Graphics;

Mesh::Mesh( std::span<const Vertex3D> vertices, std::span<int> indices, std::shared_ptr<Material> material )
//: vertexBuffer { vertices.begin(), vertices.end() }
: indexBuffer { indices.begin(), indices.end() }
, material { std::move(material) }
{
    positions.reserve( vertices.size() );
    normals.reserve( vertices.size() );
    tangents.reserve( vertices.size() );
    bitangents.reserve( vertices.size() );
    texCoords.reserve( vertices.size() );
    colors.reserve( vertices.size() );

    for ( const auto& vert : vertices )
    {
        aabb.expand( vert.position );

        positions.emplace_back( vert.position );
        normals.emplace_back( vert.normal );
        tangents.emplace_back( vert.tangent );
        bitangents.emplace_back( vert.bitangent );
        texCoords.emplace_back( vert.texCoord );
        colors.emplace_back( vert.color );
    }
}

Mesh::Mesh()                                                     = default;
Mesh::Mesh( const Mesh& )                                        = default;
Mesh::Mesh( Mesh&& ) noexcept                                    = default;
Mesh::~Mesh()                                                    = default;
Mesh&                         Mesh::operator=( const Mesh& )     = default;
Mesh&                         Mesh::operator=( Mesh&& ) noexcept = default;

//const std::vector<Vertex3D>& Mesh::getVertices() const noexcept
//{
//    return vertexBuffer;
//}

const std::vector<glm::vec3>& Mesh::getPositions() const noexcept
{
    return positions;
}

const std::vector<glm::vec3>& Mesh::getNormals() const noexcept
{
    return normals;
}

const std::vector<glm::vec3>& Mesh::getTangents() const noexcept
{
    return tangents;
}

const std::vector<glm::vec3>& Mesh::getBitangents() const noexcept
{
    return bitangents;
}

const std::vector<glm::vec3>& Mesh::getTexCoords() const noexcept
{
    return texCoords;
}

const std::vector<Color>& Mesh::getColors() const noexcept
{
    return colors;
}

const std::vector<int>& Mesh::getIndices() const noexcept
{
    return indexBuffer;
}

const std::shared_ptr<Material>& Mesh::getMaterial() const noexcept
{
    return material;
}

void Mesh::setMaterial( std::shared_ptr<Material> _material )
{
    material = std::move( _material );
}

const Math::AABB& Mesh::getAABB() const noexcept
{
    return aabb;
}
