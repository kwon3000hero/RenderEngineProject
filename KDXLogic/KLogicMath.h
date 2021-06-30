#pragma once
#include <KGAMEMATH.h>

class KLogicMath
{
public:
	static const int ArrDirIndex[8];

public:
	static int DirAniIndex(KVector _Pos);

	static __int64 IndexToKey(int _X, int _Y);

};

