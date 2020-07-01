#include "Tile.h"

Tile::Tile(Square::Sprite* sprite, TileType type, int x, int y)
{
	mSprite = sprite;
	mSprite->Parent(this);

	mTileType = type;

	mExplored = false;

	Pos(Square::Vector2(x * TILE_SIZE + TILE_SIZE * 0.5f, y * TILE_SIZE + TILE_SIZE * 0.5f));

	AddCollider(new Square::BoxCollider(mSprite->ScaledDimensions()));
}

Tile::~Tile()
{
	delete mSprite;
}

void Tile::Update()
{
	mSprite->Update();
}

void Tile::Render()
{
	mSprite->Render();
}
