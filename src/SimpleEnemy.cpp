#include "Manager.h"
#include "SimpleEnemy.h"
#include "raymath.h"

SimpleEnemy::SimpleEnemy(Texture2D _spriteSheet, Vector2 _src, Vector2 _indexingVec, int _numFrames, int _spriteFPS, Vector2 _dimensions, Vector2 _origin, float _hp) :
    Entity(_spriteSheet, _dimensions, _origin, EntityType::ENEMY_TYPE)
{
    src = _src;
    indexingVec = _indexingVec;
    currentIndex = 1;
    timeSinceLastDraw = 0;
    currentFramePos = _src;
    hp = _hp;
}