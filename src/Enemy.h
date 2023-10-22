#ifndef ENEMY
#define ENEMY

#include "Entity.h"
#include "Powerup.h"

struct Manager;

enum EnemyType
{
    BASIC,
    SIMPLE,
    FLYING
};

struct Enemy : Entity
{
    // enemy type
    EnemyType enemyType;
    // number of sprite frames
    int numFrames;
    // fps of animation
    int spriteFPS;
    // enemy death sound
    Sound deathSound;
    // coords of first image of spritesheet for enemy
    Vector2 src;
    // coords of first image of explosion sprite for enemy
    Vector2 explosionSrc;
    // dimensions of explosion texture - source
    Vector2 explosionDims;
    // dimensions of explosion texture - output
    Vector2 explosionOutputDims;
    // indexing vector for explosion frames
    Vector2 explosionIndexingVec;
    // number of explosion frames
    int numExplosionFrames;
    // explosion fps
    float explosionFps;
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
    // is exploding
    bool exploding;
    // is attacking
    bool attacking;
    // resetting position
    bool resettingPosition;
    // last position
    Vector2 lastPosition;
    // rotation
    float rotation;
    // last rotation 
    float lastRotation;
    // turn rate
    float rotationRate;
    // max velocity
    float maxVelocity;
    // bullet location in spritesheet
    Vector2 bulletSrc;
    // bullet texture dims
    Vector2 bulletTextureDims;
    // bullet outputDims;
    Vector2 bulletOutputDims;
    // bullet firing sound;
    Sound defaultFireSound;
    
    
    Enemy(Texture2D _spriteSheet, Sound _deathSound, Sound _fireSound, Vector2 _src, Vector2 _bulletSrc, Vector2 _bulletSrcDims, Vector2 _bulletOutputDims, Vector2 _explosionSrc, Vector2 _indexingVec, int _numFrames, float _spriteFPS, Vector2 _textureDims, Vector2 _outputDims, Vector2 _explosionDims, Vector2 _explosionOutputDims, int _numExplosionFrames, Vector2 _explosionIndexingVec, float _explosionFps, Vector2 _hitboxDims, Vector2 _origin, float _hp, EnemyType _enemyType, float _rotationRate, float _maxVelocity);
    void attack(Manager* _manager, float _dt);
    void update(Manager* _manager);
    void draw(float dt);
    std::shared_ptr<Powerup> dropPowerup();
    bool outOfBounds(Manager* _manager);
    void goBackToFormation(Manager* _manager, float _dt);
    void fireWeapon(Manager* _manager);
    
};

#endif // ENEMY
