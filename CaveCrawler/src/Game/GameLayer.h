#pragma once

#include "Engine/Square.h"
#include "Map/Map.h"

class GameLayer : public Square::Layer
{
private:
	Map* mMap;

public:
	GameLayer();
	virtual ~GameLayer();

	virtual void OnUpdate() override;
	virtual void OnRender() override;
};