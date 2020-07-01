#pragma once

#include <random>

class Random
{
private:
	std::mt19937 mRandomEngine;
	std::uniform_int_distribution<std::mt19937::result_type> mDistribution;

	int mSeed;

	inline void Init(int seed)
	{
		mSeed = seed;
		mRandomEngine.seed(mSeed);
	}

public:
	Random(int seed = std::random_device()())
	{
		Init(seed);
	}

	inline void Seed(int seed) { Init(seed); }
	inline int Seed() const { return mSeed; }

	inline float Float()
	{
		return (float)mDistribution(mRandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

	inline int Int(int min, int max)
	{
		return (int)((float)(max - min + 1) * Float()) + min;
	}
};