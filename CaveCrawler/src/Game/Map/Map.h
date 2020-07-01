#pragma once

#include "Engine/Square.h"
#include "Game/Map/CaveGenerator.h"
#include "Game/Map/Tile.h"

class FOV;

class Map
{
private:
	int mWidth;
	int mHeight;
	std::vector<Tile*> mMap;

	FOV* mFOV;
	Square::GameObject* mFocus;
	int mVisionRadius;
	float mVisionTimer;

	CaveGenerator* mMapGenerator;

	void ClearMap();
	void BuildMap();

public:
	Map(int width, int height);
	~Map();

	bool IsBoundary(int x, int y) const;
	bool IsWalkable(int x, int y) const;
	bool IsOpaque(int x, int y) const;

	bool Collides(Square::PhysEntity* entity, int x, int y);

	bool InFOV(Square::GameObject* entity) const;
	inline void SetFocus(Square::GameObject* focus) { mFocus = focus; }

	inline int Width() const { return mWidth; }
	inline int Height() const { return mHeight; }

	inline int TileSize() const { return mMap.front()->TILE_SIZE; }

	Square::Vector2 Entry() const;
	Square::Vector2 Exit() const;

	void Update();
	void Render();
};