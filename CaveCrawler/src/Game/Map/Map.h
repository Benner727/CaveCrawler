#pragma once

#include "Engine/Square.h"
#include "Game/Map/CaveGenerator.h"
#include "Game/Map/Tile.h"

class Map
{
private:
	int mWidth;
	int mHeight;
	std::vector<Tile*> mMap;

	CaveGenerator* mMapGenerator;

	void ClearMap();
	void BuildMap();

public:
	Map(int width, int height);
	~Map();

	void Update();
	void Render();
};