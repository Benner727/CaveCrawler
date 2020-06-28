#include "GameObject.h"

namespace Square {

	GameObject::GameObject(Vector2 pos)
		: mPos(pos)
	{
		mPrevPos = VEC2_ZERO;

		mRotation = 0.0f;

		mActive = true;

		mParent = nullptr;

		mScale = VEC2_ONE;
	}

	Vector2 GameObject::PrevPos()
	{
		return mPrevPos;
	}

	void GameObject::Pos(Vector2 pos)
	{
		mPrevPos = mPos;
		mPos = pos;
	}

	Vector2 GameObject::Pos(SPACE space)
	{
		if (space == local || mParent == nullptr)
			return mPos;

		GameObject* parent = mParent;
		Vector2 finalPos = mPos, parentScale = VEC2_ZERO;

		do
		{
			parentScale = mParent->Scale(local);
			finalPos = RotateVector(Vector2(finalPos.x * parentScale.x, finalPos.y * parentScale.y), parent->Rotation(local));
			finalPos += parent->Pos(local);

			parent = parent->Parent();

		} while (parent);

		return finalPos;
	}

	void GameObject::Rotation(float r)
	{
		mRotation = r;

		if (mRotation > 360.0f)
			mRotation -= 360.0f;

		if (mRotation < 0.0f)
			mRotation += 360.0f;
	}

	float GameObject::Rotation(SPACE space)
	{
		if (space == local || mParent == nullptr)
			return mRotation;

		return mParent->Rotation(world) + mRotation;
	}

	void GameObject::Scale(Vector2 scale)
	{
		mScale = scale;
	}

	Vector2 GameObject::Scale(SPACE space)
	{
		if (space == local || mParent == nullptr)
			return mScale;

		Vector2 scale = mParent->Scale(world);
		scale.x *= mScale.x;
		scale.y *= mScale.y;

		return scale;
	}

	void GameObject::Active(bool active)
	{
		mActive = active;
	}

	bool GameObject::Active() const
	{
		return mActive;
	}

	void GameObject::Parent(GameObject* parent)
	{
		if (parent == nullptr)
		{
			mPos = Pos(world);
			mRotation = Rotation(world);
			mScale = Scale(world);
		}
		else
		{
			if (mParent != nullptr)
				Parent(nullptr);

			Vector2 parentScale = parent->Scale(world);;

			mPos = RotateVector(Pos(world) - parent->Pos(world), -parent->Rotation(world));
			mPos.x /= parentScale.x;
			mPos.y /= parentScale.y;

			mRotation -= parent->Rotation(world);

			mScale = Vector2(mScale.x / parentScale.x, mScale.y / parentScale.y);
		}

		mParent = parent;
	}

	GameObject* GameObject::Parent()
	{
		return mParent;
	}

	void GameObject::Translate(Vector2 vec, SPACE space)
	{
		mPrevPos = mPos;

		if (space == world)
			mPos += vec;
		else
			mPos += RotateVector(vec, Rotation());
	}

	void GameObject::Rotate(float amount)
	{
		mRotation += amount;

		if (mRotation > 360.0f)
			mRotation -= 360.0f;

		if (mRotation < 0.0f)
			mRotation += 360.0f;
	}

	void GameObject::Update()
	{

	}

	void GameObject::Render(bool ignoreCamera)
	{

	}

	GameObject& GameObject::operator=(GameObject other)
	{
		mPos = other.mPos;
		mPrevPos = other.mPrevPos;

		mRotation = other.mRotation;

		mActive = other.mActive;

		mParent = nullptr;

		mScale = other.mScale;

		return *this;
	}

	GameObject::GameObject(const GameObject& other)
	{
		mPos = other.mPos;
		mPrevPos = other.mPrevPos;

		mRotation = other.mRotation;

		mActive = other.mActive;

		mParent = nullptr;

		mScale = other.mScale;
	}

}