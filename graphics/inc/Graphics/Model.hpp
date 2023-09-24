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

        const std::vector<std::shared_ptr<Mesh>>& getMeshes() const;

    private:
        std::vector<std::shared_ptr<Material>> materials;
        std::vector<std::shared_ptr<Mesh>>     meshes;
    };
}
