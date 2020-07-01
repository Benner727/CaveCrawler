#include "Map.h"

Map::Map(int width, int height)
	: mWidth(width), mHeight(height)
{
	mMapGenerator = new CaveGenerator(mWidth, mHeight);

	BuildMap();
}

Map::~Map()
{
	delete mMapGenerator;

	ClearMap();
}

bool Map::IsBoundary(int x, int y)
{
	if (x < 0 || y < 0)
		return true;

	if (x >= mWidth || y >= mHeight)
		return true;

	return false;
}

bool Map::IsWalkable(int x, int y)
{
	if (IsBoundary(x, y))
		return false;

	if (mMap[x + y * mWidth] != nullptr)
		return (mMap[x + y * mWidth]->Type() == TileType::Empty);

	return true;
}

bool Map::Collides(Square::PhysEntity* entity, int x, int y)
{
	if (IsBoundary(x, y))
		return true;

	return (mMap[x + y * mWidth]->CheckCollision(entity));
}

void Map::ClearMap()
{
	for (auto& tile : mMap)
		delete tile;
	mMap.clear();
}

void Map::BuildMap()
{
	ClearMap();

	std::vector<int> tiles = mMapGenerator->GenerateMap(std::random_device()());

	for (int y = 0; y < mHeight; y++)
	{
		for (int x = 0; x < mWidth; x++)
		{
			if (mMapGenerator->GetStart().x == x && mMapGenerator->GetStart().y == y)
				mMap.push_back(new Tile(new Square::Sprite("Entry.png"), TileType::Empty, x, y));
			else if (mMapGenerator->GetEnd().x == x && mMapGenerator->GetEnd().y == y)
				mMap.push_back(new Tile(new Square::Sprite("Exit.png"), TileType::Empty, x, y));
			else if (tiles[x + y * mWidth] == 0)
				mMap.push_back(new Tile(new Square::Sprite("Floor.png"), TileType::Empty, x, y));
			else if(tiles[x + y * mWidth] == 1)
				mMap.push_back(new Tile(new Square::Sprite("Wall.png"), TileType::Solid, x, y));
		}
	}

	Square::Graphics::Instance().SetLimit(Square::Vector2(mWidth * TileSize(), mHeight * TileSize()));
}

Square::Vector2 Map::Entry() const
{
	Square::Vector2 entry;
	entry.x = mMapGenerator->GetStart().x * TileSize() + TileSize() * 0.5f;
	entry.y = mMapGenerator->GetStart().y * TileSize() + TileSize() * 0.5f;

	return entry;
}

Square::Vector2 Map::Exit() const
{
	Square::Vector2 exit;
	exit.x = mMapGenerator->GetEnd().x * TileSize() + TileSize() * 0.5f;
	exit.y = mMapGenerator->GetEnd().y * TileSize() + TileSize() * 0.5f;

	return exit;
}

void Map::Update()
{
	if (Square::InputHandler::Instance().KeyPressed(SDL_SCANCODE_SPACE))
		BuildMap();
}

void Map::Render()
{
	for (const auto& tile : mMap)
		tile->Render();
}
