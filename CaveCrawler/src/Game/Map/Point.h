#pragma once

enum Direction {
	north,
	east,
	south,
	west,
	northeast,
	northwest,
	southeast,
	southwest,
	TOTAL_DIRECTIONS
};

struct Point 
{
	int x, y;

	Point(int x = 0, int y = 0)
		: x(x), y(y) {}

	inline Point Translate(int dir)
	{
		Point newPoint = *this;

		switch (dir)
		{
		case Direction::north:
			newPoint.y--;
			break;
		case Direction::south:
			newPoint.y++;
			break;
		case Direction::west:
			newPoint.x--;
			break;
		case Direction::east:
			newPoint.x++;
			break;
		case Direction::northeast:
			newPoint.y--;
			newPoint.x++;
			break;
		case Direction::northwest:
			newPoint.y--;
			newPoint.x--;
			break;
		case Direction::southeast:
			newPoint.y++;
			newPoint.x++;
			break;
		case Direction::southwest:
			newPoint.y++;
			newPoint.x--;
			break;
		}

		return newPoint;
	}

	bool operator < (const Point& rhs) const
	{
		return x != rhs.x ? x < rhs.x : y < rhs.y;
	}

	bool operator == (const Point& other)
	{
		return x == other.x && y == other.y;
	}

	bool operator != (const Point& other)
	{
		return x != other.x || y != other.y;
	}

	Point operator + (const Point& other)
	{
		return Point(x + other.x, y + other.y);
	}
};