#include "CircleCollider.h"

namespace Square {

	CircleCollider::CircleCollider(float radius, bool broadPhase)
		: Collider(ColliderType::Circle)
	{
		mRadius = radius;

		if (DEBUG_COLLIDERS)
		{
			if (broadPhase)
				SetDebugTexture(new Sprite("BroadPhaseCollider.png"));
			else
				SetDebugTexture(new Sprite("CircleCollider.png"));

			mDebugTexture->Scale(VEC2_ONE * (radius * 2.0f / 100.0f));
		}
	}

	CircleCollider::~CircleCollider()
	{
	}

	Vector2 CircleCollider::GetFurthestPoint()
	{
		return VEC2_RIGHT * (mRadius + Pos(GameObject::local).Magnitude());
	}

	float CircleCollider::GetRadius() const
	{
		return mRadius;
	}

}