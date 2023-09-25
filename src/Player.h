#ifndef PLAYER
#define PLAYER
#include "Entity.h"

struct Manager; // Forward declaration of Manager

struct Player : Entity
{
    std::chrono::steady_clock::time_point lastShotTime;
    // force produced by thrusters
    float force;
    // max ship velocity
    float maxVelocity;
    // current ship velocity
    Vector2 currentVelocity;
    // friction coefficient
    float frictionCoeff;
    // normal force -> used to produce friction
    float normal;
    // bullets per second
    int fireRate;
    // coords of first image of spritesheet for player
    Vector2 src;
    // indexing vector for rest of images
    Vector2 indexingVec;
    // number of sprite frams
    int numFrames;
    // default bullet spritesheet
    Texture2D defaultBulletSheet;
    // health points
    float hp;

    Player(Texture2D _spriteSheet, Texture2D _defaultBulletSheet, Vector2 _src, Vector2 _indexingVec, int _numFrames, int _spriteFPS, Vector2 _dimensions, Vector2 _origin, float _maxVelocity, float _force, float _frictionCoeff, float _normal, int _fireRate, float _hp);
    bool outOfBounds(Vector2 entity, int screenWidth, int screenHeight);
    void update(Manager* _manager, int _screenWidth, int _screenHeight, int dt);
    void fireDefault(Manager* _manager);
    void draw();
};
#endif // PLAYER
