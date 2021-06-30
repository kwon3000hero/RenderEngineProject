#pragma once
#include "KGameComponentManager.h"
#include <KGAMEMATH.h>

union MapKey
{
	struct
	{
		int X;
		int Z;
	};
	__int64 Key;
};

class KHeight3DTile
{
public:
	KVector TriangleVertex[4];
};

class KTexture;
class KMesh;
class KVertexBuffer;
class KIndexBuffer;
class KHeightTerrain : public KActorGameComponent
{
private:
	KVector m_MapData;

	KPTR<KVertexBuffer> m_VB;
	KPTR<KIndexBuffer> m_IB;
	KPTR<KMesh> m_MESH;
	KPTR<KTexture> m_HeightTexture;
	KPTR<KRenderPlayer> m_renderPlayer;

	std::map<__int64, KHeight3DTile> m_MapTile;

public:
	void CreateGround(int _x, int _z, const KGameString& _DifTexName, const KGameString& _heightMapName);

public:
	KVector& MapData()
	{
		return m_MapData;
	}

public:
	void Init() {}

	float MapY(KVector _pos);
	void AddFloor(const KGameString& _DifTexName, const KGameString& _SpTex);

};

