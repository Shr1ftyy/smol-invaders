#ifndef SIMPLE_ENEMY
#define SIMPLE_ENEMY

#include "Enemy.h"
#include "Powerup.h"

struct Manager;

struct SimpleEnemy : Enemy
{
    // fired weapon
    bool firedWeapon;

    SimpleEnemy(Texture2D _spriteSheet, Sound _deathSound, Sound _fireSound, Vector2 _src, Vector2 _bulletSrc, Vector2 _bulletSrcDims, Vector2 _bulletOutputDims, Vector2 _explosionSrc, Vector2 _indexingVec, int _numFrames, float _spriteFPS, Vector2 _textureDims, Vector2 _outputDims, Vector2 _explosionDims, Vector2 _explosionOutputDims, int _numExplosionFrames, Vector2 _explosionIndexingVec, float _explosionFps, Vector2 _hitboxDims, Vector2 _origin, float _hp, float _rotationRate, float _maxVelocity);
    Powerup* update(Manager* _manager, float _dt);
    void attack(Manager* _manager, float _dt);
};

#endif // SIMPLE_ENEMY