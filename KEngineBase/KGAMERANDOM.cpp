#include "KGAMERANDOM.h"
#include <time.h>
#include <random>

KGameRandom::Starter KGameRandom::RandomStarter = KGameRandom::Starter();
std::random_device KGameRandom::m_randomDevice;
std::mt19937_64 KGameRandom::m_randSeed = std::mt19937_64(m_randomDevice());

KGameRandom::Starter::Starter()
{
	KGameRandom::InitLinear();
}
KGameRandom::Starter::~Starter()
{
}

void KGameRandom::InitLinear()
{
	srand((unsigned int)time(nullptr));
}

short KGameRandom::RangeLinear(short _Max)
{
	return (short)(rand() % (_Max));
}

short KGameRandom::RangeLinearToOne(short _Max)
{
	return (short)(rand() % (_Max + 1));
}

int KGameRandom::RangeMersenneTwistInt(int _Max)
{
	return RangeMersenneTwistInt(0, _Max);
}

int KGameRandom::RangeMersenneTwistInt(int _Min, int _Max)
{
	std::uniform_int_distribution<int> Range = std::uniform_int_distribution<int>(_Min, _Max);
	return Range(m_randSeed);
}

float KGameRandom::RangeMersenneTwistfloat(float _Min, float _Max)
{
	std::uniform_real_distribution<float> Range = std::uniform_real_distribution<float>(_Min, _Max);
	return Range(m_randSeed);
}
