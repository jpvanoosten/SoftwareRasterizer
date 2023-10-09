#pragma once

#include "Config.hpp"
#include "Material.hpp"
#include "Mesh.hpp"

#include <filesystem>
#include <vector>

namespace Graphics
{
    class SR_API Model final
    {
    public:
        Model();
        Model( const Model& );
        Model( Model&& ) noexcept;
        ~Model();

        Model& operator=( const Model& );
        Model& operator=( Model&& ) noexcept;
        
        /// <summary>
        /// Load a model from a file.
        /// </summary>
        /// <param name="modelFile">The file path to the model to load.</param>
        explicit Model( const std::filesystem::path& modelFile );

        /// <summary>
        /// Get all of the meshes of this model.
        /// </summary>
        /// <returns>A list of all of the meshes used to render this model.</returns>
        const std::vector<std::shared_ptr<Mesh>>& getMeshes() const;

        /// <summary>
        /// Get the AABB of this model.
        /// Note: The AABB of the model is the combination of the AABBs of all of the meshes.
        /// </summary>
        /// <returns>The AABB of this model.</returns>
        const Math::AABB& getAABB() const noexcept;

    private:
        std::vector<std::shared_ptr<Material>> materials;
        std::vector<std::shared_ptr<Mesh>>     meshes;
        Math::AABB                             aabb;
    };
}
