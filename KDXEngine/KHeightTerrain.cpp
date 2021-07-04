#include "KHeightTerrain.h"
#include "KRender.h"
#include "KVertexBuffer.h"
#include "KIndexBuffer.h"
#include "KGameDebug3D.h"

void KHeightTerrain::CreateGround(int _x, int _z, const KGameString& _DifTexName, const KGameString& _heightMapName)
{
	m_HeightTexture = KTexture::Find(_heightMapName);

	m_MapData.x = (float)_x;
	m_MapData.z = (float)_z;

	float InterX = 1.0f / _x;
	float InterZ = 1.0f / _z;

	float heightMapX = m_HeightTexture->Size().x;
	float heightMapZ = m_HeightTexture->Size().y;

	int maxZ = _z + 1;
	int maxX = _x + 1;

	std::vector<KTerrainVertex> m_VTX;
	m_VTX.resize(maxX * maxZ);

	for (int z = 0; z < maxZ; z++)
	{
		for (int x = 0; x < maxX; x++)
		{
			m_VTX[z * maxZ + x].Pos = KVector(x * InterX - 0.5f, 0.0f, z * -InterZ + 0.5f, 1.0f);

			KVector PixelPos = m_VTX[z * maxZ + x].Pos;
			PixelPos.x += +0.5F;
			PixelPos.z += -0.5F;

			PixelPos.x *= heightMapX;
			PixelPos.z *= -heightMapZ;

			KVector Pixel = m_HeightTexture->Pixel((int)PixelPos.x, (int)PixelPos.z);
			m_VTX[z * maxZ + x].Pos.y = Pixel.x * 0.5f;
			m_VTX[z * maxZ + x].Color = KVector::WHITE;
			m_VTX[z * maxZ + x].Uv = KVector((float)x, (float)z);
			m_VTX[z * maxZ + x].Normal = KVector(0.0f, 1.0f, 0.0f);
			m_VTX[z * maxZ + x].Tangent = KVector(0.0f, 0.0f, 1.0f);
			m_VTX[z * maxZ + x].BiNormal = KVector(1.0f, 0.0f, 0.0f);
		}
	}

	m_VB = make_KPTR<KVertexBuffer>();
	m_IB = make_KPTR<KIndexBuffer>();

	std::vector<UINT> m_Index;

	for (int z = 0; z < _z; z++)
	{
		for (int x = 0; x < _x; x++)
		{
			int Pivot = z * maxZ + x;

			float mapX = m_VTX[Pivot].Pos.x >= 0.0f ? m_VTX[Pivot].Pos.x + 1.0f / m_MapData.x * 0.1f : m_VTX[Pivot].Pos.x - 1.0f / m_MapData.x * 0.1f;
			float mapZ = m_VTX[Pivot].Pos.z >= 0.0f ? m_VTX[Pivot].Pos.z + 1.0f / m_MapData.z * 0.1f : m_VTX[Pivot].Pos.z - 1.0f / m_MapData.z * 0.1f;

			mapX *= m_MapData.x;
			mapZ *= m_MapData.z;

			MapKey calculatedKey;
			calculatedKey.X = static_cast<int>(mapX);
			calculatedKey.Z = static_cast<int>(mapZ);

			m_MapTile[calculatedKey.Key].TriangleVertex[0] = m_VTX[Pivot].Pos;
			m_MapTile[calculatedKey.Key].TriangleVertex[1] = m_VTX[Pivot + 1].Pos;
			m_MapTile[calculatedKey.Key].TriangleVertex[2] = m_VTX[Pivot + (maxX)+1].Pos;
			m_MapTile[calculatedKey.Key].TriangleVertex[3] = m_VTX[Pivot + (maxX)].Pos;


			m_Index.push_back(Pivot);
			m_Index.push_back(Pivot + 1);
			m_Index.push_back(Pivot + maxX + 1);

			m_Index.push_back(Pivot);
			m_Index.push_back(Pivot + maxX + 1);
			m_Index.push_back(Pivot + maxX);
		}
	}

	for (int z = 0; z < maxZ; z++)
	{
		for (int x = 0; x < maxX; x++)
		{
			m_VTX[z * maxZ + x].Pos.y = 0.0f;
		}
	}

	m_VB->Create(m_VTX.size(), sizeof(KTerrainVertex), (void*)&m_VTX[0], D3D11_USAGE_DYNAMIC);
	m_IB->Create(m_Index.size(), sizeof(UINT), (void*)&m_Index[0], D3D11_USAGE_DYNAMIC);

	m_MESH = make_KPTR<KMesh>();

	m_MESH->AddVertexBuffer(m_VB);
	m_MESH->AddIndexBuffer(m_IB);

	KPTR<KRenderManager> PTR = Actor()->CreateComponent<KRenderManager>();

	m_renderPlayer = PTR->CreateRenderPlayer(m_MESH, L"HMAPDEFFERD");
	m_renderPlayer->SetTexture(L"DifTex0", _DifTexName);
	m_renderPlayer->SetTexture(L"HeightTex", _heightMapName);
	m_renderPlayer->SetSampler(L"Smp", L"LWSMP");
	m_renderPlayer->SetConstantBuffer(L"MAPDATA", &m_MapData, ConstantBufferMode::Link);

	m_renderPlayer->ShadowOn();

}

