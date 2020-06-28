#include "GameLayer.h"

GameLayer::GameLayer()
{
	mSprite = new Square::Sprite("Player.png");
	mSprite->Pos(Square::Vector2(Square::Graphics::SCREEN_WIDTH * 0.5f, Square::Graphics::SCREEN_HEIGHT * 0.5f));
}

GameLayer::~GameLayer()
{
	delete mSprite;
}

void GameLayer::OnUpdate()
{
	mSprite->Update();
}

void GameLayer::OnRender()
{
	mSprite->Render();
}