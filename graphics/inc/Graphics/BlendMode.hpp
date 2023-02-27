#pragma once

#include "Color.hpp"
#include "Config.hpp"

namespace Graphics
{

/// <summary>
/// The BlendOperation defines how the final output pixel will be determined.
/// The Source (s) value is the pixel that is generated from the pixel shader.
/// The Destination (d) value is the value that is currently stored in the backbuffer.
/// Blend operations can be defined for the color and alpha values separately.
/// The Source and Destination values are first scaled by the source and
/// destination blend factors <see cref="BlendFactor"/> before the blend operation is applied.
/// </summary>
enum class BlendOperation
{
    Add,              ///< Add source and destination ( s + d ).
    Subtract,         ///< Subtract destination from source ( s - d )
    ReverseSubtract,  ///< Subtract source from destination ( d - s )
    Min,              ///< The minimum of source and destination ( min( s, d ) )
    Max               ///< The maximum of source and destination ( max( s, d ) )
};

/// <summary>
/// The BlendFactor is used to scale the pixel operand (either Source (s) or
/// Destination (d) pixel value) before the blend operation <see cref="BlendOperation"/> is applied.
/// </summary>
enum class BlendFactor
{
    Zero,              ///< Multiply the pixel operand by 0 before applying the blend operation.
    One,               ///< Multiply the pixel operand by 1 before applying the blend operation.
    SrcColor,          ///< Multiply the pixel operand by the source color (Cs) before applying the blend operation.
    OneMinusSrcColor,  ///< Multiply the pixel operand by ( 1 - Cs ) before applying the blend operation.
    DstColor,          ///< Multiply the pixel operand by the destination color (Cd) before applying the blend operation.
    OneMinusDstColor,  ///< Multiply the pixel operand by ( 1 - Cd ) before applying the blend operation.
    SrcAlpha,          ///< Multiply the pixel operand by the source alpha ( As ) before applying the blend operation.
    OneMinusSrcAlpha,  ///< Multiply the pixel operand by ( 1 - As ) before applying the blend operation.
    DstAlpha,          ///< Multiply the pixel operand by the destination alpha ( Ad ) before applying the blend operation.
    OneMinusDstAlpha,  ///< Multiply the pixel operand by ( 1 - Ad ) before applying the blend operation.
    SrcAlphaSat,       ///< Multiply the pixel operand by min( As, 1 - Ad ) before applying the blend operation.
};

struct SR_API BlendMode
{
    /// <summary>
    /// Set to `true` to enable blending.
    /// </summary>
    bool blendEnable = false;

    /// <summary>
    /// The blend factor to apply to the source color (S).
    /// Default: `BlendFactor::One`.
    /// </summary>
    BlendFactor srcFactor = BlendFactor::One;

    /// <summary>
    /// The blend factor to apply to the destination color (D).
    /// Default: `BlendFactor::Zero`.
    /// </summary>
    BlendFactor dstFactor = BlendFactor::Zero;

    /// <summary>
    /// The blend operation to perform on the color components.
    /// Default: `BlendOperation::Add`
    /// </summary>
    BlendOperation blendOp = BlendOperation::Add;

    /// <summary>
    /// The blend factor to apply to the source alpha (As).
    /// Default: `BlendFactor::One`.
    /// </summary>
    BlendFactor srcAlphaFactor = BlendFactor::One;

    /// <summary>
    /// The blend factor to apply the the destination alpha (Ad).
    /// Default: `BlendFactor::Zero`.
    /// </summary>
    BlendFactor dstAlphaFactor = BlendFactor::Zero;

    /// <summary>
    /// The blend operation to perform on the alpha components.
    /// Default: `BlendOperation::Add`.
    /// </summary>
    BlendOperation alphaOp = BlendOperation::Add;

    constexpr BlendMode( bool           blendEnable    = false,
                         BlendFactor    srcFactor      = BlendFactor::One,
                         BlendFactor    dstFactor      = BlendFactor::Zero,
                         BlendOperation blendOp        = BlendOperation::Add,
                         BlendFactor    srcAlphaFactor = BlendFactor::One,
                         BlendFactor    dstAlphaFactor = BlendFactor::Zero,
                         BlendOperation alphaOp        = BlendOperation::Add )
    : blendEnable { blendEnable }
    , srcFactor { srcFactor }
    , dstFactor { dstFactor }
    , blendOp { blendOp }
    , srcAlphaFactor { srcAlphaFactor }
    , dstAlphaFactor { dstAlphaFactor }
    , alphaOp { alphaOp }
    {}

    /// <summary>
    /// Perform blending on the source and destination colors.
    /// </summary>
    /// <param name="srcColor">The source color.</param>
    /// <param name="dstColor">The destination color.</param>
    /// <returns></returns>
    constexpr Color Blend( const Color& srcColor, const Color& dstColor ) const noexcept;

