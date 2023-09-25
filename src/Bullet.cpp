#include "Manager.h"
#include "Bullet.h"
#include "raymath.h"

Bullet::Bullet(Texture2D _spriteSheet, Vector2 _src, Vector2 _indexingVec, int _numFrames, int _spriteFPS, Vector2 _dimensions, Vector2 _origin, Vector2 _velocity) :
	Entity(_spriteSheet, _dimensions, _origin, EntityType::PLAYER_BULLET)
{
	velocity = _velocity;
	src = _src;
	indexingVec = _indexingVec;
	currentIndex = 1;
	timeSinceLastDraw = 0;
	currentFramePos = _src;
}


bool Bullet::outOfBounds(int screenWidth, int screenHeight)
{
	if (position.x <= 0 || position.x >= screenWidth
		|| position.y <= 0 || position.y >= screenHeight) {
		return true;
	}
	return false;
}

void Bullet::update(Manager* _manager)
{
	position = Vector2Add(position, velocity);
	if (outOfBounds(_manager->screenWidth, _manager->screenHeight)) _manager->deleteEntity(id);
}

void Bullet::draw(int dt)
{
	//timeSinceLastDraw += dt;
	//if (timeSinceLastDraw >= 1000/spriteFPS) {
	//	currentIndex++;
	//	if (currentIndex >= numFrames) currentIndex = 0;

	//	Vector2 offset = { currentIndex * indexingVec.x, currentIndex * indexingVec.y };
	//	currentFramePos = Vector2Add(src, offset);

	//	timeSinceLastDraw = 0;
	//}
	

	Rectangle srcRec = { currentFramePos.x, currentFramePos.y, dimensions.x, dimensions.y };
	Rectangle destRec = { position.x, position.y, dimensions.x, dimensions.y };
	Vector2 origin = { (float)dimensions.x / 2, (float)dimensions.y / 2 };
	DrawTexturePro(spriteSheet, srcRec, destRec, origin, (float)0, WHITE);
	//DrawRectangleLines(position.x - origin.x, position.y - origin.y, dimensions.x, dimensions.y, RED);
}

