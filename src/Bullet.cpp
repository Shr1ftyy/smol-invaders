#include "Bullet.h"

Bullet::Bullet(const char* _filepath, int _numFrames, int _spriteFPS, Vector2 _dimensions, Vector2 _origin, Vector2 _velocity) :
	Entity(_filepath, _numFrames, _spriteFPS, _dimensions, _origin, PLAYER_BULLET)
{
	velocity = _velocity;
}


bool Entity::outOfBounds(Vector2 entity, int screenWidth, int screenHeight)
{
	if (entity.x <= 0 || entity.x >= screenWidth
		|| entity.y <= 0 || entity.y >= screenHeight) {
		return true;
	}
	return false;
}

