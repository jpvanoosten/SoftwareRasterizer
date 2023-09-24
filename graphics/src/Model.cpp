#include <Graphics/Model.hpp>
#include <Graphics/ResourceManager.hpp>

#include <tiny_obj_loader.h>

// Check that tinyobj loader is configured to use floats.
static_assert( std::is_same_v<tinyobj::real_t, float> );

#include <iostream>

using namespace Graphics;

constexpr Color ParseColor( const tinyobj::real_t color[3] ) noexcept
{
    return {
        static_cast<uint8_t>( color[0] * 255u ),
        static_cast<uint8_t>( color[1] * 255u ),
        static_cast<uint8_t>( color[2] * 255u ),
    };
}

inline std::shared_ptr<Material> ParseMaterial( const tinyobj::material_t& material ) noexcept
{
    Color diffuseColor  = ParseColor( material.diffuse );
    Color specularColor = ParseColor( material.specular );
    Color ambientColor  = ParseColor( material.ambient );
    Color emissiveColor = ParseColor( material.emission );
    float specularPower = material.shininess;

    // TODO: Check if we need to prefix with path to model file.
    auto diffuseTexture  = material.diffuse_texname.empty() ? nullptr : ResourceManager::loadImage( material.diffuse_texname );
    auto specularTexture = material.specular_texname.empty() ? nullptr : ResourceManager::loadImage( material.specular_texname );
    auto normalTexture   = material.bump_texname.empty() ? nullptr : ResourceManager::loadImage( material.bump_texname );
    auto ambientTexture  = material.ambient_texname.empty() ? nullptr : ResourceManager::loadImage( material.ambient_texname );
    auto emissiveTexture = material.emissive_texname.empty() ? nullptr : ResourceManager::loadImage( material.emissive_texname );

    return std::make_shared<Material>( diffuseColor, specularColor, ambientColor, emissiveColor, specularPower, diffuseTexture, specularTexture, normalTexture, ambientTexture, emissiveTexture );
}

inline std::shared_ptr<Mesh> ParseMesh( const tinyobj::mesh_t& mesh, const tinyobj::attrib_t& attrib ) noexcept
{
    std::vector<Vertex3D> vertices;
    // Reserve 3 vertices per face (we assume the mesh is triangulated).
    vertices.reserve( mesh.num_face_vertices.size() * 3 );

    size_t indexOffset = 0;
    // Loop over the faces of the mesh
    for ( size_t numVerts: mesh.num_face_vertices )
    {
        // We can only handle triangulated meshes.
        assert( numVerts == 3 );

        // Loop over the vertices of the face.
        for ( size_t v = 0; v < numVerts; ++v )
        {
            auto idx = mesh.indices[indexOffset + v];

            Vertex3D vert {};

            vert.position.x = attrib.vertices[idx.vertex_index * 3 + 0];
            vert.position.y = attrib.vertices[idx.vertex_index * 3 + 1];
            vert.position.z = attrib.vertices[idx.vertex_index * 3 + 2];

            if ( idx.normal_index >= 0 )
            {
                vert.normal.x = attrib.normals[idx.normal_index * 3 + 0];
                vert.normal.y = attrib.normals[idx.normal_index * 3 + 1];
                vert.normal.z = attrib.normals[idx.normal_index * 3 + 2];
            }

            if ( idx.texcoord_index >= 0 )
            {
                vert.texCoord.x = attrib.texcoords[idx.texcoord_index * 2 + 0];
                vert.texCoord.y = attrib.texcoords[idx.texcoord_index * 2 + 1];
            }

            vert.color.r = static_cast<uint8_t>( attrib.colors[idx.vertex_index * 3 + 0] * 255u );
            vert.color.g = static_cast<uint8_t>( attrib.colors[idx.vertex_index * 3 + 1] * 255u );
            vert.color.b = static_cast<uint8_t>( attrib.colors[idx.vertex_index * 3 + 2] * 255u );

            vertices.emplace_back( vert );
        }

        indexOffset += numVerts;
    }

    return std::make_shared<Mesh>( vertices );
}

Model::Model()                              = default;
Model::Model( const Model& )                = default;
Model::Model( Model&& ) noexcept            = default;
Model::~Model()                             = default;
Model& Model::operator=( const Model& )     = default;
Model& Model::operator=( Model&& ) noexcept = default;

Model::Model( const std::filesystem::path& modelFile )
{
    if ( !std::filesystem::exists( modelFile ) || !std::filesystem::is_regular_file( modelFile ) )
    {
        std::cerr << "ERROR: Failed to load model file: " << modelFile << std::endl;
        return;
    }

    // Default config should be fine.
    const tinyobj::ObjReaderConfig config {};
    tinyobj::ObjReader             reader;

    if ( !reader.ParseFromFile( modelFile.string(), config ) )
    {
        std::cerr << "ERROR: Failed to parse model file: " << modelFile << std::endl;
        return;
    }

    if ( !reader.Error().empty() )
    {
        std::cerr << "ERROR: Error parsing model file: " << modelFile << std::endl
                  << reader.Error() << std::endl;
        return;
    }

    if ( !reader.Warning().empty() )
    {
        std::cerr << "WARNING: Warning parsing model file: " << modelFile << std::endl
                  << reader.Warning() << std::endl;
        // It's just a warning... continue.
    }

    // Parse materials
    materials.reserve( reader.GetMaterials().size() );

    for ( const auto& m: reader.GetMaterials() )
    {
        const auto material = ParseMaterial( m );
        materials.emplace_back( material );
    }

    // Attribute arrays.
    const auto& attrib = reader.GetAttrib();

    // Parse shapes.
    for ( const auto& s: reader.GetShapes() )
    {
        auto mesh = ParseMesh( s.mesh, attrib );

        // Each mesh only has a single material associated with it.
        // Per-face materials are not supported.
        if ( !s.mesh.material_ids.empty() )
        {
            const auto materialId = s.mesh.material_ids[0];
            if ( materialId >= 0 && materialId < static_cast<int>( materials.size() ) )
            {
                mesh->setMaterial( materials[materialId] );
            }
        }

        // Add the mesh to the model's meshes array.
        meshes.emplace_back( mesh );
    }
}

const std::vector<std::shared_ptr<Mesh>>& Model::getMeshes() const
{
    return meshes;
}
