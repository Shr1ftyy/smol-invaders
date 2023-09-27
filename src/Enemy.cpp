#include "Enemy.h"
#include "Manager.h"
#include "raymath.h"
#include <iostream>

Enemy::Enemy(Texture2D _spriteSheet, Sound _deathSound, Vector2 _src, Vector2 _explosionSrc, Vector2 _indexingVec, int _numFrames, float _spriteFPS, Vector2 _textureDims, Vector2 _outputDims, Vector2 _explosionDims, Vector2 _explosionOutputDims, int _numExplosionFrames, Vector2 _explosionIndexingVec, float _explosionFps, Vector2 _hitboxDims, Vector2 _origin, float _hp) :
    Entity(_spriteSheet, _textureDims, _outputDims, _hitboxDims, _origin, EntityType::ENEMY_TYPE)
{
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
        
        DrawTexturePro(spriteSheet, srcRec, destRec, textureOrigin, (float)0, WHITE);
    
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
        
        DrawTexturePro(spriteSheet, srcRec, destRec, textureOrigin, (float)0, WHITE);
    
    }
    // DrawRectangleLines(position.x - hitboxOrigin.x, position.y - hitboxOrigin.y, hitboxDims.x, hitboxDims.y, YELLOW);
}