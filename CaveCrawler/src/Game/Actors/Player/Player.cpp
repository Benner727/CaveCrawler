#include "Player.h"

Player::Player(std::shared_ptr<Map> map)
	: Actor(map),
	mInput(Square::InputHandler::Instance()), mTimer(Square::Timer::Instance())
{
	mSprite = new Square::Sprite("HumanSprite.png");
	mSprite->Parent(this);

	mSpeed = 75.0f;
	mHorizontalSpeed = 0.0f;
	mVerticalSpeed = 0.0f;

	mSprintTime = 3.0f;
	mSprinting = false;
	mSprintRecharge = false;

	AddCollider(new Square::BoxCollider(Square::Vector2(mSprite->ScaledDimensions().x, mSprite->ScaledDimensions().y)));

	mCollisionLayer = CollisionLayers::Friendly;
	mId = Square::PhysicsManager::Instance().RegisterEntity(this, mCollisionLayer);
}

Player::~Player()
{
	delete mSprite;
}

void Player::HandleMovement()
{
	mHorizontalSpeed = 0.0f;
	mVerticalSpeed = 0.0f;

	mSprinting = false;

	if (mInput.KeyDown(SDL_SCANCODE_A)) //Move Left
	{
		mHorizontalSpeed = -mSpeed * mTimer.DeltaTime();
	}
	else if (mInput.KeyDown(SDL_SCANCODE_D)) //Move Right
	{
		mHorizontalSpeed = mSpeed * mTimer.DeltaTime();
	}

	if (mInput.KeyDown(SDL_SCANCODE_W)) //Move Up
	{
		mVerticalSpeed = mSpeed * mTimer.DeltaTime();
	}
	else if (mInput.KeyDown(SDL_SCANCODE_S)) //Move Down
	{
		mVerticalSpeed = -mSpeed * mTimer.DeltaTime();
	}

	if (mInput.KeyDown(SDL_SCANCODE_LSHIFT)) //Sprint
	{
		if (mSprintTime > 0.0f && !mSprintRecharge)
			mSprinting = true;
	}

	if (mSprinting)
	{
		mHorizontalSpeed *= 1.66f;
		mVerticalSpeed *= 1.66f;

		mSprintTime -= mTimer.DeltaTime();

		if (mSprintTime <= 0.0f)
			mSprintRecharge = true;
	}
	else if (mSprintTime < 3.0f)
		mSprintTime += mTimer.DeltaTime() * 0.5f;

	if (mSprintTime >= 3.0f)
	{
		mSprintTime = 3.0f;
		mSprintRecharge = false;
	}
}

void Player::HandleCollision()
{
	Translate(Square::VEC2_RIGHT * mHorizontalSpeed);
	if (HasLeftWall() || HasRightWall())
	{
		Pos(PrevPos());
		do
		{
			Translate(Square::VEC2_RIGHT * Square::sgn(mHorizontalSpeed));
			mHorizontalSpeed -= Square::sgn(mHorizontalSpeed);
		} while (!HasLeftWall() && !HasRightWall());
		Pos(PrevPos());

		mHorizontalSpeed = 0.0f;
	}

	Translate(Square::VEC2_UP * mVerticalSpeed);
	if (HasGround() || HasCeiling())
	{
		Pos(PrevPos());
		do
		{
			Translate(Square::VEC2_UP * Square::sgn(mVerticalSpeed));
			mHorizontalSpeed -= Square::sgn(mVerticalSpeed);
		} while (!HasGround() && !HasCeiling());
		Pos(PrevPos());

		mVerticalSpeed = 0.0f;
	}

	if (OutsideMap())
		; // Kill player?
}

void Player::Update()
{
	HandleMovement();
	HandleCollision();

	Square::Graphics::Instance().Camera(Pos());
}

void Player::Render()
{
	mSprite->Render();

	Square::PhysEntity::Render();
}
