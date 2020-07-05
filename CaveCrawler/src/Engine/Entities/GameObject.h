#pragma once

#include "Engine/Utilities/MathHelper.h"

namespace Square {

	class GameObject
	{
	public:
		enum SPACE { local = 0, world = 1 };

	private:
		Vector2 mPrevPos;
		Vector2 mPos;
		float mRotation;
		Vector2 mScale;

		bool mActive;

		GameObject* mParent;

	public:
		GameObject(Vector2 pos = VEC2_ZERO);
		
		virtual ~GameObject() = default;

		Vector2 PrevPos();
		void Pos(Vector2 pos);
		Vector2 Pos(SPACE space = world);

		void Rotation(float rotation);
		float Rotation(SPACE space = world);

		void Scale(Vector2 scale);
		Vector2 Scale(SPACE space = world);

		void Active(bool active);
		bool Active() const;

		void Parent(GameObject* parent);
		GameObject* Parent();

		void Translate(Vector2 vec, SPACE space = world);
		void Rotate(float amount);

		inline virtual Square::Vector2 Size() const { return Square::VEC2_ZERO; }

		virtual void Update();
		virtual void Render(bool ignoreCamera = false);

		GameObject& operator=(GameObject other);
		GameObject(const GameObject& other);
	};

}