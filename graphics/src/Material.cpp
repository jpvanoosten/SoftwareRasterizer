#include <Graphics/Material.hpp>

using namespace Graphics;

Material::Material(
    const Color&           diffuseColor,
    const Color&           specularColor,
    const Color&           ambientColor,
    const Color&           emissiveColor,
    float                  specularPower,
    std::shared_ptr<Image> diffuseTexture,
    std::shared_ptr<Image> alphaTexture,
    std::shared_ptr<Image> specularTexture,
    std::shared_ptr<Image> normalTexture,
    std::shared_ptr<Image> ambientTexture,
    std::shared_ptr<Image> emissiveTexture )
: diffuseColor { diffuseColor }
, specularColor { specularColor }
, ambientColor { ambientColor }
, emissiveColor { emissiveColor }
, specularPower { specularPower }
, diffuseTexture { std::move( diffuseTexture ) }
, alphaTexture { std::move( alphaTexture ) }
, specularTexture { std::move( specularTexture ) }
, normalTexture { std::move( normalTexture ) }
, ambientTexture { std::move( ambientTexture ) }
, emissiveTexture { std::move( emissiveTexture ) }
{}
