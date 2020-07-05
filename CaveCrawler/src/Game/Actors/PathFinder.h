#pragma once

#include <list>
#include <algorithm>
#include <limits>

#include "Game/Map/Map.h"

class PathFinder
{
private:
	struct Node
	{
		Point position;
		Point parent;

		bool operator == (const Node& other)
		{
			return position == other.position;
		}

		bool operator == (const Point& other)
		{
			return position == other;
		}

		Node()
		{
		}
	};

	std::shared_ptr<Map> mMap;
	std::shared_ptr<Square::GameObject> mTarget;

	bool mUseDiagonals = true;

	Point mSource;
	std::vector<int> mDijkstraGrid;
	std::vector<bool> mLosGrid;
	std::map<Point, Point> mFlowField;

	int mFarthestDistance;

	bool PathFinder::CanWalkTo(Point p, int direction);

	void CalculateLos(Point pos);

	void GenerateDijkstraGrid();
	void GenerateFlowField();

	void GeneratePaths();

public:
	PathFinder(std::shared_ptr<Map> map, std::shared_ptr<Square::GameObject> target);
	~PathFinder() = default;

	Point NextWaypoint(Square::Vector2 source, Square::Vector2 size);
	bool InLos(Square::Vector2 source, Square::Vector2 size);

	void Update();
	void Render();
};