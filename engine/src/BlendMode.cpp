#include "BlendMode.hpp"

using namespace sr;

const BlendMode BlendMode::Disable { false };
const BlendMode BlendMode::AlphaBlend { true, BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha };
const BlendMode BlendMode::AdditiveBlend { true, BlendFactor::One, BlendFactor::One };
const BlendMode BlendMode::SubtractiveBlend { true, BlendFactor::One, BlendFactor::One, BlendOperation::Subtract };
