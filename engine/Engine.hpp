#pragma once

#include "Config.hpp"

namespace Rasterizer
{
    class ENGINE_API Engine
    {
    public:
        static Engine& Get();

    protected:
    private:
        Engine() = default;
        ~Engine() = default;
    };

} // namespace Rasterizer
