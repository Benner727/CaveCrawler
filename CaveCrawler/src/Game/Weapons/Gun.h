#pragma once

#include "Game/Weapons/Weapon.h"

#include <iomanip>

class Gun : public Weapon
{
public:
	Gun(std::shared_ptr<Map> map, Square::Vector2 weaponOffset = Square::VEC2_ZERO);
	~Gun();

private:
	bool mPenetrates;

	int mType; //1=Single, 2=Burst, 3=Spread

	bool mSingleReload;
	bool mReloading;

	void HandleReload();
	void HandleShot();

public:
	void CancelReload();
	void Reload();
	bool Fire();
	void Upgrade();

	friend std::istream& operator>>(std::istream& is, Gun& gun);

	virtual Gun* Clone() const override;
};