    static const BlendMode Disable;
    static const BlendMode AlphaBlend;
    static const BlendMode AdditiveBlend;
    static const BlendMode SubtractiveBlend;
};

/// <summary>
/// Compute the blend factor for source and destination alpha values.
/// </summary>
/// <param name="sA">Source alpha.</param>
/// <param name="dA">Destination alpha.</param>
/// <param name="blendFactor">The blend factor to apply.</param>
/// <returns>The computed alpha blend factor.</returns>
constexpr uint8_t ComputeBlendFactor( uint8_t sA, uint8_t dA, BlendFactor blendFactor ) noexcept
{
    switch ( blendFactor )
    {
    case BlendFactor::Zero:
        return 0;
    case BlendFactor::One:
        return 255;
    case BlendFactor::SrcColor:
        return sA;
    case BlendFactor::OneMinusSrcColor:
        return 255 - sA;
    case BlendFactor::DstColor:
        return dA;
    case BlendFactor::OneMinusDstColor:
        return 255 - dA;
    case BlendFactor::SrcAlpha:
        return sA;
    case BlendFactor::OneMinusSrcAlpha:
        return 255 - sA;
    case BlendFactor::DstAlpha:
        return dA;
    case BlendFactor::OneMinusDstAlpha:
        return 255 - dA;
    case BlendFactor::SrcAlphaSat:
        return std::min<uint8_t>( sA, 255 - dA );
    }

    return sA;
}

constexpr Color ComputeBlendFactor( const Color& src, const Color& dst, BlendFactor blendFactor ) noexcept
{
    switch ( blendFactor )
    {
    case BlendFactor::Zero:
        return Color::Black;
    case BlendFactor::One:
        return Color::White;
    case BlendFactor::SrcColor:
        return src;
    case BlendFactor::OneMinusSrcColor:
        return Color::White - src;
    case BlendFactor::DstColor:
        return dst;
    case BlendFactor::OneMinusDstColor:
        return Color::White - dst;
    case BlendFactor::SrcAlpha:
        return { src.a, src.a, src.a, src.a };
    case BlendFactor::OneMinusSrcAlpha:
        return Color::White - Color { src.a, src.a, src.a, src.a };
    case BlendFactor::DstAlpha:
        return { dst.a, dst.a, dst.a, dst.a };
    case BlendFactor::OneMinusDstAlpha:
        return Color::White - Color { dst.a, dst.a, dst.a, dst.a };
    case BlendFactor::SrcAlphaSat:
        return min( Color { src.a, src.a, src.a, src.a }, Color::White - Color { dst.a, dst.a, dst.a, dst.a } );
    }

    return src;
}

constexpr Color ComputeBlendOp( const Color& srcColor, const Color& dstColor, BlendOperation op ) noexcept
{
    switch ( op )
    {
    case BlendOperation::Add:
        return srcColor + dstColor;
    case BlendOperation::Subtract:
        return srcColor - dstColor;
    case BlendOperation::ReverseSubtract:
        return dstColor - srcColor;
    case BlendOperation::Min:
        return min( srcColor, dstColor );
    case BlendOperation::Max:
        return max( srcColor, dstColor );
    }

    return srcColor;
}

constexpr uint8_t ComputeBlendOp( uint8_t sA, uint8_t dA, BlendOperation op ) noexcept
{
    switch ( op )
    {
    case BlendOperation::Add:
        return sA + dA;
    case BlendOperation::Subtract:
        return sA - dA;
    case BlendOperation::ReverseSubtract:
        return dA - sA;
    case BlendOperation::Min:
        return std::min( sA, dA );
    case BlendOperation::Max:
        return std::max( sA, dA );
    }

    return sA;
}

constexpr Color BlendMode::Blend( const Color& srcColor, const Color& dstColor ) const noexcept
{
    if ( !blendEnable )
        return srcColor;

    const Color sRGB = ComputeBlendFactor( srcColor, dstColor, srcFactor ) * srcColor;
    const Color dRGB = ComputeBlendFactor( srcColor, dstColor, dstFactor ) * dstColor;
    const auto  sA   = static_cast<uint8_t>( ComputeBlendFactor( srcColor.a, dstColor.a, srcAlphaFactor ) * srcColor.a / 255 );
    const auto  dA   = static_cast<uint8_t>( ComputeBlendFactor( srcColor.a, dstColor.a, dstAlphaFactor ) * dstColor.a / 255 );

    const Color   RGB = ComputeBlendOp( sRGB, dRGB, blendOp );
    const uint8_t A   = ComputeBlendOp( sA, dA, alphaOp );

    return { RGB.r, RGB.g, RGB.b, A };
}

}  // namespace Graphics