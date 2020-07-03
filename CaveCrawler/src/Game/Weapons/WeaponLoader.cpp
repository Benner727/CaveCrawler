#include "WeaponLoader.h"

WeaponLoader::WeaponLoader(std::shared_ptr<Map> map)
	: mMap(map)
{
	LoadGuns();
}

WeaponLoader::~WeaponLoader()
{
	for (auto& weapon : mWeapons)
		delete weapon;
}

void WeaponLoader::LoadGuns()
{
	std::string fullPath = Square::AssetManager::Instance().GetBasePath();
	fullPath.append("Assets/guns.txt");
	std::ifstream infile(fullPath);
	if (!infile) std::cerr << "guns.txt could not be opened!" << std::endl;

	Gun gun = Gun(mMap);
	while (infile >> gun)
	{
		mWeapons.push_back(new Gun(gun));
	}
}

Weapon* WeaponLoader::GetWeapon(int index)
{
	for (auto& weapon : mWeapons)
	{
		if (weapon->Index() == index)
			return weapon->Clone();
	}

	return nullptr;
}
