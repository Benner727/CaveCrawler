#include "PathFinder.h"

bool PathFinder::CanWalkTo(Point p, int direction)
{
	switch (direction)
	{
	case Direction::north:
		return mMap->IsWalkable(p.x, p.y -1);
		break;
	case Direction::east:
		return mMap->IsWalkable(p.x + 1, p.y);
		break;
	case Direction::south:
		return mMap->IsWalkable(p.x, p.y + 1);
		break;
	case Direction::west:
		return mMap->IsWalkable(p.x - 1, p.y);
		break;
	case Direction::northeast:
		return mMap->IsWalkable(p.x, p.y - 1) && mMap->IsWalkable(p.x + 1, p.y) && mMap->IsWalkable(p.x + 1, p.y - 1);
		break;
	case Direction::northwest:
		return mMap->IsWalkable(p.x, p.y - 1) && mMap->IsWalkable(p.x - 1, p.y) && mMap->IsWalkable(p.x - 1, p.y - 1);
		break;
	case Direction::southeast:
		return mMap->IsWalkable(p.x, p.y + 1) && mMap->IsWalkable(p.x + 1, p.y) && mMap->IsWalkable(p.x + 1, p.y + 1);
		break;
	case Direction::southwest:
		return mMap->IsWalkable(p.x, p.y + 1) && mMap->IsWalkable(p.x - 1, p.y) && mMap->IsWalkable(p.x - 1, p.y + 1);
		break;
	}
}

void PathFinder::CalculateLos(Point pos)
{
	int xDif = mSource.x - pos.x;
	int yDif = mSource.y - pos.y;

	int xDifAbs = abs(xDif);
	int yDifAbs = abs(yDif);

	bool hasLos = false;

	int xDifOne = Square::sgn(xDif);
	int yDifOne = Square::sgn(yDif);

	if (xDifAbs >= yDifAbs)
	{
		if (mLosGrid[(pos.x + xDifOne) + pos.y * mMap->Width()])
			hasLos = true;
	}

	if (yDifAbs >= xDifAbs)
	{
		if (mLosGrid[pos.x + (pos.y + yDifOne) * mMap->Width()])
			hasLos = true;
	}

	if (yDifAbs > 0 && xDifAbs > 0) 
	{
		if (!mLosGrid[(pos.x + xDifOne) + (pos.y + yDifOne) * mMap->Width()])
		{
			hasLos = false;
		}
		else if (yDifAbs == xDifAbs) 
		{
			if (mDijkstraGrid[(pos.x + xDifOne) + pos.y * mMap->Width()] == -1 || mDijkstraGrid[pos.x + (pos.y + yDifOne) * mMap->Width()] == -1)
				hasLos = false;
		}
	}

	mLosGrid[pos.x + pos.y * mMap->Width()] = hasLos;
}

void PathFinder::GenerateDijkstraGrid()
{
	mDijkstraGrid = std::vector<int>(mMap->Width() * mMap->Height(), -1);
	mLosGrid = std::vector<bool>(mMap->Width() * mMap->Height(), false);

	int total_directions = 4;
	if (mUseDiagonals) total_directions = 8;

	std::queue<Point> queue;
	int distance = 0;

	queue.push(mSource);
	mDijkstraGrid[mSource.x + mSource.y * mMap->Width()] = distance;
	mLosGrid[mSource.x + mSource.y * mMap->Width()] = true;

	while (queue.size() > 0)
	{
		Point position = queue.front();
		queue.pop();

		distance = mDijkstraGrid[position.x + position.y * mMap->Width()];

		for (int direction = 0; direction < total_directions; direction++)
		{
			Point newPosition = position.Translate(direction);
			
			if (mDijkstraGrid[newPosition.x + newPosition.y * mMap->Width()] == -1 && mMap->IsWalkable(newPosition.x, newPosition.y))
			{
				queue.push(newPosition);
				mDijkstraGrid[newPosition.x + newPosition.y * mMap->Width()] = distance + 1;
				CalculateLos(newPosition);
			}
		}
	}
}

void PathFinder::GenerateFlowField()
{
	for (int y = 0; y < mMap->Height(); y++)
	{
		for (int x = 0; x < mMap->Width(); x++)
		{
			if (mDijkstraGrid[x + y * mMap->Width()] == -1)
				continue;

			Point position(x, y);

			int total_directions = 4;
			if (mUseDiagonals) total_directions = 8;

			int dir = -1;
			int minDist = std::numeric_limits<int>::max();
			for (int direction = 0; direction < total_directions; direction++)
			{
				if (CanWalkTo(position, direction))
				{
					Point newPosition = position.Translate(direction);
					int dist = mDijkstraGrid[newPosition.x + newPosition.y * mMap->Width()] - mDijkstraGrid[position.x + position.y * mMap->Width()];

					if (dist < minDist)
					{
						minDist = dist;
						dir = direction;
					}
				}
			}

			if (dir != -1)
				mFlowField[position] = position.Translate(dir);
		}
	}
}

void PathFinder::GeneratePaths()
{
	mSource.x = (round(mTarget->Pos().x - mTarget->Size().x * 0.5f)) / mMap->TileSize();
	mSource.y = (round(mTarget->Pos().y - mTarget->Size().y * 0.5f)) / mMap->TileSize();

	mDijkstraGrid.clear();
	mLosGrid.clear();

	mFlowField.clear();

	GenerateDijkstraGrid();
	GenerateFlowField();
}

PathFinder::PathFinder(std::shared_ptr<Map> map, std::shared_ptr<Square::GameObject> target)
	: mMap(map), mTarget(target)
{
	GeneratePaths();
}


Point PathFinder::NextWaypoint(Square::Vector2 source, Square::Vector2 size)
{
	Point pos(round((source.x - size.x * 0.5f) / mMap->TileSize()), round((source.y - size.y * 0.5f) / mMap->TileSize()));

	Point newPos = mFlowField[pos];

	return newPos;
}

bool PathFinder::InLos(Square::Vector2 source, Square::Vector2 size)
{
	Point pos(round((source.x - size.x * 0.5f) / mMap->TileSize()), round((source.y - size.y * 0.5f) / mMap->TileSize()));

	bool inLos = mLosGrid[pos.x + pos.y * mMap->Width()];

	return inLos;
}

void PathFinder::Update()
{
	Point newSource;
	
	newSource.x = (round(mTarget->Pos().x - mTarget->Size().x * 0.5f)) / mMap->TileSize();
	newSource.y = (round(mTarget->Pos().y - mTarget->Size().y * 0.5f)) / mMap->TileSize();

	if (!(newSource == mSource))
		GeneratePaths();
}

void PathFinder::Render()
{

}