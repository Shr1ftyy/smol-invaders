#ifndef ENTITY
#define ENTITY
#include "raylib.h"
#include "vector"
#include <chrono>

enum EntityType
{
	PLAYER_TYPE,
	PLAYER_BULLET,
	ENEMY_TYPE,
	ENEMY_BULLET
};

struct Entity
{
	Texture2D spriteSheet;
	int numFrames;
	int spriteFPS;
	int spriteIndex;
	Vector2 dimensions;
	Vector2 position;
	EntityType type;
	std::chrono::steady_clock::time_point lastFrameUpdateTime;

	Entity(const char* _filepath, int _numFrames, int _spriteFPS, Vector2 _dimensions, Vector2 _origin, EntityType _entityType);
	void update();
	void draw();
};

using EntityVec = std::vector<Entity*>;

#endif // ENTITY
