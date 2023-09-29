#ifndef POWERUP
#define POWERUP 

#include "Entity.h"

#include <chrono>
#include <unordered_map>

struct Manager;

enum PowerupType
{
    LIFE,
    DOUBLE_BULLET,
    PIERCING,
    INCREASE_FIRERATE,
    TRIPLE_BULLET,
    SIDEWAYS,
};

struct Powerup : Entity
{
    inline static std::unordered_map<PowerupType, float> powerupLifetimes=
    {
        {PowerupType::LIFE, 0.0},
        {PowerupType::DOUBLE_BULLET, 10000.0},
        {PowerupType::PIERCING, 10000.0},
        {PowerupType::INCREASE_FIRERATE, 10000.0},
        {PowerupType::TRIPLE_BULLET, 10000.0},
        {PowerupType::SIDEWAYS, 10000.0}
    };
    // powerup type
    PowerupType powerupType;
    // velocity vector
    Vector2 velocity;
    // time it was registered
    std::chrono::system_clock::time_point timeRegistered;
    
    Powerup(Texture2D _spriteSheet, Vector2 _textureDims, Vector2 _outputDims, Vector2 _hitboxDims, Vector2 _origin, EntityType _entityType, PowerupType _powerupType, Vector2 velocity);
    void update(Manager* _manager, float dt);
    bool outOfBounds(int screenWidth, int screenHeight);
};

#endif // POWERUP
