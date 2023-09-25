#ifndef SIMPLE_ENEMY
#define SIMPLE_ENEMY

#include "Entity.h"

struct Manager;

struct SimpleEnemy : Entity
{
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
    // health points
    float hp;

    SimpleEnemy(Texture2D _spriteSheet, Vector2 _src, Vector2 _indexingVec, int _numFrames, int _spriteFPS, Vector2 _dimensions, Vector2 _origin, float _hp);
    void update();
    void draw();
    void outOfBounds(Manager* _manager);
};

#endif // SIMPLE_ENEMY