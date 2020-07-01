#pragma once

#include "Game/Weapons/Weapon.h"

class Player : public Actor
{
private:
	Square::InputHandler& mInput;
	Square::Timer& mTimer;

	Square::Sprite* mSprite;

	std::vector<Weapon*> mWeapon;
	int mWeaponSlot;

	float mHorizontalSpeed;
	float mVerticalSpeed;
	float mSpeed;

	float mSprintTime;
	bool mSprinting;
	bool mSprintRecharge;

	int mFullHealth;
	float mHealTimer;
	float mImmunityTimer;

	void HandleMovement();
	void HandleCollision();
	void HandleGun();

public:
	Player(std::shared_ptr<Map> map);
	~Player();

	void Hit(PhysEntity* other) override;

	inline std::string WeaponName() const { return mWeapon[mWeaponSlot]->Name(); }
	inline int Ammo() const { return mWeapon[mWeaponSlot]->Ammo(); }
	inline int Clip() const { return mWeapon[mWeaponSlot]->Clip(); }
	inline int ClipSize() const { return mWeapon[mWeaponSlot]->ClipSize(); }

	void GiveAmmo(int id = -1);
	inline void FillAmmo();
	inline bool HasGun(int id) const;
	void GiveWeapon(Weapon* weapon);
	inline Weapon* GetWeapon() { return mWeapon[mWeaponSlot]; }

	inline bool Dead() const { return (mHealth <= 0); }

	void Update();
	void Render();
};