#include "GameLayer.h"

GameLayer::GameLayer()
{
	mMap = std::make_shared<Map>(48 * 2, 32 * 2);
	mPlayer = std::make_shared<Player>(mMap);

	mPlayer->Pos(mMap->Entry());
	mMap->SetFocus(mPlayer.get());
}

GameLayer::~GameLayer()
{
	
}

void GameLayer::OnUpdate()
{
	mMap->Update();
	mPlayer->Update();

	ProjectileManager::Instance().Update();
}

void GameLayer::OnRender()
{
	mMap->Render();
	mPlayer->Render();

	ProjectileManager::Instance().Render();
}