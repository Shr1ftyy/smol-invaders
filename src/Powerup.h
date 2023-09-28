#ifndef POWERUP
#define POWERUP 

#include "Entity.h"

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
    // powerup type
    PowerupType powerupType;
    Vector2 velocity;
    
    Powerup(Texture2D _spriteSheet, Vector2 _textureDims, Vector2 _outputDims, Vector2 _hitboxDims, Vector2 _origin, EntityType _entityType, PowerupType _powerupType, Vector2 velocity);
    void update(Manager* _manager, float dt);
    bool outOfBounds(int screenWidth, int screenHeight);
};

#endif // POWERUP
