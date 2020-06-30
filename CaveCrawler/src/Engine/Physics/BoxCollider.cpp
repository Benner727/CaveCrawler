#include "BoxCollider.h"

namespace Square {

	BoxCollider::BoxCollider(Vector2 size, float rotation)
		: Collider(ColliderType::Box)
	{
		AddVert(0, Vector2(-0.5f * size.x, -0.5f * size.y));
		AddVert(1, Vector2(0.5f * size.x, -0.5f * size.y));
		AddVert(2, Vector2(-0.5f * size.x, 0.5f * size.y));
		AddVert(3, Vector2(0.5f * size.x, 0.5f * size.y));

		if (DEBUG_COLLIDERS)
		{
			SetDebugTexture(new Sprite("BoxCollider.png"));
			mDebugTexture->Scale(size / 100.0f);
		}
	}

	BoxCollider::~BoxCollider()
	{
		for (int i = 0; i < MAX_VERTS; i++)
			delete mVerts[i];
	}

	Vector2 BoxCollider::GetFurthestPoint()
	{
		Vector2 localPos = Pos(GameObject::local);

		int furthestIndex = 0;
		float dist = (localPos + mVerts[0]->Pos(GameObject::local)).MagnitudeSqr();
		float otherDist = 0.0f;

		for (int i = 1; i < MAX_VERTS; i++)
		{
			otherDist = (localPos + mVerts[i]->Pos(GameObject::local)).MagnitudeSqr();

			if (otherDist > dist)
			{
				furthestIndex = i;
				dist = otherDist;
			}
		}

		return (localPos + mVerts[furthestIndex]->Pos(GameObject::local));
	}

	Vector2 BoxCollider::GetVertexPos(int index) const
	{
		return mVerts[index]->Pos();
	}

	void BoxCollider::AddVert(int index, Vector2 pos)
	{
		mVerts[index] = new GameObject(pos);
		mVerts[index]->Parent(this);
	}

}