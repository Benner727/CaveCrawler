#pragma once

#include "Game/Actors/Actor.h"

class Projectile : public Actor
{
private:
	float mHorizontalSpeed;
	float mVerticalSpeed;

protected:
	Square::Timer& mTimer;

	Square::AnimatedSprite* mSprite;

	float mSpeed;

	bool mPenetrates;
	bool mReduceDamage;
	bool mShell;

public:
	Projectile(std::shared_ptr<Map> map, Square::Vector2 pos, float rotation, bool shell = false);
	virtual ~Projectile();

	void Hit(PhysEntity* other) override;

	bool CleanUp();

	void Update();
	void Render();
};