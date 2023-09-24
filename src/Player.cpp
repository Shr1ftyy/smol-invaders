#include "Player.h"
#include "raymath.h"

Player::Player(const char* _filepath, int _numFrames, int _spriteFPS, Vector2 _dimensions, Vector2 _origin, float _maxVelocity, float _force, float _frictionCoeff, float _normal, int _fireRate) :
	Entity(_filepath, _numFrames, _spriteFPS, _dimensions, _origin, PLAYER_TYPE)
{
	currentVelocity = { 0, 0 };
	maxVelocity = _maxVelocity;
	force = _force;
	frictionCoeff = _frictionCoeff;
	normal = _normal;
	fireRate = _fireRate;
}

//------------------------------------------------------------------------------------
// Out-Of-Bounds check
//------------------------------------------------------------------------------------
bool Player::outOfBounds(Vector2 entity, int screenWidth, int screenHeight)
{
	if (entity.x <= 0 || entity.x >= screenWidth
		|| entity.y <= 0 || entity.y >= screenHeight) {
		return true;
	}
	return false;
}


//------------------------------------------------------------------------------------
// Update player
//------------------------------------------------------------------------------------
void Player::update(int _screenWidth, int _screenHeight, int dt)
{
	Vector2 oldShipPosition = position;

	Vector2 resultantVelocity = currentVelocity;
	float resultantForce = force - (frictionCoeff * normal);
	resultantForce = resultantForce <= 0 ? resultantForce : 0;

	if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) resultantVelocity.x = resultantVelocity.x + ((float)force * dt);
	if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) resultantVelocity.x = resultantVelocity.x - ((float)force * dt);
	if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) resultantVelocity.y = resultantVelocity.y - ((float)force * dt);
	if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) resultantVelocity.y = resultantVelocity.y + ((float)force * dt);

	if (Vector2Length(resultantVelocity) >= maxVelocity)
	{
		resultantVelocity = Vector2Normalize(resultantVelocity);
		resultantVelocity.x *= force;
		resultantVelocity.y *= force;
	}

	Vector2 positionDelta = { resultantVelocity.x * dt, resultantVelocity.y * dt };
	position = Vector2Add(position, positionDelta);

	// TODO: ADD GLOBAL HITBOX VAR
	//if (IsKeyDown(KEY_H)) showHitboxes = !showHitboxes;

	if (outOfBounds(position, _screenWidth, _screenHeight)) {
		position = oldShipPosition;
	}

	currentVelocity = resultantVelocity;

	// fire weapon(s)
	//if (IsKeyDown(KEY_SPACE)) {
	//	auto now = std::chrono::high_resolution_clock::now();
	//	auto timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastShotTime);
	//	if (timeElapsed.count() >= (float)1000 / BULLET_HEIGHT) {
	//		fireWeapons(shipPosition, shipSize, &bullets);
	//		lastShotTime = now;
	//	}
	//}

}
