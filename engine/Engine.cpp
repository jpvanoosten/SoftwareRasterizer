#include "Engine.hpp"

using namespace Rasterizer;

Engine& Engine::Get()
{
    static Engine engine{};
    return engine;
}