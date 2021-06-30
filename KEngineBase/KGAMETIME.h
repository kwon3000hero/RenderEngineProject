#pragma once
#include <Windows.h>

class KGAMETIME
{
private:
	static KGAMETIME MainTime;

public:
	static void Reset();
	static void MainTimeUpdate();
	static float DeltaTime(float speed = 1.0f);

private:
	class Starter
	{
	public:
		Starter();
		~Starter();
	};
	static Starter TimeStarter;

public:
	static LARGE_INTEGER Frequency;
	static void FrequencyInit();

private:
	LARGE_INTEGER PrevCounter;
	LARGE_INTEGER NextCounter;
	float FTime;
	double DTime;

public:
	float LastFTime() {
		return FTime;
	}

	double LastDTime() {
		return DTime;
	}

	void Wait(float _WaitTime);

	void Start();
	float FEnd();
	double DEnd();

public:
	KGAMETIME();
	~KGAMETIME();
};

