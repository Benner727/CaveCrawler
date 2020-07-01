#include "Weapon.h"

Weapon::Weapon(std::shared_ptr<Map> map, Square::Vector2 weaponOffset)
	: mMap(map), mWeaponOffset(weaponOffset), mTimer(Square::Timer::Instance())
{
	mSprite = nullptr;

	mBulletOffset = Square::VEC2_ZERO;
	mBulletOffset.x = 10.0f;

	mAutomatic = false;

	mName = "Weapon?";

	mDamage = 25;

	mMaxAmmo = 9999;
	mAmmo = mMaxAmmo;
	mClipSize = 50;
	mClip = mClipSize;

	mFireDelay = 0.07f;
	mFireTimer = 0.0f;

	mReloadDelay = 1.0f;
	mReloadTimer = 0.0f;
}

Weapon::~Weapon()
{
	delete mSprite;
}

void Weapon::HandleReload()
{
}

void Weapon::Update()
{
	if (mFireTimer > 0.0f)
		mFireTimer -= mTimer.DeltaTime();

	if (mReloadTimer > 0.0f)
		mReloadTimer -= mTimer.DeltaTime();

	HandleReload();
}

void Weapon::Render()
{
	if (mSprite)
		mSprite->Render();
}