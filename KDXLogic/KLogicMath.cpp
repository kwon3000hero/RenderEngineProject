#include "KLogicMath.h"
#include <KGameInput.h>

const int KLogicMath::ArrDirIndex[8] =
{
	6,
	5,
	4,
	3,
	2,
	1,
	0,
	7,
};

int KLogicMath::DirAniIndex(KVector _Pos)
{
	if (KVector::ZERO == _Pos)
	{
		return 0;
	}

	float Test = _Pos.AngleDegZ();
	Test += 22.5f;

	if (360 <= Test)
	{
		Test -= 360.0f;
	}



	float Cut = 360.0f / 8.0f;
	return KLogicMath::ArrDirIndex[(int)(Test / Cut)];
}

__int64 KLogicMath::IndexToKey(int _X, int _Y)
{
	__int64 Result;

	int* Ptr = (int*)&Result;

	Ptr[0] = _X;
	Ptr[1] = _Y;

	return Result;
}