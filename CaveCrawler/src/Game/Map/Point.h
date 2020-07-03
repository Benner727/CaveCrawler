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

	inline void Translate(int dir)
	{
		switch (dir)
		{
		case Direction::north:
			y--;
			break;
		case Direction::south:
			y++;
			break;
		case Direction::west:
			x--;
			break;
		case Direction::east:
			x++;
			break;
		case Direction::northeast:
			y--;
			x++;
			break;
		case Direction::northwest:
			y--;
			x--;
			break;
		case Direction::southeast:
			y++;
			x++;
			break;
		case Direction::southwest:
			y++;
			x--;
			break;
		}
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