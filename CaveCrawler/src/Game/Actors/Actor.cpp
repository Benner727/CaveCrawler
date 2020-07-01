#include "Actor.h"

Actor::Actor(std::shared_ptr<Map> map)
	: mMap(map)
{
	
}

bool Actor::IgnoreCollisions()
{
	return !Active();
}

bool Actor::HasGround()
{
	if (mMap == nullptr)
		return false;

	float floor = 0.0f;
	Square::Vector2 range = Square::VEC2_ZERO;

	for (int i = 0; i < mColliders.size(); i++)
	{
		if (mColliders[i]->GetType() == Square::Collider::ColliderType::Box)
		{
			for (int j = 0; j < 4; j++)
			{
				Square::Vector2 pos = static_cast<Square::BoxCollider*>(mColliders[i])->GetVertexPos(j);

				if ((i == 0 && j == 0) || pos.y > floor)
					floor = pos.y;

				if ((i == 0 && j == 0) || pos.x < range.x)
					range.x = pos.x;
				if ((i == 0 && j == 0) || pos.x > range.y)
					range.y = pos.x;
			}
		}
		else if (mColliders[i]->GetType() == Square::Collider::ColliderType::Circle)
		{
			Square::Vector2 pos = static_cast<Square::CircleCollider*>(mColliders[i])->Pos();
			float r = static_cast<Square::CircleCollider*>(mColliders[i])->GetRadius();

			if (i == 0 || pos.y + r > floor)
				floor = pos.y + r;

			if (i == 0 || pos.x - r < range.x)
				range.x = pos.x - r;
			if (i == 0 || pos.x + r > range.y)
				range.y = pos.x + r;
		}
	}

	bool hit = false;
	int tileX, tileY;
	for (float x = range.x; ; x += mMap->TileSize())
	{
		x = fmin(x, range.y);

		tileX = x / mMap->TileSize();
		tileY = floor / mMap->TileSize();

		if (!mMap->IsWalkable(tileX, tileY))
		{
			if (mMap->Collides(this, tileX, tileY))
			{
				hit = true;
				break;
			}
		}

		if (x >= range.y)
			break;
	}

	return hit;
}

bool Actor::HasCeiling()
{
	if (mMap == nullptr)
		return false;

	float ceiling = 0.0f;
	Square::Vector2 range = Square::VEC2_ZERO;

	for (int i = 0; i < mColliders.size(); i++)
	{
		if (mColliders[i]->GetType() == Square::Collider::ColliderType::Box)
		{
			for (int j = 0; j < 4; j++)
			{
				Square::Vector2 pos = static_cast<Square::BoxCollider*>(mColliders[i])->GetVertexPos(j);

				if ((i == 0 && j == 0) || pos.y < ceiling)
					ceiling = pos.y;

				if ((i == 0 && j == 0) || pos.x < range.x)
					range.x = pos.x;
				if ((i == 0 && j == 0) || pos.x > range.y)
					range.y = pos.x;
			}
		}
		else if (mColliders[i]->GetType() == Square::Collider::ColliderType::Circle)
		{
			Square::Vector2 pos = static_cast<Square::CircleCollider*>(mColliders[i])->Pos();
			float r = static_cast<Square::CircleCollider*>(mColliders[i])->GetRadius();

			if (i == 0 || pos.y - r < ceiling)
				ceiling = pos.y - r;

			if (i == 0 || pos.x - r < range.x)
				range.x = pos.x - r;
			if (i == 0 || pos.x + r > range.y)
				range.y = pos.x + r;
		}
	}

	bool hit = false;
	int tileX, tileY;
	for (float x = range.x; ; x += mMap->TileSize())
	{
		x = fmin(x, range.y);

		tileX = x / mMap->TileSize();
		tileY = ceiling / mMap->TileSize();

		if (!mMap->IsWalkable(tileX, tileY))
		{
			if (mMap->Collides(this, tileX, tileY))
			{
				hit = true;
				break;
			}
		}

		if (x >= range.y)
			break;
	}

	return hit;
}

