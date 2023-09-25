#include "Manager.h"
#include "Entity.h"

EntityId Entity::newId = 0;

Entity::Entity(Texture2D _spriteSheet, Vector2 _dimensions, Vector2 _origin, EntityType _entityType)
{
    spriteSheet = _spriteSheet;
    dimensions = _dimensions;
    position = _origin;
    type = _entityType;
    id = Entity::newId;
    Entity::newId++;
}

void Entity::update() {}

void Entity::draw()
{
    Rectangle srcRec = { 0.0f, 0.0f, dimensions.x, dimensions.y };
    Rectangle destRec = { position.x, position.y, dimensions.x, dimensions.y };
    Vector2 origin = { (float)dimensions.x / 2, (float)dimensions.y / 2 };
    DrawTexturePro(spriteSheet, srcRec, destRec, origin, (float)0, WHITE);
}
