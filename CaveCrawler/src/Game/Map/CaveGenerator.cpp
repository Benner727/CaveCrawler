#include "CaveGenerator.h"

CaveGenerator::CaveGenerator(int width, int height, int fillPercent, int smoothingIterations)
	: mWidth(width), mHeight(height), mFillPercent(fillPercent), mSmoothingIterations(smoothingIterations)
{
	mGenerationComplete = false;
}

CaveGenerator::~CaveGenerator()
{
}


bool CaveGenerator::InsideMap(int x, int y)
{
	return (x >= 0 && x < mWidth && y >= 0 && y < mHeight);
}

int CaveGenerator::GetSurroundingWallCount(int x, int y)
{
	int wallCount = 0;

	for (int neighborX = x - 2; neighborX <= x + 2; neighborX++)
	{
		for (int neighborY = y - 2; neighborY <= y + 2; neighborY++)
		{
			if (InsideMap(neighborX, neighborY))
			{
				if (x != neighborX || y != neighborY)
				{
					if (mMap[neighborX + neighborY * mWidth] == WALL)
						wallCount++;
				}
			}
			else
				wallCount++;
		}
	}

	return wallCount;
}

std::vector<Point> CaveGenerator::GetRegionTiles(int startX, int startY)
{
	std::vector<Point> tiles;
	std::vector<int> mapFlags(mWidth * mHeight, 0);
	int tileType = mMap[startX + startY * mWidth];

	std::queue<Point> queue;
	queue.push(Point(startX, startY));
	mapFlags[startX + startY * mWidth] = 1;

	while (!queue.empty())
	{
		Point tile = queue.front();
		queue.pop();

		tiles.push_back(tile);

		for (int y = tile.y - 1; y <= tile.y + 1; y++)
		{
			for (int x = tile.x - 1; x <= tile.x + 1; x++)
			{
				if (InsideMap(x, y) && (x == tile.x || y == tile.y))
				{
					if (mapFlags[x + y * mWidth] == 0 && mMap[x + y * mWidth] == tileType)
					{
						mapFlags[x + y * mWidth] = 1;
						queue.push(Point(x, y));
					}
				}
			}
		}
	}

	return tiles;
}

std::vector<std::vector<Point>> CaveGenerator::GetRegions(int tileType)
{
	std::vector<std::vector<Point>> regions;
	std::vector<int> mapFlags(mWidth * mHeight, 0);

	for (int y = 0; y < mHeight; y++)
	{
		for (int x = 0; x < mWidth; x++)
		{
			if (mapFlags[x + y * mWidth] == 0 && mMap[x + y * mWidth] == tileType)
			{
				std::vector<Point> newRegion = GetRegionTiles(x, y);
				regions.push_back(newRegion);

				for (Point tile : newRegion)
					mapFlags[tile.x + tile.y * mWidth] = 1;
			}
		}
	}

	return regions;
}

void CaveGenerator::CreateEntryPoints(std::vector<Point> region)
{
	for (int i = 0; i < region.size(); i++)
	{
		bool tileFound = true;

		for (int y = -3; y <= 3; y++)
		{
			for (int x = -3; x <= 3; x++)
			{
				if (!InsideMap((region[i].x + x), (region[i].y + y)) || mMap[(region[i].x + x) + (region[i].y + y) * mWidth] != 0)
				{
					tileFound = false;
					break;
				}
			}
			if (!tileFound) break;
		}

		if (tileFound)
		{
			mStart = Point(region[i].x, region[i].y);
			break;
		}
	}

	for (int i = region.size() - 1; i >= 0; i--)
	{
		bool tileFound = true;

		for (int y = -3; y <= 3; y++)
		{
			for (int x = -3; x <= 3; x++)
			{
				if (!InsideMap((region[i].x + x), (region[i].y + y)) || mMap[(region[i].x + x) + (region[i].y + y) * mWidth] != 0)
				{
					tileFound = false;
					break;
				}
			}
			if (!tileFound) break;
		}

		if (tileFound)
		{
			mEnd = Point(region[i].x, region[i].y);
			break;
		}
	}

	if (mRand.Int(0, 100) > 50)
	{
		Point temp = mStart;
		mStart = mEnd;
		mEnd = temp;
	}
}

void CaveGenerator::RandomFillMap()
{
	mMap.clear();

	for (int y = 0; y < mHeight; y++)
	{
		for (int x = 0; x < mWidth; x++)
		{
			if (x == 0 || x == mWidth - 1 || y == 0 || y == mHeight - 1)
				mMap.push_back(WALL);
			else
				mMap.push_back(((mRand.Int(0, 100) < mFillPercent) ? WALL : EMPTY));
		}
	}
}

void CaveGenerator::SmoothMap()
{
	std::vector<int> tempMap = mMap;

	for (int y = 0; y < mHeight; y++)
	{
		for (int x = 0; x < mWidth; x++)
		{
			int neighborWallCount = GetSurroundingWallCount(x, y);

			if (neighborWallCount > 12)
				tempMap[x + y * mWidth] = WALL;
			else if (neighborWallCount < 12)
				tempMap[x + y * mWidth] = EMPTY;
		}
	}
	
	mMap = tempMap;
}

void CaveGenerator::TrimMap()
{
	static int wallThresholdSize = 25;
	std::vector<std::vector<Point>> wallRegions = GetRegions(WALL);

	for (std::vector<Point> wallRegion : wallRegions)
	{
		if (wallRegion.size() < wallThresholdSize)
		{
			for (Point tile : wallRegion)
				mMap[tile.x + tile.y * mWidth] = EMPTY;
		}
	}

	std::vector<std::vector<Point>> roomRegions = GetRegions(EMPTY);
	std::sort(roomRegions.begin(), roomRegions.end(),
		[](const std::vector<Point>& a, const std::vector<Point>& b) {
			return a.size() < b.size();
		});

	mGenerationComplete = (roomRegions.back().size() >= 1500);

	CreateEntryPoints(roomRegions.back());
	roomRegions.pop_back();

	for (std::vector<Point> roomRegion : roomRegions)
	{
		for (Point tile : roomRegion)
			mMap[tile.x + tile.y * mWidth] = WALL;
	}
}

std::vector<int> CaveGenerator::GenerateMap(int seed)
{
	mRand.Seed(seed);

	mGenerationComplete = false;

	while (!mGenerationComplete)
	{
		RandomFillMap();

		for (int i = 0; i < mSmoothingIterations; i++)
			SmoothMap();

		TrimMap();
	}

	return mMap;
}