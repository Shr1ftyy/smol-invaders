#include "Enemy.h"
#include "Manager.h"
#include "raymath.h"

Enemy::Enemy(Texture2D _spriteSheet, Sound _deathSound, Vector2 _src, Vector2 _indexingVec, int _numFrames, int _spriteFPS, Vector2 _textureDims, Vector2 _outputDims, Vector2 _hitboxDims, Vector2 _origin, float _hp) :
    Entity(_spriteSheet, _textureDims, _outputDims, _hitboxDims, _origin, EntityType::ENEMY_TYPE)
{
    deathSound = _deathSound;
    src = _src;
    indexingVec = _indexingVec;
    currentIndex = 1;
    timeSinceLastDraw = 0;
    currentFramePos = _src;
    hp = _hp;
}

void Enemy::update(Manager* _manager)
{ 
    // TODO: implement enemy updating
    if(hp <= 0) {
        _manager->deleteEntity(id);
        PlaySound(deathSound);
    }
}