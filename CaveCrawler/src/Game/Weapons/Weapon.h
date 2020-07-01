#pragma once

#include "Game/Projectiles/ProjectileManager.h"

class Weapon : public Square::GameObject
{
private:
	Square::Timer& mTimer;

protected:
	Square::Audio& mAudio;

	Square::Sprite* mSprite;
	std::string mName;

	std::shared_ptr<Map> mMap;

	Square::Vector2 mWeaponOffset;
	Square::Vector2 mBulletOffset;

	int mIndex;

	bool mAutomatic;

	int mDamage;

	int mMaxAmmo;
	int mAmmo;
	int mClipSize;
	int mClip;

	float mFireDelay;
	float mFireTimer;

	float mReloadDelay;
	float mReloadTimer;

	virtual void HandleReload();

public:
	Weapon(std::shared_ptr<Map> map, Square::Vector2 weaponOffset = Square::VEC2_ZERO);
	virtual ~Weapon();

	inline int Index() const { return mIndex; }

	inline bool Automatic() const { return mAutomatic; }

	inline std::string Name() const { return mName; }

	inline int Ammo() const { return mAmmo; }
	inline int Clip() const { return mClip; }
	inline int ClipSize() const { return mClipSize; }

	inline void GiveAmmo() { mAmmo = mMaxAmmo; }

	virtual void CancelReload() = 0;
	virtual void Reload() = 0;
	virtual bool Fire() = 0;
	virtual void Upgrade() = 0;

	inline void Aim(float angle) { Rotation(angle); }

	void Update();
	void Render();

	virtual Weapon* Clone() const = 0;
};