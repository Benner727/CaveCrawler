#pragma once

#include "Game/Actors/Actor.h"
#include "Game/Actors/PathFinder.h"
#include "Engine/Square.h"

#include <random>
#include <ctime>

class Enemy : public Actor
{
private:
	std::shared_ptr<PathFinder> mPathFinder;

	float mHorizontalSpeed;
	float mVerticalSpeed;

protected:
	Square::Audio& mAudio;
	Square::Timer& mTimer;

	Square::Sprite* mSprite;

	Square::GameObject* mTarget;
	Square::Vector2 mWaypoint;
	bool mFindNewWaypoint;

	float mSpeed;
	int mFullHealth;

	virtual void HandleMovement();
	virtual void HandleAttack();
	virtual void HandleCollision();
	virtual void HandlePathing();

public:
	Enemy(std::shared_ptr<Map> map, std::shared_ptr<PathFinder> pathFinder, Square::GameObject* target = nullptr);
	virtual ~Enemy();

	bool IgnoreCollisions() override;
	void Hit(PhysEntity* other) override;

	inline void Damage(int damage) { mDamage = damage; }
	inline void Health(int health) { mHealth = health; }
	inline int Health() const { return mHealth; }

	inline bool Dead() const { return mHealth < 1; }

	inline Square::Vector2 Size() const { return mSprite->ScaledDimensions(); }

	void Update();
	void Render();
};