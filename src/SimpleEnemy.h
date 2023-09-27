#ifndef SIMPLE_ENEMY
#define SIMPLE_ENEMY

#include "Enemy.h"

struct SimpleEnemy : Enemy
{
    SimpleEnemy(Texture2D _spriteSheet, Sound _deathSound, Vector2 _src, Vector2 _indexingVec, int _numFrames, int _spriteFPS, Vector2 _textureDims, Vector2 _outputDims, Vector2 _hitboxDims, Vector2 _origin, float _hp);
};

#endif // SIMPLE_ENEMY