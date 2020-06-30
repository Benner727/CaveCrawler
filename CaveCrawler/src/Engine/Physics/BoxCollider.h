#pragma once

#include "Engine/Physics/Collider.h"

#include <vector>

namespace Square {

	class BoxCollider : public Collider
	{
	private:
		static const int MAX_VERTS = 4;
		GameObject* mVerts[MAX_VERTS];

		void AddVert(int index, Vector2 pos);

	public:
		BoxCollider(Vector2 size, float rotation = 0.0f);
		~BoxCollider();

		Vector2 GetFurthestPoint() override;

		Vector2 GetVertexPos(int index) const;
	};

}