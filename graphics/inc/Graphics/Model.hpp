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

        /// <summary>
        /// Load a model from a file.
        /// </summary>
        /// <param name="modelFile">The file path to the model to load.</param>
        explicit Model( const std::filesystem::path& modelFile );

    private:
        std::vector<std::shared_ptr<Material>> materials;
        std::vector<std::shared_ptr<Mesh>>     meshes;
    };
}
