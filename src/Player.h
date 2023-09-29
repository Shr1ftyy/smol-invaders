#ifndef PLAYER
#define PLAYER

#include "Entity.h"
#include "Powerup.h"

#include <unordered_map>
#include <utility>

struct Manager; // Forward declaration of Manager

struct Player : Entity
{
    // default bullet firing sound
    Sound defaultFireSound;
    // power up sound
    Sound powerupSound;
    // last time bullet was shot
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
    // default bullets per second
    int defaultFireRate;
    // coords of first image of spritesheet for player
    Vector2 src;
    // indexing vector for rest of images
    Vector2 indexingVec;
    // number of sprite frams
    int numFrames;
    // sprite fps
    float spriteFPS;
    // default bullet spritesheet
    Texture2D defaultBulletSheet;
    // health points
    float hp;
    // active powerups
    std::unordered_map<PowerupType, std::pair<Powerup*, float>> activePowerups;

    Player(Texture2D _spriteSheet, Sound _defaultFireSound, Sound _powerupSound, Texture2D _defaultBulletSheet, Vector2 _src, Vector2 _indexingVec, int _numFrames, float _spriteFPS, Vector2 _textureDims, Vector2 _outputDims, Vector2 _hitboxDims, Vector2 _origin, float _maxVelocity, float _force, float _frictionCoeff, float _normal, int _fireRate, float _hp);
    bool outOfBounds(Vector2 entity, int screenWidth, int screenHeight);
    void update(Manager* _manager, int _screenWidth, int _screenHeight, float dt);
    void fireDefault(Manager* _manager);
    void draw();
    void powerUp(Powerup* _powerup);
};
#endif // PLAYER
