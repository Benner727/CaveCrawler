#pragma once

#include "Engine/Physics/PhysEntity.h"

#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <algorithm>

namespace Square {

	class PhysicsManager
	{
	private:
		std::vector<PhysEntity*> mCollisionLayers[static_cast<unsigned int>(PhysEntity::CollisionLayers::MaxLayers)];
		std::bitset<static_cast<unsigned int>(PhysEntity::CollisionLayers::MaxLayers)> mLayerMasks[static_cast<unsigned int>(PhysEntity::CollisionLayers::MaxLayers)];

		unsigned long mLastId;

		PhysicsManager();
		~PhysicsManager();

	public:
		static PhysicsManager& Instance();

		void SetLayerCollisionMask(PhysEntity::CollisionLayers layer, PhysEntity::CollisionFlags flags);

		unsigned long RegisterEntity(PhysEntity* entity, PhysEntity::CollisionLayers layer);
		void UnregisterEntity(unsigned long id);

		bool CheckCollision(PhysEntity* entity);

		void Update();
	};

}