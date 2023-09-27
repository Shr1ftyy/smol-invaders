#ifndef ENEMY
#define ENEMY

#include "Entity.h"

struct Manager;

struct Enemy : Entity
{
    // number of sprite frames
    int numFrames;
    // fps of animation
    int spriteFPS;
    // enemy death sound
    Sound deathSound;
    // coords of first image of spritesheet for player
    Vector2 src;
    // indexing vector for rest of images
    Vector2 indexingVec;
    // current frame index
    int currentIndex;
    // current frame pos
    Vector2 currentFramePos;
    // time since last update (ms)
    float timeSinceLastDraw;
    // health points
    float hp;
    // is destroyed
    bool destroyed;
    // is exploding
    bool exploding;

    Enemy(Texture2D _spriteSheet, Sound _deathSound, Vector2 _src, Vector2 _indexingVec, int _numFrames, float _spriteFPS, Vector2 _textureDims, Vector2 _outputDims, Vector2 _hitboxDims, Vector2 _origin, float _hp);
    void update(Manager* _manager);
    void draw(float dt);
};

#endif // ENEMY