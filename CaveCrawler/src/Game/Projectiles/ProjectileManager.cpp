#include "ProjectileManager.h"

ProjectileManager& ProjectileManager::Instance()
{
	static ProjectileManager instance;
	return instance;
}

ProjectileManager::ProjectileManager()
{
}

ProjectileManager::~ProjectileManager()
{
	for (auto& projectile : mProjectiles)
		delete projectile;
}

void ProjectileManager::AddProjectile(Projectile* projectile)
{
	mProjectiles.push_back(projectile);
}

void ProjectileManager::Update()
{
	for (std::vector<Projectile*>::iterator it = mProjectiles.begin(); it != mProjectiles.end();)
	{
		if ((*it)->CleanUp())
		{
			delete* it;
			it = mProjectiles.erase(it);
		}
		else
		{
			(*it)->Update();
			++it;
		}
	}
}

void ProjectileManager::Render()
{
	for (const auto& projectile : mProjectiles)
		projectile->Render();
}