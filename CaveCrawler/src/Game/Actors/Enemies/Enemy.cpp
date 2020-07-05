#include "Enemy.h"

Enemy::Enemy(std::shared_ptr<Map> map, std::shared_ptr<PathFinder> pathFinder, Square::GameObject* target)
	: Actor(map), mPathFinder(pathFinder), mTarget(target),
	mAudio(Square::Audio::Instance()), mTimer(Square::Timer::Instance())
{
	mSprite = nullptr;

	mSpeed = 50.0f;
	mHorizontalSpeed = 0.0f;
	mVerticalSpeed = 0.0f;

	mFindNewWaypoint = true;

	mHealth = 100;
	mDamage = 1;
}

Enemy::~Enemy()
{
	delete mSprite;
}

bool Enemy::IgnoreCollisions()
{
	return Dead();
}

void Enemy::Hit(PhysEntity* other)
{
	if (other == nullptr)
		return;

	if (other->CollisionLayer() == PhysEntity::CollisionLayers::FriendlyProjectiles)
	{
		if (std::find(mHitBy.begin(), mHitBy.end(), other->Id()) != mHitBy.end())
			return;

		mHitBy.push_back(other->Id());

		mSprite->Flash();

		mHealth -= other->Damage();
	}
}

void Enemy::HandleMovement()
{
	mHorizontalSpeed = 0.0f;
	mVerticalSpeed = 0.0f;

	if (mTarget == nullptr)
		return;

	if ((Pos() - mTarget->Pos()).Magnitude() <= mSprite->ScaledDimensions().x)
		return; //Attack

	HandlePathing();
}

void Enemy::HandleAttack()
{

}

void Enemy::HandleCollision()
{
	Translate(Square::VEC2_RIGHT * mHorizontalSpeed);
	if (HasLeftWall() || HasRightWall())
	{
		Pos(PrevPos());
		do
		{
			Translate(Square::VEC2_RIGHT * Square::sgn(mHorizontalSpeed));
			mHorizontalSpeed -= Square::sgn(mHorizontalSpeed);
		} while (!HasLeftWall() && !HasRightWall());
		Pos(PrevPos());

		mHorizontalSpeed = 0.0f;
	}

	Translate(Square::VEC2_UP * mVerticalSpeed);
	if (HasGround() || HasCeiling())
	{
		Pos(PrevPos());
		do
		{
			Translate(Square::VEC2_UP * Square::sgn(mVerticalSpeed));
			mHorizontalSpeed -= Square::sgn(mVerticalSpeed);
		} while (!HasGround() && !HasCeiling());
		Pos(PrevPos());

		mVerticalSpeed = 0.0f;
	}

	if (OutsideMap())
		; // Kill enemy?
}

void Enemy::HandlePathing()
{
	Square::Vector2 direction;

	if (mTarget != nullptr && mPathFinder->InLos(Pos(), Size()))
	{
		direction = (mTarget->Pos(GameObject::world) - Pos(GameObject::world)).Normalize();
		mFindNewWaypoint = true;
	}
	else
	{
		if (mFindNewWaypoint)
		{
			Point p = mPathFinder->NextWaypoint(Pos(), Size());
			mWaypoint = Square::Vector2(p.x * mMap->TileSize() + mMap->TileSize() * 0.5f, p.y * mMap->TileSize() + mMap->TileSize() * 0.5f);
			
			mFindNewWaypoint = false;
		}

		if ((Pos() - mWaypoint).Magnitude() >= mSpeed * Square::Timer::Instance().DeltaTime())
			direction = (mWaypoint - Pos(Square::GameObject::world)).Normalize();
		else
			mFindNewWaypoint = true;
	}

	float angle = atan2(direction.y, direction.x);

	//Translate(Square::RotateVector(mSpeed * Square::Timer::Instance().DeltaTime(), angle));
	mHorizontalSpeed = cos(angle) * mSpeed * mTimer.DeltaTime();
	mVerticalSpeed = sin(angle) * -mSpeed * mTimer.DeltaTime();
}

void Enemy::Update()
{
	HandleMovement();
	HandleCollision();
	HandleAttack();
}

void Enemy::Render()
{
	if (mMap->InFOV(this))
		mSprite->Render();

	PhysEntity::Render();
}