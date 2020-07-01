#pragma once

#include <queue>

#include "Game/Random.h"

struct Point {
	int x, y;

	Point(int x = 0, int y = 0)
		: x(x), y(y) {}
};

class CaveGenerator
{
public:
	CaveGenerator(int width, int height, int fillPercent = 48, int smoothingIterations = 6);
	~CaveGenerator();

private:
	static const int EMPTY = 0;
	static const int WALL = 1;

	int mWidth;
	int mHeight;

	Random mRand;

	int mFillPercent; // Chance for initial tiles to be wall [0, 100]
	int mSmoothingIterations;

	bool mGenerationComplete;

	std::vector<int> mMap;
	Point mStart;
	Point mEnd;

	bool InsideMap(int x, int y);
	int GetSurroundingWallCount(int x, int y);

	std::vector<Point> GetRegionTiles(int startX, int startY);
	std::vector<std::vector<Point>> GetRegions(int tileType);

	void CreateEntryPoints(std::vector<Point> region);

	void RandomFillMap();
	void SmoothMap();
	void TrimMap();

public:
	std::vector<int> GenerateMap(int seed);

	Point GetStart() const { return mStart; }
	Point GetEnd() const { return mEnd; }
};