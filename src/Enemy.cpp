#include "Enemy.h"
#include "Manager.h"

#include "raymath.h"

Enemy::Enemy(Texture2D _spriteSheet, Sound _deathSound, Sound _fireSound, Vector2 _src, Vector2 _bulletSrc, Vector2 _bulletSrcDims, Vector2 _bulletOutputDims, Vector2 _explosionSrc, Vector2 _indexingVec, int _numFrames, float _spriteFPS, Vector2 _textureDims, Vector2 _outputDims, Vector2 _explosionDims, Vector2 _explosionOutputDims, int _numExplosionFrames, Vector2 _explosionIndexingVec, float _explosionFps, Vector2 _hitboxDims, Vector2 _origin, float _hp, EnemyType _enemyType, float _rotationRate, float _maxVelocity) :
Entity(_spriteSheet, _textureDims, _outputDims, _hitboxDims, _origin, EntityType::ENEMY_TYPE)
{
    enemyType = _enemyType;
    deathSound = _deathSound;
    src = _src;
    explosionSrc = _src;
    indexingVec = _indexingVec;
    currentIndex = 0;
    numFrames = _numFrames;
    timeSinceLastDraw = 0;
    currentFramePos = _src;
    hp = _hp;
    destroyed, exploding = 0;
    spriteFPS = _spriteFPS;
    explosionSrc = _explosionSrc;
    explosionDims = _explosionDims;
    explosionOutputDims = _explosionOutputDims;
    explosionIndexingVec = _explosionIndexingVec;
    numExplosionFrames = _numExplosionFrames;
    explosionFps = _explosionFps;
    this->attacking = false;
    rotationRate = _rotationRate;
    rotation = 0.0;
    lastRotation = rotation;
    maxVelocity = _maxVelocity;
    
    resettingPosition = false;
    lastPosition = position;
    
    bulletSrc = _bulletSrc;
    bulletTextureDims = _bulletSrcDims;
    bulletOutputDims = _bulletOutputDims;
    defaultFireSound = _fireSound;
}

bool Enemy::outOfBounds(Manager* _manager)
{
    if (position.y >= _manager->screenHeight)
    {
        return true;
    }
    return false;
}

void Enemy::fireWeapon(Manager* _manager)
{
    float bulletX = position.x;
    float bulletY = position.y + (hitboxDims.y / 2);
    // TODO: add bullet indexing vector
    Bullet* bullet = new Bullet(spriteSheet, bulletSrc, {0, 0}, 0, 0.0, bulletTextureDims, bulletOutputDims, bulletOutputDims, {bulletX, bulletY}, {0, 0.5}, 5);
    _manager->addEntity(bullet);
    PlaySound(defaultFireSound);
}


void Enemy::goBackToFormation(Manager* _manager, float _dt)
{
    attacking = false;
    Vector2 formationPosition = (*_manager->assignedPositionMap[id]);
    if
    (
        (
        lastPosition.x <= formationPosition.x  && position.x >= formationPosition.x
        || lastPosition.x >= formationPosition.x && position.x <= formationPosition.x
        )
        &&
        (
        lastPosition.y <= formationPosition.y  && position.y >= formationPosition.y
        || lastPosition.y >= formationPosition.y && position.y <= formationPosition.y
        )
    )
    {
        position = formationPosition;
        resettingPosition = false;
    }
    else
    {
        Vector2 disp = Vector2Subtract(formationPosition, position);
        Vector2 velocityVec = Vector2Normalize(disp);
        velocityVec.x *= maxVelocity * _dt;  
        velocityVec.y *= maxVelocity * _dt;
        
        position = Vector2Add(position, velocityVec);
    }
    
}

void Enemy::attack(Manager* _manager, float _dt)
{ }

