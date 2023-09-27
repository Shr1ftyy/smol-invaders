#include "Entity.h"

EntityId Entity::newId = 0;

Entity::Entity(Texture2D _spriteSheet, Vector2 _textureDims, Vector2 _outputDims, Vector2 _hitboxDims, Vector2 _origin, EntityType _entityType)
{
    spriteSheet = _spriteSheet;
    textureDims = _textureDims;
    outputDims = _outputDims;
    hitboxDims = _hitboxDims;
    position = _origin;
    type = _entityType;
    id = Entity::newId;
    Entity::newId++;
}

void Entity::update()
{
}

void Entity::draw()
{
    Rectangle srcRec = {0.0f, 0.0f, textureDims.x, textureDims.y};
    Rectangle destRec = {position.x, position.y, outputDims.x, outputDims.y};
    Vector2 origin = {(float)outputDims.x / 2, (float)outputDims.y / 2};
    DrawTexturePro(spriteSheet, srcRec, destRec, origin, (float)0, WHITE);
    // DrawRectangleLines(position.x - origin.x, position.y - origin.y, hitboxDims.x, hitboxDims.y, RED);
}
