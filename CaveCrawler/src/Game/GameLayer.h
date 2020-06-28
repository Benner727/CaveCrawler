#pragma once

#include "Engine/Square.h"

class GameLayer : public Square::Layer
{
private:
	Square::Sprite* mSprite;

public:
	GameLayer();
	virtual ~GameLayer();

	virtual void OnUpdate() override;
	virtual void OnRender() override;
};