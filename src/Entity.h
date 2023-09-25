#ifndef ENTITY
#define ENTITY
#include "raylib.h"
#include <unordered_map>
#include <chrono>

struct Manager;

enum EntityType
{
	PLAYER_TYPE,
	PLAYER_BULLET,
	ENEMY_TYPE,
	ENEMY_BULLET
};

using EntityId =  unsigned int;
using EventMap = std::unordered_map<EntityId, std::vector<void*>*>*;

struct Entity
{
	static EntityId newId;
	EntityId id;
	Texture2D spriteSheet;
	Vector2 dimensions;
	Vector2 position;
	EntityType type;

	Entity(Texture2D _spriteSheet, Vector2 _dimensions, Vector2 _origin, EntityType _entityType);
	void update();
	void draw();
};

using EntityMap = std::unordered_map<EntityId, Entity*>;

#endif // ENTITY
