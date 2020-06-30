#pragma once

#include "Engine/Physics/Collider.h"

namespace Square {

	class CircleCollider : public Collider
	{
	private:
		float mRadius;

	public:
		CircleCollider(float radius, bool broadPhase = false);
		~CircleCollider();

		Vector2 GetFurthestPoint() override;

		float GetRadius() const;
	};

}