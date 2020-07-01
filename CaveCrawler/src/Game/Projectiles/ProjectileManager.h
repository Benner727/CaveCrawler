#pragma once

#include "Game/Projectiles/Projectile.h"

class ProjectileManager
{
private:
	ProjectileManager();
	~ProjectileManager();

	std::vector<Projectile*> mProjectiles;

public:
	static ProjectileManager& Instance();

	void AddProjectile(Projectile* projectile);

	void Update();
	void Render();
};