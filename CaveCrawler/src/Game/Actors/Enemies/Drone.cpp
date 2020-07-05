#include "Drone.h"

Drone::Drone(std::shared_ptr<Map> map, std::shared_ptr<PathFinder> pathFinder, Square::GameObject* target)
	: Enemy(map, pathFinder, target)
{
	mSprite = new Square::Sprite("DroneSprite.png");
	mSprite->Parent(this);

	mSpeed = 50.0f;

	mHealth = 100;
	mDamage = 12;

	AddCollider(new Square::BoxCollider(Square::Vector2(mSprite->ScaledDimensions().x, mSprite->ScaledDimensions().y)));

	mCollisionLayer = CollisionLayers::Hostile;
	mId = Square::PhysicsManager::Instance().RegisterEntity(this, mCollisionLayer);
}
