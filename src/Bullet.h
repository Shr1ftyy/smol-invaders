#ifndef BULLET
#define BULLET
#include "Entity.h"

class Bullet : Entity
{
	Vector2 velocity;

	Bullet(const char* _filepath, int _numFrames, int _spriteFPS, Vector2 _dimensions, Vector2 _origin, Vector2 _velocity);
	bool Entity::outOfBounds(Vector2 entity, int screenWidth, int screenHeight);
	void update();
	void draw();
};

#endif
