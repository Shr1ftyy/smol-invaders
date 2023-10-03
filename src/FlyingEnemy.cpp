#include "FlyingEnemy.h"
#include "Manager.h"
#include "Entity.h"

#include "raylib.h"
#include "raymath.h"
#include <cmath>
//#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

FlyingEnemy::FlyingEnemy(Texture2D _spriteSheet, Sound _deathSound, Sound _fireSound, Vector2 _src, Vector2 _bulletSrc, Vector2 _bulletSrcDims, Vector2 _bulletOutputDims, Vector2 _explosionSrc, Vector2 _indexingVec, int _numFrames, float _spriteFPS, Vector2 _textureDims, Vector2 _outputDims, Vector2 _explosionDims, Vector2 _explosionOutputDims, int _numExplosionFrames, Vector2 _explosionIndexingVec, float _explosionFps, Vector2 _hitboxDims, Vector2 _origin, float _hp, float _rotationRate, float _maxVelocity) : 
Enemy(_spriteSheet, _deathSound, _fireSound, _src, _bulletSrc, _bulletSrcDims, _bulletOutputDims, _explosionSrc, _indexingVec, _numFrames, _spriteFPS, _textureDims, _outputDims, _explosionDims, _explosionOutputDims, _numExplosionFrames, _explosionIndexingVec, _explosionFps, _hitboxDims, _origin, _hp, EnemyType::FLYING, _rotationRate, _maxVelocity)
{}

// TODO: create flyTowards function for all enemy types???
void FlyingEnemy::attack(Manager* _manager, float _dt)
{
    lastRotation = rotation;
    Player* player;
    
    for(auto entry : _manager->entities)
    {
        auto entity = entry.second;
        if(entity->type == EntityType::PLAYER_TYPE)
        {
            
            player = static_cast<Player*>(entity);
        }
    }
    
    Vector2 disp = Vector2Subtract(player->position, position);
    float theta = - (180.0/M_PI) * atan((double)disp.x/(double)disp.y);
    float delta = theta - rotation;
    if (!(position.y > player->position.y))
    {
        if
        (
         lastRotation <= theta && rotation > theta
         || lastRotation >= theta && rotation < theta
         || rotation != theta
         )
        {
            if(delta <= 0 )
            {
                rotation -= (rotationRate/1000) * _dt;
            }
            else
            {
                rotation += (rotationRate/1000) * _dt;
            }
            
        }
        // fire bullets towards player
        if(position.y >= player->position.y - 160 && !firedWeapon)
        {
            fireWeapon(_manager);
            firedWeapon = true;
        }        
        
        Vector2 velocityVec = Vector2Normalize(disp);
        velocityVec.x *= maxVelocity * _dt;  
        velocityVec.y *= maxVelocity * _dt;
        
        position = Vector2Add(position, velocityVec);
    } else
    {
        rotation = 0;
        Vector2 velocityVec = {0, maxVelocity * _dt};
        position = Vector2Add(position, velocityVec);
    }    
}

Powerup* FlyingEnemy::update(Manager* _manager, float _dt)
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
    else
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

