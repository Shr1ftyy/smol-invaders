#include "Entity.h"

Entity::Entity(const char* _filepath, int _numFrames, int _spriteFPS, Vector2 _dimensions, Vector2 _origin, EntityType _entityType)
{
	spriteSheet = LoadTexture("./resources/textures/ship.png");
	numFrames = _numFrames;
	spriteFPS = _spriteFPS;
	dimensions = _dimensions;
	position = _origin;
	type = _entityType;
	lastFrameUpdateTime = std::chrono::high_resolution_clock::now();
}

void Entity::update() {}

void Entity::draw()
{
	//auto now = std::chrono::high_resolution_clock::now();

	//std::chrono::milliseconds timePassedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFrameUpdateTime);
	//int timePassed = timePassedMs.count();
	//if (timePassed >= (1000/spriteFPS)) {
	//	currentFrame++;
	//	if (currentFrame >= numFrames - 1) {
	//		currentFrame = 0;
	//	}
	//}

	Rectangle srcRec = { 0.0f, 0.0f, dimensions.x, dimensions.y };
	Rectangle destRec = { position.x, position.y, dimensions.x, dimensions.y };
	Vector2 origin = { (float)dimensions.x / 2, (float)dimensions.y / 2 };
	Vector2 xOffset{ (float)dimensions.x / 2, 0 };
	Vector2 yOffset{ 0, -(float)dimensions.y / 2 };
	DrawTexturePro(spriteSheet, srcRec, destRec, origin, (float)0, WHITE);
}

