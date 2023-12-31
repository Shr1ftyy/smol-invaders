#ifndef BULLET
#define BULLET
#include "Entity.h"

struct Manager;

struct Bullet : Entity
{
    // number of sprite frames
    int numFrames;
    // fps of animation
    float spriteFPS;
    // bullet velocity
    Vector2 velocity;
    // coords of first image of spritesheet for player
    Vector2 src;
    // indexing vector for rest of images
    Vector2 indexingVec;
    // current index
    int currentIndex;
    // current frame pos
    Vector2 currentFramePos;
    // time since last update (ms)
    int timeSinceLastDraw;
    // damage
    float dmg;
    // is exploding
    bool exploding;
    // previous position of bullet
    Vector2 prevPosition;
    
    Bullet(Texture2D _spriteSheet, Vector2 _src, Vector2 _indexingVec, int _numFrames, float _spriteFPS, Vector2 _textureDims, Vector2 _outputDims, Vector2 _hitboxDims, Vector2 _origin, Vector2 _velocity, float _dmg, EntityType _type);
    bool outOfBounds(int screenWidth, int screenHeight);
    void update(Manager* _manager, float dt);
    void draw(Manager* _manager, int dt);
};

#endif
