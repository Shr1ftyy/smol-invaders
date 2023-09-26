#include "SimpleEnemy.h"
#include "raymath.h"

SimpleEnemy::SimpleEnemy(Texture2D _spriteSheet, Vector2 _src, Vector2 _indexingVec, int _numFrames, int _spriteFPS, Vector2 _textureDims, Vector2 _hitboxDims, Vector2 _origin, float _hp) :
    Enemy(_spriteSheet, _src, _indexingVec, _numFrames, _spriteFPS, _textureDims, _hitboxDims, _origin, _hp)
{ }
