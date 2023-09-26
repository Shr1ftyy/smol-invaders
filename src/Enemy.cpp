#include "Enemy.h"
#include "Manager.h"
#include "raymath.h"

Enemy::Enemy(Texture2D _spriteSheet, Vector2 _src, Vector2 _indexingVec, int _numFrames, int _spriteFPS, Vector2 _textureDims, Vector2 _hitboxDims, Vector2 _origin, float _hp) :
    Entity(_spriteSheet, _textureDims, _hitboxDims, _origin, EntityType::ENEMY_TYPE)
{
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
    }
}