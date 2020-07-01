#include "GameLayer.h"

GameLayer::GameLayer()
{
	mMap = std::make_shared<Map>(48 * 2, 32 * 2);
	mPlayer = std::make_shared<Player>(mMap);

	mPlayer->Pos(mMap->Entry());
}

GameLayer::~GameLayer()
{
	
}

void GameLayer::OnUpdate()
{
	mMap->Update();
	mPlayer->Update();
}

void GameLayer::OnRender()
{
	mMap->Render();
	mPlayer->Render();
}