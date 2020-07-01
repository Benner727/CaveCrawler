#include "FOV.h"

static int multipliers[4][8] = {
	{1, 0, 0, -1, -1, 0, 0, 1},
	{0, 1, -1, 0, 0, -1, 1, 0},
	{0, 1, 1, 0, 0, -1, -1, 0},
	{1, 0, 0, 1, -1, 0, 0, -1}
};


FOV::FOV(Map* map)
	: mMap(map)
{
	void ResetFOV();
}

void FOV::ResetFOV()
{
	mPoints.clear();
	for (int i = 0; i < mMap->Width() * mMap->Height(); i++)
		mPoints.push_back(false);
}

void FOV::SetVisible(int x, int y)
{
	mPoints[x + y * mMap->Width()] = true;
}

void FOV::CastLight(int x, int y, int radius, int row,
	float start_slope, float end_slope, int xx, int xy, int yx, int yy)
{
	if (start_slope < end_slope)
	{
		return;
	}

	float next_start_slope = start_slope;

	for (int i = row; i <= radius; i++)
	{
		bool blocked = false;

		for (int dx = -i, dy = -i; dx <= 0; dx++)
		{
			float l_slope = (dx - 0.5) / (dy + 0.5);
			float r_slope = (dx + 0.5) / (dy - 0.5);
			if (start_slope < r_slope)
			{
				continue;
			}
			else if (end_slope > l_slope)
			{
				break;
			}

			int sax = dx * xx + dy * xy;
			int say = dx * yx + dy * yy;
			if ((sax < 0 && (int)std::abs(sax) > x) ||
				(say < 0 && (int)std::abs(say) > y))
			{
				continue;
			}

			int ax = x + sax;
			int ay = y + say;
			if (ax >= mMap->Width() || ay >= mMap->Height())
			{
				continue;
			}

			int radius2 = radius * radius;
			if ((int)(dx * dx + dy * dy) < radius2)
			{
				SetVisible(ax, ay);
			}

			if (blocked)
			{
				if (mMap->IsOpaque(ax, ay))
				{
					next_start_slope = r_slope;
					continue;
				}
				else
				{
					blocked = false;
					start_slope = next_start_slope;
				}
			}
			else if (mMap->IsOpaque(ax, ay))
			{
				blocked = true;
				next_start_slope = r_slope;
				CastLight(x, y, radius, i + 1, start_slope, l_slope, xx,
					xy, yx, yy);
			}
		}

		if (blocked)
		{
			break;
		}
	}
}

void FOV::CalculateFOV(int sourceX, int sourceY, int radius)
{
	ResetFOV();

	SetVisible(sourceX, sourceY);

	for (int i = 0; i < 8; i++)
	{
		CastLight(sourceX, sourceY, radius, 1, 1.0f, 0.0f,
			multipliers[0][i], multipliers[1][i], multipliers[2][i], multipliers[3][i]);
	}
}

bool FOV::InFOV(int x, int y) const
{
	return mPoints[x + y * mMap->Width()];
}