Powerup* Enemy::dropPowerup()
{
    PowerupType powerupType;
    Texture2D powerupTexture;
    
    float p = (float)rand()/RAND_MAX;
    
    if (p <= 0.25)
    {
        float p_1 = (float)rand()/RAND_MAX;
        
        if (p_1  <= 0.05)
        {
            powerupType = PowerupType::LIFE;
            powerupTexture = LoadTexture("./resources/textures/life.png");
        }
        else if (p_1 > 0.05 <= 0.24)
        {
            powerupType = PowerupType::DOUBLE_BULLET;
            powerupTexture = LoadTexture("./resources/textures/double.png");
        }
        else if (p_1 > 0.24 <= 0.43)
        {
            powerupType = PowerupType::PIERCING;
            powerupTexture = LoadTexture("./resources/textures/piercing.png");
        }
        else if (p_1 > 0.43 <= 0.62)
        {
            powerupType = PowerupType::INCREASE_FIRERATE;
            powerupTexture = LoadTexture("./resources/textures/increase_firerate.png");
        }
        else if (p_1 > 0.62 <= 0.81)
        {
            powerupType = PowerupType::TRIPLE_BULLET;
            powerupTexture = LoadTexture("./resources/textures/triple.png");
        }
        else
        {
            powerupType = PowerupType::SIDEWAYS;
            powerupTexture = LoadTexture("./resources/textures/sideways.png");
        }
        
        Powerup* powerup = new Powerup
        (
         powerupTexture,
         { (float)powerupTexture.width, (float)powerupTexture.height },
         { 16, 16 },
         { 16, 16 },
         { position.x, position.y },
         EntityType::POWERUP_TYPE,
         powerupType,
         {0, 0.3}
         );
        
        return powerup;
    }
    return nullptr; 
}

void Enemy::update(Manager* _manager)
{
    // TODO: implement enemy updating
    if (!exploding && !destroyed && hp <= 0)
    {
        PlaySound(deathSound);
        exploding = true;
        hitboxDims = {0, 0};
        timeSinceLastDraw = 0;
        currentIndex = 0;
    }
}

void Enemy::draw(float dt)
{
    if(!exploding && !destroyed)
    {
        timeSinceLastDraw += dt;
        
        if (timeSinceLastDraw >= (float)1000 / spriteFPS)
        {
            currentIndex++;
            if (currentIndex >= numFrames)
            {
                currentIndex = 0;
            }
            
            Vector2 offset = {(float)currentIndex * indexingVec.x, (float)currentIndex * indexingVec.y};
            currentFramePos = Vector2Add(src, offset);
            
            timeSinceLastDraw = 0;
        }
        
        Rectangle srcRec = {currentFramePos.x, currentFramePos.y, textureDims.x, textureDims.y};
        Rectangle destRec = {position.x, position.y, outputDims.x, outputDims.y};
        Vector2 textureOrigin = {(float)textureDims.x / 2, (float)textureDims.y / 2};
        
        DrawTexturePro(spriteSheet, srcRec, destRec, textureOrigin, rotation, WHITE);
        
        Vector2 hitboxOrigin = {(float)hitboxDims.x / 2, (float)hitboxDims.y / 2};
    }
    else if (exploding && !destroyed)
    {
        timeSinceLastDraw += dt;
        
        if (timeSinceLastDraw >= (float)1000 / explosionFps)
        {
            currentIndex++;
            if (currentIndex >= numExplosionFrames)
            {
                destroyed = true;
            }
            
            Vector2 offset = {(float)currentIndex * explosionIndexingVec.x, (float)currentIndex * explosionIndexingVec.y};
            currentFramePos = Vector2Add(explosionSrc, offset);
            
            timeSinceLastDraw = 0;
        }
        
        Rectangle srcRec = {currentFramePos.x, currentFramePos.y, explosionDims.x, explosionDims.y};
        Rectangle destRec = {position.x, position.y, explosionOutputDims.x, explosionOutputDims.y};
        Vector2 textureOrigin = {(float)explosionDims.x / 2, (float)explosionDims.y / 2};
        
        DrawTexturePro(spriteSheet, srcRec, destRec, textureOrigin, rotation, WHITE);
        
    }
    // DrawRectangleLines(position.x - hitboxOrigin.x, position.y - hitboxOrigin.y, hitboxDims.x, hitboxDims.y, YELLOW);
}