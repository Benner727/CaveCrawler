#pragma once

#include "Actors/Player/Player.h"

class GameLayer : public Square::Layer
{
private:
	std::shared_ptr<Map> mMap;
	std::shared_ptr<Player> mPlayer;

public:
	GameLayer();
	virtual ~GameLayer();

	virtual void OnUpdate() override;
	virtual void OnRender() override;
};