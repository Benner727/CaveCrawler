#pragma once

#include "Game/Map/Map.h"

#include <cmath>

class FOV
{
private:
	Map* mMap;
	std::vector<bool> mPoints;

	void ResetFOV();
	void SetVisible(int x, int y);
	void CastLight(int x, int y, int radius, int row,
		float start_slope, float end_slope, int xx, int xy, int yx, int yy);

public:
	FOV(Map* map);
	~FOV() = default;

	void CalculateFOV(int x, int y, int radius);
	bool InFOV(int x, int y) const;
};