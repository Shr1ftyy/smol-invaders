#include "Enemy.h"
#include "Manager.h"
#include "raymath.h"
#include <iostream>

Enemy::Enemy(Texture2D _spriteSheet, Sound _deathSound, Vector2 _src, Vector2 _indexingVec, int _numFrames, float _spriteFPS, Vector2 _textureDims, Vector2 _outputDims, Vector2 _hitboxDims, Vector2 _origin, float _hp) :
    Entity(_spriteSheet, _textureDims, _outputDims, _hitboxDims, _origin, EntityType::ENEMY_TYPE)
{
    deathSound = _deathSound;
    src = _src;
    indexingVec = _indexingVec;
    currentIndex = 0;
    numFrames = _numFrames;
    timeSinceLastDraw = 0;
    currentFramePos = _src;
    hp = _hp;
    destroyed, exploding = 0;
    spriteFPS = _spriteFPS;
}

void Enemy::update(Manager* _manager)
{
    // TODO: implement enemy updating
    if (hp <= 0)
    {
        PlaySound(deathSound);
        exploding = true;
        destroyed = true;
        hitboxDims = {0, 0};
    }
}

void Enemy::draw(float dt)
{

    timeSinceLastDraw += dt;
    std::cout << "timeSinceLastDraw: " << timeSinceLastDraw << std::endl;
    
    if (timeSinceLastDraw >= (float)1000 / spriteFPS)
    {
        std::cout << "updating sprite..." << std::endl;
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
    
    std::cout << currentIndex << " " << numFrames << std::endl;
    std::cout << currentFramePos.x << " " << currentFramePos.y << std::endl;

    DrawTexturePro(spriteSheet, srcRec, destRec, textureOrigin, (float)0, WHITE);
    std::cout << "drawn" << std::endl;

    Vector2 hitboxOrigin = {(float)hitboxDims.x / 2, (float)hitboxDims.y / 2};
    // DrawRectangleLines(position.x - hitboxOrigin.x, position.y - hitboxOrigin.y, hitboxDims.x, hitboxDims.y, YELLOW);
}