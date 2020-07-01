#pragma once

#include "Game/Projectiles/Projectile.h"

class Bullet : public Projectile
{
public:
	Bullet(std::shared_ptr<Map> map, Square::Vector2 pos, float rotation, bool shell = false);
	~Bullet();

	inline void Damage(int damage) { mDamage = damage; }
	inline void Penetrates(bool penetrates) { mPenetrates = penetrates; }
};