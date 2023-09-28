#ifndef ENTITY
#define ENTITY
#include "raylib.h"
#include <chrono>
#include <unordered_map>
#include <vector>

enum EntityType
{
    PLAYER_TYPE,
    PLAYER_BULLET,
    ENEMY_TYPE,
    ENEMY_BULLET,
    POWERUP_TYPE
};

using EntityId = unsigned int;
using EventMap = std::unordered_map<EntityId, std::vector<void*>*>*;

struct Entity
{
    static EntityId newId;
    EntityId id;
    Texture2D spriteSheet;
    Vector2 textureDims;
    Vector2 outputDims;
    Vector2 hitboxDims;
    Vector2 position;
    EntityType type;
    bool destroyed;

    Entity(Texture2D _spriteSheet, Vector2 _textureDims, Vector2 _outputDims, Vector2 _hitboxDims, Vector2 _origin, EntityType _entityType);
    void update();
    void draw();
};

using EntityMap = std::unordered_map<EntityId, Entity*>;

#endif // ENTITY
