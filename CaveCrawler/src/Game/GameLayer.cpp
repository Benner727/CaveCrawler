#include "GameLayer.h"

#include "Game/Weapons/Gun.h"

GameLayer::GameLayer()
{
	mMap = std::make_shared<Map>(48 * 2, 32 * 2);
	mPlayer = std::make_shared<Player>(mMap);

	mPlayer->Pos(mMap->Entry());
	mMap->SetFocus(mPlayer.get());

	Gun* gun = new Gun(mMap);
	mPlayer->GiveWeapon(gun);

	mPathFinder = std::make_shared<PathFinder>(mMap, mPlayer);

	mEnemies.push_back(new Drone(mMap, mPathFinder, mPlayer.get()));
	mEnemies.back()->Pos(mMap->Exit());
}

GameLayer::~GameLayer()
{
	
}

void GameLayer::OnUpdate()
{
	mMap->Update();
	mPlayer->Update();

	ProjectileManager::Instance().Update();

	for (auto& enemy : mEnemies)
		enemy->Update();

	mPathFinder->Update();
}

void GameLayer::OnRender()
{
	mMap->Render();

	mPathFinder->Render();

	mPlayer->Render();

	ProjectileManager::Instance().Render();

	for (const auto& enemy : mEnemies)
		enemy->Render();
}