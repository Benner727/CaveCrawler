#include "Projectile.h"

Projectile::Projectile(std::shared_ptr<Map> map, Square::Vector2 pos, float rotation, bool shell)
	: Actor(map), mShell(shell), mTimer(Square::Timer::Instance())
{
	mHorizontalSpeed = 0.0f;
	mVerticalSpeed = 0.0f;

	mSpeed = 500.0f;

	mPenetrates = false;
	mReduceDamage = false;

	mSprite = nullptr;
}

Projectile::~Projectile()
{
	delete mSprite;
}

void Projectile::Hit(PhysEntity* other)
{
	if (other == nullptr)
	{
		Active(false);
		return;
	}

	if (std::find(mHitBy.begin(), mHitBy.end(), other->Id()) != mHitBy.end())
		return;

	mHitBy.push_back(other->Id());

	//Penetration chance
	if (!mPenetrates || !(rand() % 3))
		Active(false);
	else
		mReduceDamage = true;
}

bool Projectile::CleanUp()
{
	return !Active();
}

void Projectile::Update()
{
	if (Active())
	{
		mSprite->Update();

		mHorizontalSpeed = mSpeed * mTimer.DeltaTime();

		Translate(Square::VEC2_RIGHT * mHorizontalSpeed, local);

		if (mReduceDamage)
		{
			mDamage *= mShell ? 0.5f : 0.75f;
			mReduceDamage = false;
		}

		if (HasLeftWall() || HasRightWall() || HasCeiling() || HasGround() || InsideTile())
		{
			//Active(false);
			Hit(nullptr);
		}
	}
}

void Projectile::Render()
{
	if (Active())
		mSprite->Render();

	PhysEntity::Render();
}
