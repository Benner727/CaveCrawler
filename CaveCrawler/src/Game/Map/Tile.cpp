#include "Tile.h"

Tile::Tile(Square::Sprite* sprite, TileType type, int x, int y)
{
	mSprite = sprite;
	mSprite->Parent(this);

	mTileType = type;

	int tileSize = mSprite->ScaledDimensions().x;
	Pos(Square::Vector2(x * tileSize + tileSize * 0.5f, y * tileSize + tileSize * 0.5f));

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
	mSprite->Render(true);
}
