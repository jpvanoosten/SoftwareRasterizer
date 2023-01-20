#pragma once

#include "Box.hpp"
#include "Effect.hpp"
#include "Pickup.hpp"
#include "Player.hpp"

#include <Image.hpp>
#include <Math/AABB.hpp>
#include <TileMap.hpp>

#include <LDtkLoader/Level.hpp>
#include <LDtkLoader/World.hpp>

enum class ColliderType
{
    Default,
    Sand,
    Slime,
};

struct Collider
{
    ColliderType type = ColliderType::Default;
    Math::AABB   aabb;
    bool         isOneWay = false;
    bool         isTrap   = false;
};

class Level final
{
public:
    Level() = default;
    Level( const ldtk::Project& project, const ldtk::World& world, const ldtk::Level& level );

    void update( float deltaTime );

    // Reset level.
    void reset();

    // Choose which character to use for this level.
    void setCharacter( size_t characterId );

    const Player& getPlayer() const noexcept
    {
        return player;
    }

    void draw( sr::Image& image ) const;

private:
    // Add a pickup with a name, initial position.
    void addPickup( std::string_view name, const glm::vec2& pos );
    // Check collision with a pickup and an AABB collider.
    void checkPickupCollision( const Math::Sphere& pickupCollider, const Math::AABB& colliderAABB, glm::vec2& pos, glm::vec2& vel );
    void updateCollisions( float deltaTime );
    void updatePickups( float deltaTime );
    void updateEffects( float deltaTime );
    void updateBoxes( float deltaTime );

    const ldtk::World* world = nullptr;
    const ldtk::Level* level = nullptr;

    std::string levelName;

    // Level colliders.
    std::vector<Collider> colliders;

    // Fruit sprites.
    std::map<std::string, std::shared_ptr<sr::SpriteSheet>> fruitSprites;
    // Box sprites.
    std::map<std::string, Box> boxPrefabs;
    

    // All pickups
    std::vector<Pickup> allPickups;
    // Sprite animation to play when a pickup is collected.
    sr::SpriteAnim pickupCollected;
    // Currently playing effects.
    std::vector<Effect> effects;

    // Boxes
    std::vector<std::shared_ptr<Box>> boxes;

    // Level tile map.
    sr::TileMap tileMap;
    // Separate tile map for spike traps.
    sr::TileMap spikeMap;

    Player    player;
    glm::vec2 playerStart { 0 };
};
