#pragma once

#include "Engine/Square.h"

enum class TileType { Empty = 0, Solid };

class Tile : public Square::PhysEntity
{
public:
	static const int TILE_SIZE = 32;

private:
	Square::Sprite* mSprite;

	TileType mTileType;

public:
	Tile(Square::Sprite* sprite, TileType type, int x, int y);
	~Tile();

	inline TileType Type() const { return mTileType; }

	void Update();
	void Render();
};