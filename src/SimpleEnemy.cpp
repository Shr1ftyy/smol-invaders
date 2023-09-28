#include "SimpleEnemy.h"
#include "Manager.h"
#include "Entity.h"

#include "raylib.h"
#include "raymath.h"

#include <iostream>

SimpleEnemy::SimpleEnemy(Texture2D _spriteSheet, Sound _deathSound, Vector2 _src, Vector2 _explosionSrc, Vector2 _indexingVec, int _numFrames, float _spriteFPS, Vector2 _textureDims, Vector2 _outputDims, Vector2 _explosionDims, Vector2 _explosionOutputDims, int _numExplosionFrames, Vector2 _explosionIndexingVec, float _explosionFps, Vector2 _hitboxDims, Vector2 _origin, float _hp) : 
    Enemy(_spriteSheet, _deathSound, _src, _explosionSrc, _indexingVec, _numFrames, _spriteFPS, _textureDims, _outputDims, _explosionDims, _explosionOutputDims, _numExplosionFrames, _explosionIndexingVec, _explosionFps, _hitboxDims, _origin, _hp, EnemyType::SIMPLE)
{}

Powerup* SimpleEnemy::update(Manager* _manager)
{
    // TODO: implement enemy updating
    if (!exploding && !destroyed && hp <= 0)
    {
        PlaySound(deathSound);
        exploding = true;
        hitboxDims = {0, 0};
        timeSinceLastDraw = 0;
        currentIndex = 0;
        
        PowerupType powerupType;
        Texture2D powerupTexture;
        
        float p = (float)rand()/RAND_MAX;
        std::cout << p << std::endl;

        if (p <= 0.25)
        {
            float p_1 = (float)rand()/RAND_MAX;
            
            if (p_1  <= 0.05)
            {
                powerupType = PowerupType::LIFE;
                powerupTexture = LoadTexture("./resources/textures/life.png");
            }
            else if (p_1 > 0.05 <= 0.24)
            {
                powerupType = PowerupType::DOUBLE_BULLET;
                powerupTexture = LoadTexture("./resources/textures/double.png");
            }
            else if (p_1 > 0.24 <= 0.43)
            {
                powerupType = PowerupType::PIERCING;
                powerupTexture = LoadTexture("./resources/textures/piercing.png");
            }
            else if (p_1 > 0.43 <= 0.62)
            {
                powerupType = PowerupType::INCREASE_FIRERATE;
                powerupTexture = LoadTexture("./resources/textures/increase_firerate.png");
            }
            else if (p_1 > 0.62 <= 0.81)
            {
                powerupType = PowerupType::TRIPLE_BULLET;
                powerupTexture = LoadTexture("./resources/textures/triple.png");
            }
            else
            {
                powerupType = PowerupType::SIDEWAYS;
                powerupTexture = LoadTexture("./resources/textures/sideways.png");
            }
            
            Powerup* powerup = new Powerup
            (
                powerupTexture,
                { (float)powerupTexture.width, (float)powerupTexture.height },
                { 16, 16 },
                { 16, 16 },
                { position.x, position.y },
                EntityType::POWERUP_TYPE,
                powerupType,
                {0, 0.3}
            );
            
            return powerup;
        }
        
        return nullptr;
    }
    
    return nullptr;
}
