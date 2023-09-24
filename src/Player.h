#ifndef PLAYER
#define PLAYER
#include "Entity.h"

struct Player : Entity
{
	std::chrono::steady_clock::time_point lastShotTime;
	// force produced by thrusters
	float force;
	// max ship velocity
	float maxVelocity;
	// current ship velocity
	Vector2 currentVelocity;
	// friction coefficient
	float frictionCoeff;
	// normal force -> used to produce friction
	float normal;
	// bullets per second
	int fireRate;

	Player(const char* _filepath, int _numFrames, int _spriteFPS, Vector2 _dimensions, Vector2 _origin, float _maxVelocity, float _force, float _frictionCoeff, float _normal, int _fireRate);
	bool outOfBounds(Vector2 entity, int screenWidth, int screenHeight);
	void update(int _screenWidth, int _screenHeight, int dt);
	void draw();
};
#endif // PLAYER
