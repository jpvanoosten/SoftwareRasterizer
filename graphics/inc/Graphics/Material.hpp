#pragma once

#include "Config.hpp"
#include "Image.hpp"

namespace Graphics
{
struct SR_API Material final
{
    explicit Material(
        const Color& diffuseColor  = Color::White,
        const Color& specularColor = Color::Black,
        const Color& ambientColor  = Color::Black,
        const Color& emissiveColor = Color::Black,
        float        specularPower = -1.0f,
        std::shared_ptr<Image> diffuseTexture = nullptr,
        std::shared_ptr<Image> alphaTexture = nullptr,
        std::shared_ptr<Image> specularTexture = nullptr,
        std::shared_ptr<Image> normalTexture = nullptr,
        std::shared_ptr<Image> ambientTexture = nullptr,
        std::shared_ptr<Image> emissiveTexture = nullptr
    );

    Material( const Material& ) = default;
    Material( Material&& ) noexcept = default;
    ~Material() = default;
    Material& operator=( const Material& ) = default;
    Material& operator=( Material&& ) noexcept = default;

    Color diffuseColor;
    Color specularColor;
    Color ambientColor;
    Color emissiveColor;
    float specularPower;

    std::shared_ptr<Image> diffuseTexture;
    std::shared_ptr<Image> alphaTexture;
    std::shared_ptr<Image> specularTexture;
    std::shared_ptr<Image> normalTexture;
    std::shared_ptr<Image> ambientTexture;
    std::shared_ptr<Image> emissiveTexture;
};
}  // namespace Graphics
