#pragma once
#include <random>

class KGameRandom
{
private:
	static std::random_device m_randomDevice;
	static std::mt19937_64 m_randSeed;

private:
	class Starter
	{
	public:
		Starter();
		~Starter();
	};
	static Starter RandomStarter;

public:
	static void InitLinear();
	static short RangeLinear(short _Max);
	static short RangeLinearToOne(short _Max);

	static int RangeMersenneTwistInt(int _Max);

	static int RangeMersenneTwistInt(int _Min, int _Max);

	static float RangeMersenneTwistfloat(float _Min, float _Max);
};

