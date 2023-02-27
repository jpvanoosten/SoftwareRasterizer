#pragma once

namespace Graphics
{

/// <summary>
/// Address modes used for texture sampling.
/// </summary>
enum class AddressMode
{
    Wrap,    ///< Tile the texture.
    Mirror,  ///< Flip the texture coordinates at integer boundaries.
    Clamp,   ///< Clamp texture coordinates in the range 0..1.
};

/// <summary>
/// FillMode determines how primitives are rendered.
/// * FillMode::WireFrame: Primitives are rendered as lines.
/// * FillMode::Solid: Primitives are rendered as solid objects.
/// </summary>
/// <remarks>
enum class FillMode
{
    WireFrame,  ///< Polygons are drawn as line segments.
    Solid       ///< Polygons interiors are filled.
};

}
