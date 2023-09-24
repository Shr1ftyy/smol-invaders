#include "Player.h"
#include "raymath.h"
#include <fmt/core.h>

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

	bool engineOn = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A) || 
		IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);
	
	std::string engineStatus = fmt::format("engine status: {}", engineOn);
	Font hackNerdFontRegular = LoadFontEx("resources/fonts/HackNerdFontMono/HackNerdFontMono-Regular.ttf", 20, 0, 250);

	DrawText(engineStatus.c_str(), 10, 110, 20, RAYWHITE);

	Vector2 resultantVelocity = currentVelocity;

	float engineForce = engineOn ? force : 0;

	float resultantForce = engineForce - (frictionCoeff * normal);

	if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) resultantVelocity.x += ((float)resultantForce * dt);
	if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) resultantVelocity.x -= ((float)resultantForce * dt);
	if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) resultantVelocity.y -= ((float)resultantForce * dt);
	if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) resultantVelocity.y += ((float)resultantForce * dt);
	if(!engineOn)
	{
		Vector2 resultantForceVec = Vector2Normalize(resultantVelocity);
		resultantForceVec.x *= resultantForce;
		resultantForceVec.y *= resultantForce;
		resultantVelocity.x += resultantForceVec.x * dt;
		resultantVelocity.y += resultantForceVec.y * dt;

		// stop moving by friction
		if (currentVelocity.x > 0 && resultantVelocity.x <= 0) 
		{
			resultantVelocity.x = 0;
		}
		else if (currentVelocity.x < 0 && resultantVelocity.x >= 0)
		{
			resultantVelocity.x = 0;
		}

		if (currentVelocity.y > 0 && resultantVelocity.y <= 0) 
		{
			resultantVelocity.y = 0;
		}
		else if (currentVelocity.y < 0 && resultantVelocity.y >= 0)
		{
			resultantVelocity.y = 0;
		}

	}

	if (Vector2Length(resultantVelocity) >= maxVelocity)
	{
		resultantVelocity = Vector2Normalize(resultantVelocity);
		resultantVelocity.x *= maxVelocity;
		resultantVelocity.y *= maxVelocity;
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
