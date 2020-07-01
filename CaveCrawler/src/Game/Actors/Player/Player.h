#pragma once

#include "Game/Weapons/Weapon.h"

class Player : public Actor
{
private:
	Square::InputHandler& mInput;
	Square::Timer& mTimer;

	Square::Sprite* mSprite;

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

public:
	Player(std::shared_ptr<Map> map);
	~Player();

	void Hit(PhysEntity* other) override;

	inline bool Dead() const { return (mHealth <= 0); }

	void Update();
	void Render();
};