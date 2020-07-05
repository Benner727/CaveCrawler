#pragma once

#include "Game/Actors/Enemies/Enemy.h"

class Drone : public Enemy
{
public:
	Drone(std::shared_ptr<Map> map, std::shared_ptr<PathFinder> pathFinder, Square::GameObject* target = nullptr);
	~Drone() = default;
};