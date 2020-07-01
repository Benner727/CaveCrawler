#include "Gun.h"

Gun::Gun(std::shared_ptr<Map> map, Square::Vector2 weaponOffset)
	: Weapon(map, weaponOffset)
{
	Pos(mWeaponOffset);

	mIndex = 0;

	mAutomatic = false;

	mName = "9mm Pistol";

	mMaxAmmo = 75;
	mAmmo = mMaxAmmo;
	mClipSize = 15;
	mClip = mClipSize;

	mDamage = 25;

	mPenetrates = false;

	mType = 3;

	mFireDelay = 0.2f;
	mFireTimer = 0.0f;

	mReloadDelay = 2.13f;
	mReloadTimer = 0.0f;

	mReloading = false;
	mSingleReload = true;
}

Gun::~Gun()
{
}

void Gun::HandleReload()
{
	if (mReloading && mReloadTimer <= 0.0f)
	{
		if (mSingleReload)
		{
			int reloadAmount = mClipSize - mClip;

			if (reloadAmount <= mAmmo)
			{
				mAmmo -= reloadAmount;
				mClip = mClipSize;
			}
			else
			{
				mClip += mAmmo;
				mAmmo = 0;
			}
		}
		else if (mClip < mClipSize && mAmmo > 0)
		{
			mClip++;
			mAmmo--;
			mReloadTimer = mReloadDelay;
		}
	}
}

void Gun::CancelReload()
{
	mReloadTimer = 0.0f;
	mReloading = false;
}

void Gun::Reload()
{
	if (mReloading)
		return;

	if (mAmmo > 0)
	{
		if (mClip != mClipSize)
		{
			mReloading = true;
			mReloadTimer = mReloadDelay;

			if (!mSingleReload)
				mReloadTimer *= 3.0f;
		}
	}
}

bool Gun::Fire()
{
	if (mClip > 0)
	{
		CancelReload();

		if (mFireTimer <= 0.0f)
		{
			Square::Graphics::Instance().CameraShake();

			switch (mType)
			{
			case 1: //Single Shot
				break;
			case 2: //Double Shot
				break;
			case 3: //Triple Shot
				break;
			case 4: //Explosive
				break;
			}

			mFireTimer = mFireDelay;

			mClip--;
			if (mClip < 1)
			{
				Reload();
			}
		}

		return true;
	}
	else if (!mReloading)
		Reload();

	return false;
}

void Gun::Upgrade()
{
	CancelReload();

	mDamage *= 2.0f;
	mClip = mClipSize;
	mMaxAmmo *= 1.5f;
	mAmmo = mMaxAmmo;
}

Gun* Gun::Clone() const
{
	return new Gun(*this);
}

std::istream& operator>>(std::istream& is, Gun& gun)
{
	is >> gun.mIndex >> std::quoted(gun.mName)
		>> gun.mAutomatic >> gun.mClipSize >> gun.mMaxAmmo
		>> gun.mDamage >> gun.mFireDelay >> gun.mReloadDelay
		>> gun.mType >> gun.mSingleReload >> gun.mPenetrates;

	gun.mClip = gun.mClipSize;
	gun.mAmmo = gun.mMaxAmmo;

	return is;
}