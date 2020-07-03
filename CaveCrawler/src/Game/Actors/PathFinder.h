#pragma once

#include <list>
#include <algorithm>

#include "Game/Map/Map.h"

class PathFinder
{
private:
	struct Node
	{
		Point position;
		Point parent;
		int distance;
		int cost;

		bool operator == (const Node& other)
		{
			return position == other.position;
		}

		bool operator == (const Point& other)
		{
			return position == other;
		}

		bool operator < (const Node& other)
		{
			return distance < other.distance;
		}

		Node()
		{
			cost = std::numeric_limits<int>::infinity();
			distance = std::numeric_limits<int>::infinity();
		}
	};

	const int ORTHOGONAL_COST = 10;
	const int DIAGONAL_COST = 14;
	const int MAX_COST = 320; // 32 tiles orthogonally

	const Map& mMap;
	Point mStart;
	Point mEnd;
	std::list<Node> mOpenNodes;
	std::list<Node> mClosedNodes;

	bool mUseDiagonals;

	bool CanWalkTo(Point p, int direction);
	bool FillOpenNodes(Node& n);
	bool UnviablePoint(Point p, int cost);
	int CalculateDistanceToEnd(Point p);

public:
	PathFinder(const Map& map);
	~PathFinder() = default;

	std::list<Point> GeneratePath(Point source, Point destination, bool useDiagonals = true);
};