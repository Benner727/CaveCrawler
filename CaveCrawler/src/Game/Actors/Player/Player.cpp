#include "Player.h"

Player::Player(std::shared_ptr<Map> map)
	: Actor(map),
	mInput(Square::InputHandler::Instance()), mTimer(Square::Timer::Instance())
{
	mSprite = new Square::Sprite("HumanSprite.png");
	mSprite->Parent(this);

	mWeaponSlot = 0;

	mSpeed = 75.0f;
	mHorizontalSpeed = 0.0f;
	mVerticalSpeed = 0.0f;

	mSprintTime = 3.0f;
	mSprinting = false;
	mSprintRecharge = false;

	mFullHealth = 100;
	mHealth = mFullHealth;

	mHealTimer = 1.0f;

	mImmunityTimer = 0.0f;

	AddCollider(new Square::BoxCollider(Square::Vector2(mSprite->ScaledDimensions().x, mSprite->ScaledDimensions().y)));

	mCollisionLayer = CollisionLayers::Friendly;
	mId = Square::PhysicsManager::Instance().RegisterEntity(this, mCollisionLayer);
}

Player::~Player()
{
	delete mSprite;
}

void Player::Hit(PhysEntity* other)
{
	if (other == nullptr)
		return;

	if (std::find(mHitBy.begin(), mHitBy.end(), other->Id()) != mHitBy.end())
		return;

	mHitBy.push_back(other->Id());

	mSprite->Flash();

	// Apply damage
	mHealth = (mHealth - other->Damage());

	if (mHealth < 0) mHealth = 0;
}

void Player::GiveAmmo(int id)
{
	if (id == -1)
		mWeapon[mWeaponSlot]->GiveAmmo();
	else
	{
		for (auto& weapon : mWeapon)
		{
			if (weapon->Index() == id)
			{
				weapon->GiveAmmo();
				break;
			}
		}
	}
}

void Player::FillAmmo()
{
	for (auto* weapon : mWeapon)
		weapon->GiveAmmo();
}

bool Player::HasGun(int id) const
{
	for (auto& weapon : mWeapon)
	{
		if (weapon->Index() == id)
			return true;
	}

	return false;
}

void Player::GiveWeapon(Weapon* weapon)
{
	if (!HasGun(weapon->Index()))
	{
		if (mWeapon.size() < 3)
		{
			mWeapon.push_back(weapon);
			mWeapon.back()->Pos(Pos());
			mWeapon.back()->Parent(this);
			mWeapon[mWeaponSlot]->CancelReload();
			mWeaponSlot = mWeapon.size() - 1;
		}
		else
		{
			delete mWeapon[mWeaponSlot];
			mWeapon[mWeaponSlot] = weapon;
			mWeapon[mWeaponSlot]->Pos(Pos());
			mWeapon[mWeaponSlot]->Parent(this);
		}
	}
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

void Player::HandleGun()
{
	if (mWeapon.size() < 1)
		return;

	int prevSlot = mWeaponSlot;

	if (mInput.KeyPressed(SDL_SCANCODE_Q))
		--mWeaponSlot;
	else if (mInput.KeyPressed(SDL_SCANCODE_E))
		++mWeaponSlot;

	if (prevSlot != mWeaponSlot) mWeapon[prevSlot]->CancelReload();

	if (mInput.KeyPressed(SDL_SCANCODE_1))
		mWeaponSlot = 0;
	else if (mInput.KeyPressed(SDL_SCANCODE_2) && mWeapon.size() > 1)
		mWeaponSlot = 1;
	else if (mInput.KeyPressed(SDL_SCANCODE_3) && mWeapon.size() > 2)
		mWeaponSlot = 2;

	if (mWeaponSlot < 0)
		mWeaponSlot = mWeapon.size() - 1;
	else if (mWeaponSlot > mWeapon.size() - 1)
		mWeaponSlot = 0;

	if (mInput.KeyDown(SDL_SCANCODE_R)) //Reload
		mWeapon[mWeaponSlot]->Reload();

	Square::Vector2 mousePos = mInput.MousePos() + Square::Vector2(Square::Graphics::Instance().Camera().x, Square::Graphics::Instance().Camera().y);
	Square::Vector2 direction = (mousePos - Pos(Square::GameObject::world)).Normalize();
	float angle = atan2(direction.y, direction.x) * RAD_TO_DEG;
	mWeapon[mWeaponSlot]->Aim(angle);

	if ((mInput.MouseButtonDown(Square::InputHandler::MOUSE_BUTTON::left) && mWeapon[mWeaponSlot]->Automatic())
		|| mInput.MouseButtonPressed(Square::InputHandler::MOUSE_BUTTON::left))
	{
		mWeapon[mWeaponSlot]->Fire();
	}

	mWeapon[mWeaponSlot]->Update();
}

void Player::Update()
{
	HandleMovement();
	HandleCollision();
	HandleGun();

	if (mHealTimer <= 0.0f)
	{
		mHealTimer = 1.0f;
	}
	else
		mHealTimer -= mTimer.DeltaTime();

	if (mImmunityTimer <= 0.0f)
	{
		mHitBy.clear();
		mImmunityTimer = 0.33f;
	}
	else if (mHitBy.size())
		mImmunityTimer -= mTimer.DeltaTime();

	Square::Graphics::Instance().Camera(Pos());
}

void Player::Render()
{
	mSprite->Render();

	if (mWeapon.size())
		mWeapon[mWeaponSlot]->Render();

	Square::PhysEntity::Render();
}
