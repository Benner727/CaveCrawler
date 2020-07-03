#pragma once

#include <fstream>

#include "Game/Weapons/Gun.h"

class WeaponLoader
{
private:
	std::shared_ptr<Map> mMap;

	std::vector<Weapon*> mWeapons;

	void LoadGuns();

public:
	WeaponLoader(std::shared_ptr<Map> map);
	~WeaponLoader();

	Weapon* GetWeapon(int index);
	int TotalWeapons() const { return mWeapons.size(); }
};