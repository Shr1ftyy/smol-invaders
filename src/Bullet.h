#ifndef BULLET
#define BULLET
#include "Entity.h"

struct Manager;

struct Bullet : Entity
{
    Vector2 velocity;
    // coords of first image of spritesheet for player
    Vector2 src;
    // indexing vector for rest of images
    Vector2 indexingVec;
    // current frame index
    int currentIndex;
    // current frame pos
    Vector2 currentFramePos;
    // time since last update (ms)
    int timeSinceLastDraw;
    // damage
    float dmg;

    Bullet(Texture2D _spriteSheet, Vector2 _src, Vector2 _indexingVec, int _numFrames, int _spriteFPS, Vector2 _textureDims, Vector2 _hitboxDims, Vector2 _origin, Vector2 _velocity, float _dmg);
    bool outOfBounds(int screenWidth, int screenHeight);
    void update(Manager* _manager);
    void draw(int dt);
};

#endif
