#include "GameLayer.h"

GameLayer::GameLayer()
{
	mMap = new Map(48 * 2, 32 * 2);
}

GameLayer::~GameLayer()
{
	delete mMap;
}

void GameLayer::OnUpdate()
{
	mMap->Update();
}

void GameLayer::OnRender()
{
	mMap->Render();
}