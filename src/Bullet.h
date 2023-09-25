#ifndef BULLET
#define BULLET
#include "Entity.h"

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

    Bullet(Texture2D _spriteSheet, Vector2 _src, Vector2 _indexingVec, int _numFrames, int _spriteFPS, Vector2 _dimensions, Vector2 _origin, Vector2 _velocity);
    bool outOfBounds(int screenWidth, int screenHeight);
    void update(Manager* _manager);
    void draw(int dt);
};

#endif
