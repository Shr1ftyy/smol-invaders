#ifndef MANAGER
#define MANAGER
#include "Player.h"

struct Manager
{
	int screenWidth;
	int screenHeight;
	// game clock -> used for physics
	std::chrono::steady_clock::time_point lastUpdateTime;
	EntityVec entities;
	std::vector<void*>* events;

	Manager(int _screenWidth, int _screenHeight);
	void addEntity(Entity* _entity);
	void addEntity(Player* _entity);
	void update();
	void draw();
};

#endif // MANAGER