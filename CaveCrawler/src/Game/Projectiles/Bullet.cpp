#include "Bullet.h"

Bullet::Bullet(std::shared_ptr<Map> map, Square::Vector2 pos, float rotation, bool shell)
	: Projectile(map, pos, rotation, shell)
{
	mSprite = new Square::AnimatedSprite("BulletSprite.png", 0, 0, 4, 4, 2, (rand() % 2) ? 0.01 : 0.05f, Square::AnimatedSprite::ANIM_DIR::horizontal);
	mSprite->WrapMode(Square::AnimatedSprite::WRAP_MODE::once);
	mSprite->Parent(this);

	mSpeed = 750.0f;
	mDamage = 25;

	AddCollider(new Square::CircleCollider(mSprite->ScaledDimensions().x * 0.5f));

	Pos(pos);
	Rotation(rotation);

	mCollisionLayer = CollisionLayers::FriendlyProjectiles;
	mId = Square::PhysicsManager::Instance().RegisterEntity(this, mCollisionLayer);
}

Bullet::~Bullet()
{

}