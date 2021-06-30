#pragma once
#include <KGAMEMATH.h>

struct KVertex2D
{
	KVector Pos;
	KVector Uv;
	KVector Color;
};

struct KVertex3D
{
	KVector Pos;
	KVector Color;
	KVector Normal;
};

struct KTerrainVertex
{
	KVector Pos;
	KVector Uv;
	KVector Color;
	KVector Normal;
	KVector BiNormal;
	KVector Tangent;
};

struct KFBX3DVertex
{
	KVector Pos;
	KVector Uv;
	KVector Color;
	KVector Normal;
	KVector BiNormal;
	KVector Tangent;
	KVector Weight; // 가중치
	int Index[4]; // 인덱스

	KFBX3DVertex() : Color(KVector::WHITE), Index{ 0, }
	{
	}
};

enum class MatrixDDataType
{
	MDT_SCALE,
	MDT_ROT,
	MDT_ROTX,
	MDT_ROTY,
	MDT_ROTZ,
	MDT_POS,
	MDT_OFFSET,
	MDT_FINAL,
	MDT_REVOLUTION,
	MDT_PARENT,
	MDT_VIEW,
	MDT_PROJ,
	MDT_WV,
	MDT_WVP,
	MDT_MAX
};

struct TransformMatrix
{
	union
	{
		struct
		{
			KMatrix SCALE;
			KMatrix ROT;
			KMatrix ROTX;
			KMatrix ROTY;
			KMatrix ROTZ;
			KMatrix POS;
			KMatrix Offset;
			KMatrix Final;
			KMatrix REVOLUTION;
			KMatrix Parent;
			KMatrix VIEW;
			KMatrix PROJ;
			KMatrix WV;
			KMatrix WVP;
		};
		KMatrix ArrMat[13];
	};


public:
	void Init()
	{
		int size = static_cast<int>(MatrixDDataType::MDT_MAX);
		for (size_t i = 0; i < size; i++)
		{
			ArrMat[i].Identity();
		}
	}

	void CalculateFinalMatrix()
	{
		Offset = SCALE * ROT * POS * REVOLUTION;
		Final = Offset * Parent;
	}

	void CalculateTransformMatrix()
	{
		WV = Final * VIEW;
		WVP = WV * PROJ;
	}

public:
	TransformMatrix()
	{
	}

	TransformMatrix(const TransformMatrix& _MAT)
	{
		memcpy_s(this, sizeof(TransformMatrix), &_MAT, sizeof(TransformMatrix));
	}
};