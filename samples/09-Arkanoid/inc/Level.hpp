#pragma once

#include "Brick.hpp"
#include "Physics.hpp"

#include <Graphics/Image.hpp>
#include <Math/Circle.hpp>

#include <optional>
#include <vector>

class Level
{
public:
    Level() = default;

    explicit Level( int levelId );

    void update( float deltaTime );
    void draw( Graphics::Image& image );

    /// <summary>
    /// 
    /// </summary>
    /// <param name="c"></param>
    /// <param name="v"></param>
    /// <returns></returns>
    std::optional<Physics::HitInfo> checkCollision( const Math::Circle& c, const glm::vec2& v ) const;

private:
    std::vector<Brick> bricks;
};