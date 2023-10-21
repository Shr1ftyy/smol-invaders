#include "SimpleEnemy.h"
#include "Manager.h"
#include "Entity.h"

#include "raylib.h"
#include "raymath.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

SimpleEnemy::SimpleEnemy(Texture2D _spriteSheet, Sound _deathSound, Sound _fireSound, Vector2 _src, Vector2 _bulletSrc, Vector2 _bulletSrcDims, Vector2 _bulletOutputDims, Vector2 _explosionSrc, Vector2 _indexingVec, int _numFrames, float _spriteFPS, Vector2 _textureDims, Vector2 _outputDims, Vector2 _explosionDims, Vector2 _explosionOutputDims, int _numExplosionFrames, Vector2 _explosionIndexingVec, float _explosionFps, Vector2 _hitboxDims, Vector2 _origin, float _hp, float _rotationRate, float _maxVelocity) : 
Enemy(_spriteSheet, _deathSound, _fireSound, _src, _bulletSrc, _bulletSrcDims, _bulletOutputDims, _explosionSrc, _indexingVec, _numFrames, _spriteFPS, _textureDims, _outputDims, _explosionDims, _explosionOutputDims, _numExplosionFrames, _explosionIndexingVec, _explosionFps, _hitboxDims, _origin, _hp, EnemyType::SIMPLE, _rotationRate, _maxVelocity)
{}

void SimpleEnemy::attack(Manager* _manager, float _dt)
{
    if(!firedWeapon)
    {
        fireWeapon(_manager);
        firedWeapon = true;
    }
}

Powerup* SimpleEnemy::update(Manager* _manager, float _dt)
{
    // TODO: implement enemy updating
    if((float)rand()/RAND_MAX < 0.0001)
    {
        attacking = true;    
    }
    
    if(attacking)
    {
        attack(_manager, _dt);
    }
    else if (resettingPosition)
    {
        goBackToFormation(_manager, _dt);
        firedWeapon = false;
    }

    if (!resettingPosition)
    {
        position = (*_manager->assignedPositionMap[id]);
    }
    
    if (!exploding && !destroyed && hp <= 0)
    {
        PlaySound(deathSound);
        exploding = true;
        hitboxDims = {0, 0};
        timeSinceLastDraw = 0;
        currentIndex = 0;
        
        return dropPowerup();
    }
    
    if (outOfBounds(_manager))
    {
        position.y = 0;
        attacking = false;
        resettingPosition = true;
    }
    
    return nullptr;
}