bool Actor::HasLeftWall()
{
	if (mMap == nullptr)
		return false;

	float left = 0.0f;
	Square::Vector2 range = Square::VEC2_ZERO;

	for (int i = 0; i < mColliders.size(); i++)
	{
		if (mColliders[i]->GetType() == Square::Collider::ColliderType::Box)
		{
			for (int j = 0; j < 4; j++)
			{
				Square::Vector2 pos = static_cast<Square::BoxCollider*>(mColliders[i])->GetVertexPos(j);

				if ((i == 0 && j == 0) || pos.x < left)
					left = pos.x;

				if ((i == 0 && j == 0) || pos.y < range.x)
					range.x = pos.y;
				if ((i == 0 && j == 0) || pos.y > range.y)
					range.y = pos.y;
			}
		}
		else if (mColliders[i]->GetType() == Square::Collider::ColliderType::Circle)
		{
			Square::Vector2 pos = static_cast<Square::CircleCollider*>(mColliders[i])->Pos();
			float r = static_cast<Square::CircleCollider*>(mColliders[i])->GetRadius();

			if (i == 0 || pos.x - r < left)
				left = pos.x - r;

			if (i == 0 || pos.y - r < range.x)
				range.x = pos.y - r;
			if (i == 0 || pos.y + r > range.y)
				range.y = pos.y + r;
		}
	}

	bool hit = false;
	int tileX, tileY;
	for (float y = range.x; ; y += mMap->TileSize())
	{
		y = fmin(y, range.y);

		tileX = left / mMap->TileSize();
		tileY = y / mMap->TileSize();

		if (!mMap->IsWalkable(tileX, tileY))
		{
			if (mMap->Collides(this, tileX, tileY))
			{
				hit = true;
				break;
			}
		}

		if (y >= range.y)
			break;
	}

	return hit;
}

bool Actor::HasRightWall()
{
	if (mMap == nullptr)
		return false;

	float right = 0.0f;
	Square::Vector2 range = Square::VEC2_ZERO;

	for (int i = 0; i < mColliders.size(); i++)
	{
		if (mColliders[i]->GetType() == Square::Collider::ColliderType::Box)
		{
			for (int j = 0; j < 4; j++)
			{
				Square::Vector2 pos = static_cast<Square::BoxCollider*>(mColliders[i])->GetVertexPos(j);

				if ((i == 0 && j == 0) || pos.x > right)
					right = pos.x;

				if ((i == 0 && j == 0) || pos.y < range.x)
					range.x = pos.y;
				if ((i == 0 && j == 0) || pos.y > range.y)
					range.y = pos.y;
			}
		}
		else if (mColliders[i]->GetType() == Square::Collider::ColliderType::Circle)
		{
			Square::Vector2 pos = static_cast<Square::CircleCollider*>(mColliders[i])->Pos();
			float r = static_cast<Square::CircleCollider*>(mColliders[i])->GetRadius();

			if (i == 0 || pos.x + r > right)
				right = pos.x + r;

			if (i == 0 || pos.y - r < range.x)
				range.x = pos.y - r;
			if (i == 0 || pos.y + r > range.y)
				range.y = pos.y + r;
		}
	}

	bool hit = false;
	int tileX, tileY;
	for (float y = range.x; ; y += mMap->TileSize())
	{
		y = fmin(y, range.y);

		tileX = right / mMap->TileSize();
		tileY = y / mMap->TileSize();

		if (!mMap->IsWalkable(tileX, tileY))
		{
			if (mMap->Collides(this, tileX, tileY))
			{
				hit = true;
				break;
			}
		}

		if (y >= range.y)
			break;
	}

	return hit;
}

bool Actor::InsideTile()
{
	bool hit = false;
	int tileX = Pos().x / mMap->TileSize(),
		tileY = Pos().y / mMap->TileSize();

	if (!mMap->IsWalkable(tileX, tileY))
	{
		if (mMap->Collides(this, tileX, tileY))
		{
			hit = true;
		}
	}

	return hit;
}

bool Actor::OutsideMap()
{
	int tileX = Pos().x / mMap->TileSize();
	int tileY = Pos().y / mMap->TileSize();

	return mMap->IsBoundary(tileX, tileY);
}

void Actor::Update()
{
}

void Actor::Render()
{
}