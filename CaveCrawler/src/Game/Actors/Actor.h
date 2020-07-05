#pragma once

#include "Game/Map/Map.h"

class Actor : public Square::PhysEntity
{
protected:
	std::shared_ptr<Map> mMap;

	bool HasGround();
	bool HasCeiling();
	bool HasLeftWall();
	bool HasRightWall();

	bool InsideTile();

	bool OutsideMap();

public:
	Actor(std::shared_ptr<Map> map);
	virtual ~Actor() = default;

	bool IgnoreCollisions() override;

	virtual void Update();
	virtual void Render();
};