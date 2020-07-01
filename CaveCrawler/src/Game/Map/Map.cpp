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
