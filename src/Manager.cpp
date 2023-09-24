#include "Manager.h"

Manager::Manager(int _screenWidth, int _screenHeight)
{
	screenWidth = _screenWidth;
	screenHeight = _screenHeight;
	//events = new std::vector<void*>();
}

void Manager::addEntity(Entity* _entity)
{
	entities.push_back(_entity);
}

void Manager::addEntity(Player* _entity)
{
	entities.push_back(_entity);
}

void Manager::update()
{
	auto now = std::chrono::high_resolution_clock::now();
	auto elapsed = now - lastUpdateTime;
	int dt = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
	lastUpdateTime = now;

	for (auto entity : entities) {
		Player* player = static_cast<Player*>(entity);
		switch (entity->type)
		{
		case PLAYER_TYPE:
			player->update(screenWidth, screenHeight, dt);
		default:
			entity->update();
		}
	}
}

void Manager::draw()
{
	for (auto entity : entities) {
		entity->draw();
	}
}
