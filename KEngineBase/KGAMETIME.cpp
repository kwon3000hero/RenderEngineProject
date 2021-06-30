#include "KGAMETIME.h"
#include <assert.h>

KGAMETIME KGAMETIME::MainTime;

void KGAMETIME::MainTimeUpdate()
{
	MainTime.DEnd();
}

void KGAMETIME::Reset()
{
	MainTime.Start();
}

float KGAMETIME::DeltaTime(float speed)
{
	return MainTime.FTime * speed;
}

LARGE_INTEGER KGAMETIME::Frequency = LARGE_INTEGER();
KGAMETIME::Starter KGAMETIME::TimeStarter = KGAMETIME::Starter();

KGAMETIME::Starter::Starter()
{
	KGAMETIME::FrequencyInit();
}

KGAMETIME::Starter::~Starter()
{
}

void KGAMETIME::FrequencyInit()
{
	QueryPerformanceFrequency(&Frequency);
}

KGAMETIME::KGAMETIME() : PrevCounter(), NextCounter()
{
	Start();
}

KGAMETIME::~KGAMETIME()
{

}

void KGAMETIME::Start()
{
	QueryPerformanceCounter(&PrevCounter);
	QueryPerformanceCounter(&NextCounter);
}
float KGAMETIME::FEnd() {
	return (float)DEnd();
}
double KGAMETIME::DEnd()
{
	QueryPerformanceCounter(&NextCounter);

	if (0 == Frequency.QuadPart)
	{
		assert(false);
		return 0.0f;
	}

	DTime = ((double)NextCounter.QuadPart - (double)PrevCounter.QuadPart) / (double)Frequency.QuadPart;
	FTime = (float)DTime;

	PrevCounter = NextCounter;
	return DTime;
}

void KGAMETIME::Wait(float _WaitTime)
{
	float Time = _WaitTime;

	while (0.0f < Time)
	{
		Start();
		Time -= FEnd();
	}

	int a = 0;
}