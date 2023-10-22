#include "Player.h"
#include "Bullet.h"
#include "Manager.h"

#define FMT_HEADER_ONLY
#include "raymath.h"
#include <fmt/core.h>

Player::Player(Texture2D _spriteSheet, Sound _defaultFireSound, Sound _powerupSound, Texture2D _defaultBulletSheet, Vector2 _src, Vector2 _indexingVec, int _numFrames, float _spriteFPS, Vector2 _textureDims, Vector2 _outputDims, Vector2 _hitboxDims, Vector2 _origin, float _maxVelocity, float _force, float _frictionCoeff, float _normal, int _fireRate, float _hp) :
Entity(_spriteSheet, _textureDims, _outputDims, _hitboxDims, _origin, EntityType::PLAYER_TYPE)
{
    defaultFireSound = _defaultFireSound;
    powerupSound = _powerupSound;
    
    currentVelocity = {0, 0};
    maxVelocity = _maxVelocity;
    force = _force;
    frictionCoeff = _frictionCoeff;
    normal = _normal;
    fireRate = _fireRate;
    defaultFireRate = _fireRate;
    
    numFrames = _numFrames;
    spriteFPS = _spriteFPS;
    src = _src;
    indexingVec = _indexingVec;
    defaultBulletSheet = _defaultBulletSheet;
    hp = _hp;
}

//------------------------------------------------------------------------------------
// Out-Of-Bounds check
//------------------------------------------------------------------------------------
bool Player::outOfBounds(Vector2 entity, int screenWidth, int screenHeight)
{
    if (entity.x <= 0 || entity.x >= screenWidth || entity.y <= 0 || entity.y >= screenHeight)
    {
        return true;
    }
    return false;
}

void Player::fireDefault(Manager* _manager)
{
    Vector2 bulletTextureDims = {32, 32};
    Vector2 buletHitboxDims = {15, 15};
    float bulletX = position.x;
    float bulletY = position.y - (hitboxDims.y / 2);
    std::shared_ptr<Bullet> bullet = std::shared_ptr<Bullet>
    (
        new Bullet(defaultBulletSheet, {192, 64}, {32, 0}, 4, 30.0, bulletTextureDims, bulletTextureDims, buletHitboxDims, {bulletX, bulletY}, {0, -0.5}, 5, EntityType::PLAYER_BULLET)
    );

    _manager->addEntity(bullet);
    PlaySound(defaultFireSound);
}

//------------------------------------------------------------------------------------
// Update player
//------------------------------------------------------------------------------------
void Player::update(Manager* _manager, int _screenWidth, int _screenHeight, float dt)
{
    // check if powerups have expired
    for(auto p = begin(activePowerups); p != end(activePowerups);)
    {
        float* timeLeft = &(p->second);
        PowerupType powerupType = p->first;
        
        (*timeLeft) -= dt;
        if ((*timeLeft) <= 0)
        {
            switch (powerupType)
            {
                case (PowerupType::INCREASE_FIRERATE):
                fireRate = fireRate > defaultFireRate ? defaultFireRate : fireRate;
                default:
                ;
            }
            activePowerups.erase(p++);
            continue;   
            
        } 
        else
        {
            switch (powerupType)
            {
                case (PowerupType::INCREASE_FIRERATE):
                fireRate = fireRate < 2*defaultFireRate ? 2*defaultFireRate : fireRate;
                default:
                ;
            }
            ++p;
            continue;
        }
        ++p;
    }
    
    Vector2 oldShipPosition = position;
    
    bool engineOn = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A) ||
        IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);
    
    std::string engineStatus = fmt::format("engine status: {}", engineOn);
    
    DrawTextEx(_manager->gameFont, engineStatus.c_str(), {10, 110}, 20, 1.0, RAYWHITE);
    
    Vector2 resultantVelocity = currentVelocity;
    
    float engineForce = engineOn ? force : 0;
    
    float resultantForce = engineForce - (frictionCoeff * normal);
    
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
        resultantVelocity.x += ((float)resultantForce * dt);
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        resultantVelocity.x -= ((float)resultantForce * dt);
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
        resultantVelocity.y -= ((float)resultantForce * dt);
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
        resultantVelocity.y += ((float)resultantForce * dt);
    if (!engineOn)
    {
        Vector2 resultantForceVec = Vector2Normalize(resultantVelocity);
        resultantForceVec.x *= resultantForce;
        resultantForceVec.y *= resultantForce;
        resultantVelocity.x += resultantForceVec.x * dt;
        resultantVelocity.y += resultantForceVec.y * dt;
        
        // stop moving by friction
        if (currentVelocity.x > 0 && resultantVelocity.x <= 0)
        {
            resultantVelocity.x = 0;
        }
        else if (currentVelocity.x < 0 && resultantVelocity.x >= 0)
        {
            resultantVelocity.x = 0;
        }
        
        if (currentVelocity.y > 0 && resultantVelocity.y <= 0)
        {
            resultantVelocity.y = 0;
        }
        else if (currentVelocity.y < 0 && resultantVelocity.y >= 0)
        {
            resultantVelocity.y = 0;
        }
    }
    
    if (Vector2Length(resultantVelocity) >= maxVelocity)
    {
        resultantVelocity = Vector2Normalize(resultantVelocity);
        resultantVelocity.x *= maxVelocity;
        resultantVelocity.y *= maxVelocity;
    }
    
    Vector2 positionDelta = {resultantVelocity.x * dt, resultantVelocity.y * dt};
    position = Vector2Add(position, positionDelta);
    
    // TODO: ADD GLOBAL HITBOX VAR
    // if (IsKeyDown(KEY_H)) showHitboxes = !showHitboxes;
    
    if (outOfBounds(position, _screenWidth, _screenHeight))
    {
        position = oldShipPosition;
    }
    
    currentVelocity = resultantVelocity;
    
    // fire weapon(s)
    if (IsKeyDown(KEY_SPACE))
    {
        auto now = std::chrono::steady_clock::now();
        auto timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastShotTime);
        if (timeElapsed.count() >= (float)1000 / fireRate)
        {
            fireDefault(_manager);
            lastShotTime = now;
        }
    }
}

void Player::powerUp(std::shared_ptr<Powerup> powerup)
{
    float powerupTime = Powerup::powerupLifetimes[powerup->powerupType];
    activePowerups[powerup->powerupType] = powerupTime; 
    PlaySound(powerupSound);
}
