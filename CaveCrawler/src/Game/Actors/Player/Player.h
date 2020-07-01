#pragma once

#include "Game/Actors/Actor.h"
#include "Engine/Square.h"

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

	void HandleMovement();
	void HandleCollision();

public:
	Player(std::shared_ptr<Map> map);
	~Player();

	void Update();
	void Render();
};