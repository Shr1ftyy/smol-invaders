#include "SimpleEnemy.h"
#include "raymath.h"

SimpleEnemy::SimpleEnemy(Texture2D _spriteSheet, Sound _deathSound, Vector2 _src, Vector2 _indexingVec, int _numFrames, int _spriteFPS, Vector2 _textureDims, Vector2 _outputDims, Vector2 _hitboxDims, Vector2 _origin, float _hp) :
    Enemy(_spriteSheet, _deathSound, _src, _indexingVec, _numFrames, _spriteFPS, _textureDims, _outputDims, _hitboxDims, _origin, _hp)
{ }
