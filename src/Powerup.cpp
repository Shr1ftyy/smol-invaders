#include "Powerup.h"
#include "Manager.h"

#include "raymath.h"

Powerup::Powerup(Texture2D _spriteSheet, Vector2 _textureDims, Vector2 _outputDims, Vector2 _hitboxDims, Vector2 _origin, EntityType _entityType, PowerupType _powerupType, Vector2 _velocity) :
    Entity(_spriteSheet, _textureDims, _outputDims, _hitboxDims, _origin, EntityType::POWERUP_TYPE)
{
    powerupType = _powerupType;
    velocity = _velocity;
}

bool Powerup::outOfBounds(int screenWidth, int screenHeight)
{
    if (position.x <= 0 || position.x >= screenWidth || position.y <= 0 || position.y >= screenHeight)
    {
        return true;
    }
    return false;
}

void Powerup::update(Manager* _manager, float dt)
{
    if (!destroyed)
    {
        Vector2 displacement = {velocity.x * dt, velocity.y * dt};
        position = Vector2Add(position, displacement);
        if (outOfBounds(_manager->screenWidth, _manager->screenHeight))
        {
            destroyed = true;
            _manager->deleteQueue.push_back(this);
        }
    }
    else 
    {
        _manager->deleteQueue.push_back(this);
    }
}
