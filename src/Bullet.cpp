#include "Bullet.h"
#include "Manager.h"
#include "raymath.h"

Bullet::Bullet(Texture2D _spriteSheet, Vector2 _src, Vector2 _indexingVec, int _numFrames, float _spriteFPS, Vector2 _textureDims, Vector2 _outputDims, Vector2 _hitboxDims, Vector2 _origin, Vector2 _velocity, float _dmg) : Entity(_spriteSheet, _textureDims, _outputDims, _hitboxDims, _origin, EntityType::PLAYER_BULLET)
{
    numFrames = _numFrames;
    spriteFPS = _spriteFPS;
    velocity = _velocity;
    src = _src;
    indexingVec = _indexingVec;
    currentIndex = 1;
    timeSinceLastDraw = 0;
    currentFramePos = _src;
    dmg = _dmg;
    destroyed = 0;
    exploding = 0;
}

bool Bullet::outOfBounds(int screenWidth, int screenHeight)
{
    if (position.x <= 0 || position.x >= screenWidth || position.y <= 0 || position.y >= screenHeight)
    {
        return true;
    }
    return false;
}

void Bullet::update(Manager* _manager, float dt)
{
    Vector2 displacement = {velocity.x * dt, velocity.y * dt};
    if (!destroyed && !exploding)
    {
        position = Vector2Add(position, displacement);
        if (outOfBounds(_manager->screenWidth, _manager->screenHeight))
            destroyed = true;
    }
}

void Bullet::draw(int dt)
{
    if (exploding)
    {
        timeSinceLastDraw += dt;
        if (timeSinceLastDraw >= 1000 / spriteFPS)
        {
            currentIndex++;
            if (currentIndex >= numFrames)
            {
                exploding = 0;
                destroyed = 1;
                return;
            }

            Vector2 offset = {currentIndex * indexingVec.x, currentIndex * indexingVec.y};
            currentFramePos = Vector2Add(src, offset);

            timeSinceLastDraw = 0;
        }
    }

    Rectangle srcRec = {currentFramePos.x, currentFramePos.y, textureDims.x, textureDims.y};
    Rectangle destRec = {position.x, position.y, textureDims.x, textureDims.y};
    Vector2 textureOrigin = {(float)textureDims.x / 2, (float)textureDims.y / 2};
    DrawTexturePro(spriteSheet, srcRec, destRec, textureOrigin, (float)0, WHITE);

    Vector2 hitboxOrigin = {(float)hitboxDims.x / 2, (float)hitboxDims.y / 2};
    // DrawRectangleLines(position.x - hitboxOrigin.x, position.y - hitboxOrigin.y, hitboxDims.x, hitboxDims.y, YELLOW);
}
