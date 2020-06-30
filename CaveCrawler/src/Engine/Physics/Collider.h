#pragma once

#include "Engine/Entities/Sprite.h"

namespace Square {

	class Collider : public GameObject
	{
	public:
		enum class ColliderType
		{
			Box,
			Circle
		};

	protected:
		ColliderType mType;

		static const bool DEBUG_COLLIDERS = false;
		Sprite* mDebugTexture;

		void SetDebugTexture(Sprite* texture);

	public:
		Collider(ColliderType type);
		virtual ~Collider();

		virtual Vector2 GetFurthestPoint() = 0;

		ColliderType GetType() const;

		virtual void Render();
	};

}