float KHeightTerrain::MapY(KVector _pos)
{
	float X = (_pos.x * m_MapData.x / Actor()->Transform()->WorldScale().x);
	float Z = (_pos.z * m_MapData.z / Actor()->Transform()->WorldScale().z);

	int MapX = (int)X;
	int MapZ = (int)Z;

	_pos.y = 0.0f;

	if (0 > _pos.x)
	{
		MapX += -1;
	}

	if (0 < _pos.z)
	{
		MapZ += 1;
	}

#ifdef _DEBUG
	KGameDebug3D::DebugText(KVector(0, 700), 20.0f, KVector::WHITE, L"%d %d", MapX, MapZ);
#endif

	MapKey CalKey;
	CalKey.X = (int)MapX;
	CalKey.Z = (int)MapZ;

	if (m_MapTile.end() == m_MapTile.find(CalKey.Key))
	{
		AssertMsg(L"맵 바깥 범위입니다.");
	}

	KVector temp = m_MapTile[CalKey.Key].TriangleVertex[0];
	// 위
	KVector UpTri0 = m_MapTile[CalKey.Key].TriangleVertex[0] * Actor()->Transform()->WorldScale();
	KVector UpTri1 = m_MapTile[CalKey.Key].TriangleVertex[1] * Actor()->Transform()->WorldScale();
	KVector UpTri2 = m_MapTile[CalKey.Key].TriangleVertex[2] * Actor()->Transform()->WorldScale();

	// 아래
	KVector DownTri0 = m_MapTile[CalKey.Key].TriangleVertex[0] * Actor()->Transform()->WorldScale();
	KVector DownTri1 = m_MapTile[CalKey.Key].TriangleVertex[2] * Actor()->Transform()->WorldScale();
	KVector DownTri2 = m_MapTile[CalKey.Key].TriangleVertex[3] * Actor()->Transform()->WorldScale();

	KVector pos = _pos;

	float checkDown = -1.0f;
	DirectX::TriangleTests::Intersects(_pos, KVector::UP, UpTri0, UpTri1, UpTri2, checkDown);

	float checkUp = -1.0f;
	DirectX::TriangleTests::Intersects(_pos, KVector::UP, DownTri0, DownTri1, DownTri2, checkUp);

	if (0 != checkDown)
	{
		return checkDown;
	}

	if (0 != checkUp)
	{
		return checkUp;
	}

	return 0.0f;
}


void KHeightTerrain::AddFloor(const KGameString& _DifTexName, const KGameString& _SpTex)
{
	int Count = (int)m_MapData.w;

	KGameString Dif = KGameString(L"DifTex") + ++Count;
	KGameString Sp = KGameString(L"DifTexSp") + Count;


	m_renderPlayer->SetTexture(Dif, _DifTexName);
	m_renderPlayer->SetTexture(Sp, _SpTex);
	m_MapData.w = (float)Count;
}
