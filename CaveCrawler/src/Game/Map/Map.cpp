#include "Map.h"

#include "Game/Map/FOV.h"

Map::Map(int width, int height)
	: mWidth(width), mHeight(height)
{
	mMapGenerator = new CaveGenerator(mWidth, mHeight);

	BuildMap();

	mFOV = new FOV(this);
	mFocus = nullptr;
	mVisionRadius = 21;
	mVisionTimer = 0.0f;
}

Map::~Map()
{
	delete mMapGenerator;

	ClearMap();
}

bool Map::IsBoundary(int x, int y) const
{
	if (x < 0 || y < 0)
		return true;

	if (x >= mWidth || y >= mHeight)
		return true;

	return false;
}

bool Map::IsWalkable(int x, int y) const
{
	if (IsBoundary(x, y))
		return false;

	if (mMap[x + y * mWidth] != nullptr)
		return (mMap[x + y * mWidth]->Type() == TileType::Empty);

	return true;
}

bool Map::IsOpaque(int x, int y) const
{
	return !IsWalkable(x, y);
}

bool Map::Collides(Square::PhysEntity* entity, int x, int y)
{
	if (IsBoundary(x, y))
		return true;

	return (mMap[x + y * mWidth]->CheckCollision(entity));
}

bool Map::InFOV(Square::GameObject* entity) const
{
	int x = entity->Pos().x;
	int y = entity->Pos().y;

	if (mFOV->InFOV((int)x / TileSize(), (int)y / TileSize()))
		return true;
	else
	{
		if (!IsOpaque((int)(x + TileSize() * 0.5f) / TileSize(), (int)(y + TileSize() * 0.5f) / TileSize())
			&& mFOV->InFOV((int)(x + TileSize() * 0.5f) / TileSize(), (int)(y + TileSize() * 0.5f) / TileSize()))
			return true;
		if (!IsOpaque((int)(x + TileSize() * 0.5f) / TileSize(), (int)(y - TileSize() * 0.5f) / TileSize())
			&& mFOV->InFOV((int)(x + TileSize() * 0.5f) / TileSize(), (int)(y - TileSize() * 0.5f) / TileSize()))
			return true;
		if (!IsOpaque((int)(x - TileSize() * 0.5f) / TileSize(), (int)(y + TileSize() * 0.5f) / TileSize())
			&& mFOV->InFOV((int)(x - TileSize() * 0.5f) / TileSize(), (int)(y + TileSize() * 0.5f) / TileSize()))
			return true;
		if (!IsOpaque((int)(x - TileSize() * 0.5f) / TileSize(), (int)(y - TileSize() * 0.5f) / TileSize())
			&& mFOV->InFOV((int)(x - TileSize() * 0.5f) / TileSize(), (int)(y - TileSize() * 0.5f) / TileSize()))
			return true;
	}

	return false;
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
	if (mFocus != nullptr)
	{
		if (mVisionTimer <= 0.0f)
		{
			mFOV->CalculateFOV(mFocus->Pos().x / TileSize(), mFocus->Pos().y / TileSize(), mVisionRadius);
			mVisionTimer = 1.0f;
		}
		else
			mVisionTimer -= Square::Timer::Instance().DeltaTime();
	}
}

void Map::Render()
{
	for (const auto& tile : mMap)
	{
		if (mFocus != nullptr)
		{
			if (!mFOV->InFOV(tile->Pos().x / TileSize(), tile->Pos().y / TileSize()))
			{
				if (tile->Explored())
					tile->Dim();
			}
			else
				tile->Explore();
		}
		else
			tile->Explore();
		
		if (tile->Explored())
			tile->Render();
	}
}
