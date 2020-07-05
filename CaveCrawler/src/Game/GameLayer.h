#pragma once

#include "Actors/Player/Player.h"
#include "Actors/Enemies/Drone.h"

class GameLayer : public Square::Layer
{
private:
	std::shared_ptr<Map> mMap;
	std::shared_ptr<Player> mPlayer;
	std::shared_ptr<PathFinder> mPathFinder;
	std::vector<Enemy*> mEnemies;

public:
	GameLayer();
	virtual ~GameLayer();

	virtual void OnUpdate() override;
	virtual void OnRender() override;
};