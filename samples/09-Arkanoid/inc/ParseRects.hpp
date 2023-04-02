#pragma once

#include <Math/Rect.hpp>

#include <filesystem>
#include <vector>

// Parse the XML file and return a list of rectangles in the texture atlas.
std::vector<Math::RectI> ParseRects( const std::filesystem::path& xmlFile );
