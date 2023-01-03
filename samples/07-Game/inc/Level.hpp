#pragma once

#include "Player.hpp"

#include <Image.hpp>
#include <Math/AABB.hpp>

#include <LDtkLoader/World.hpp>
#include <LDtkLoader/Level.hpp>

class Level
{
public:
    Level() = default;
    Level( const ldtk::World& world, const ldtk::Level& level );

    void update( float deltaTime );

    // Reset level.
    void reset();

    void draw( sr::Image& image ) const;


private:
    const ldtk::World* world = nullptr;
    const ldtk::Level* level = nullptr;

    // Level colliders.
    std::vector<Math::AABB> colliders;

    Player player;
};