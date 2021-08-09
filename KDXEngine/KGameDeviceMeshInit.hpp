#pragma once
#include "KGameDevice.h"
#include "KVertexHeader.h"
#include "KMesh.h"
#include "KVertexBuffer.h"
#include "KIndexBuffer.h"

template<typename T>
void KGameDevice<T>::InitDefaultMesh()
{
	{
		std::vector<KVertex2D> m_VTX;
		m_VTX.push_back({ { -0.5f, 0.5f, 0.0f, 1.0F }, { 0.0f, 0.0f, 0.0f, 1.0F }, KVector::WHITE });
		m_VTX.push_back({ { 0.5f, 0.5f, 0.0f, 1.0F },  { 1.0f, 0.0f, 0.0f, 1.0F },KVector::WHITE });
		m_VTX.push_back({ { 0.5f, -0.5f, 0.0f, 1.0F },  { 1.0f, 1.0f, 0.0f, 1.0F },KVector::WHITE });
		m_VTX.push_back({ { -0.5f, -0.5f, 0.0f, 1.0F }, { 0.0f, 1.0f, 0.0f, 1.0F },KVector::WHITE });
		KVertexBuffer::Create(L"RECT", m_VTX);

		std::vector<UINT> m_IDX = { 0, 1, 2, 0, 2, 3 };
		KIndexBuffer::Create(L"RECT", m_IDX);
		KMesh::Create(L"RECT");
	}

	{
		std::vector<UINT> m_IDX = { 0, 1, 2, 3, 0 };
		KIndexBuffer::Create(L"RECTDEBUG", m_IDX);
		KMesh::Create(L"RECTDEBUG", L"RECT", L"RECTDEBUG");
	}

	{
		std::vector<KVertex2D> m_VTX;
		m_VTX.push_back({ { -0.5f, 0.0f, 0.5f, 1.0F },	{ 0.0f, 0.0f, 0.0f, 1.0F }, KVector::WHITE });
		m_VTX.push_back({ { 0.5f, 0.0f, 0.5f, 1.0F },	{ 1.0f, 0.0f, 0.0f, 1.0F }, KVector::WHITE });
		m_VTX.push_back({ { 0.5f, 0.0f, -0.5f, 1.0F },	{ 1.0f, 1.0f, 0.0f, 1.0F }, KVector::WHITE });
		m_VTX.push_back({ { -0.5f, 0.0f, -0.5f, 1.0F }, { 0.0f, 1.0f, 0.0f, 1.0F }, KVector::WHITE });
		KVertexBuffer::Create(L"GRIDRECT", m_VTX);

		// 인덱스 버퍼 완료
		std::vector<UINT> m_IDX = { 0, 1, 2, 0, 2, 3 };
		KIndexBuffer::Create(L"GRIDRECT", m_IDX);
		KMesh::Create(L"GRIDRECT");
	}

	{
		std::vector<KVertex2D> m_VTX;
		m_VTX.push_back({ KVector{ 0.0f, 0.0f, 0.0f, 1.0F },	{ 0.0f, 0.0f, 0.0f, 1.0F }, KVector::WHITE });
		m_VTX.push_back({ KVector{ 1.0f, 0.0f, 0.0f, 1.0F },	{ 1.0f, 0.0f, 0.0f, 1.0F },KVector::WHITE });
		m_VTX.push_back({ KVector{ 1.0f, -1.0f, 0.0f, 1.0F },	{ 1.0f, 1.0f, 0.0f, 1.0F },KVector::WHITE });
		m_VTX.push_back({ KVector{ 0.0f, -1.0f, 0.0f, 1.0F }, { 0.0f, 1.0f, 0.0f, 1.0F },KVector::WHITE });
		KVertexBuffer::Create(L"DEBUGTEXRECT", m_VTX);

		// 인덱스 버퍼 완료
		std::vector<UINT> m_IDX = { 0, 1, 2, 0, 2, 3 };
		KIndexBuffer::Create(L"DEBUGTEXRECT", m_IDX);
		KMesh::Create(L"DEBUGTEXRECT");

	}



	{
		std::vector<KVertex2D> ArrVtx;
		ArrVtx.resize(24);

		{
			// 뒷면
			{
				ArrVtx[0] = { KVector(-0.5f, 0.5f, -0.5f, 1.0f)  ,{ 0.0f, 0.0f, 0.0f, 1.0F }, KVector::RED };
				ArrVtx[1] = { KVector(0.5f, 0.5f, -0.5f, 1.0f)   ,{ 1.0f, 0.0f, 0.0f, 1.0F }, KVector::RED };
				ArrVtx[2] = { KVector(0.5f, -0.5f, -0.5f, 1.0f)  ,{ 1.0f, 1.0f, 0.0f, 1.0F }, KVector::WHITE };
				ArrVtx[3] = { KVector(-0.5f, -0.5f, -0.5f, 1.0f) ,{ 0.0f, 1.0f, 0.0f, 1.0F }, KVector::WHITE };
			}
			// 앞면
			{
				ArrVtx[4] = { KVector(-0.5f, 0.5f, 0.5f, 1.0f)    ,{ 0.0f, 0.0f, 0.0f, 1.0F },KVector::RED };
				ArrVtx[5] = { KVector(0.5f, 0.5f, 0.5f, 1.0f)     ,{ 1.0f, 0.0f, 0.0f, 1.0F },KVector::RED };
				ArrVtx[6] = { KVector(0.5f, -0.5f, 0.5f, 1.0f)    ,{ 1.0f, 1.0f, 0.0f, 1.0F },KVector::GREEN };
				ArrVtx[7] = { KVector(-0.5f, -0.5f, 0.5f, 1.0f)   ,{ 0.0f, 1.0f, 0.0f, 1.0F },KVector::GREEN };
			}
			// 오른쪽
			{
				ArrVtx[8] = { KVector(0.5f, 0.5f, -0.5f, 1.0f)     ,{ 0.0f, 0.0f, 0.0f, 1.0F },KVector::WHITE };
				ArrVtx[9] = { KVector(0.5f, 0.5f, 0.5f, 1.0f)      ,{ 1.0f, 0.0f, 0.0f, 1.0F },KVector::WHITE };
				ArrVtx[10] = { KVector(0.5f, -0.5f, 0.5f, 1.0f)    ,{ 1.0f, 1.0f, 0.0f, 1.0F },KVector::WHITE };
				ArrVtx[11] = { KVector(0.5f, -0.5f, -0.5f, 1.0f)   ,{ 0.0f, 1.0f, 0.0f, 1.0F },KVector::WHITE };
			}
			// 왼쪽
			{
				ArrVtx[12] = { KVector(-0.5f, 0.5f, -0.5f, 1.0f)    ,{ 0.0f, 0.0f, 0.0f, 1.0F },KVector::WHITE };
				ArrVtx[13] = { KVector(-0.5f, 0.5f, 0.5f, 1.0f)     ,{ 1.0f, 0.0f, 0.0f, 1.0F },KVector::WHITE };
				ArrVtx[14] = { KVector(-0.5f, -0.5f, 0.5f, 1.0f)    ,{ 1.0f, 1.0f, 0.0f, 1.0F },KVector::WHITE };
				ArrVtx[15] = { KVector(-0.5f, -0.5f, -0.5f, 1.0f)   ,{ 0.0f, 1.0f, 0.0f, 1.0F },KVector::WHITE };

			}
			// 아랫면
			{
				ArrVtx[16] = { KVector(-0.5f, -0.5f, -0.5f, 1.0f)    ,{ 0.0f, 0.0f, 0.0f, 1.0F },KVector::GREEN };
				ArrVtx[17] = { KVector(0.5f, -0.5f, -0.5f, 1.0f)     ,{ 1.0f, 0.0f, 0.0f, 1.0F },KVector::GREEN };
				ArrVtx[18] = { KVector(0.5f, -0.5f, 0.5f, 1.0f)      ,{ 1.0f, 1.0f, 0.0f, 1.0F },KVector::GREEN };
				ArrVtx[19] = { KVector(-0.5f, -0.5f, 0.5f, 1.0f)     ,{ 0.0f, 1.0f, 0.0f, 1.0F },KVector::GREEN };
			}
			// 윗면
			{
				ArrVtx[20] = { KVector(-0.5f, 0.5f, -0.5f, 1.0f)    ,{ 0.0f, 0.0f, 0.0f, 1.0F },KVector::GREEN };
				ArrVtx[21] = { KVector(0.5f, 0.5f, -0.5f, 1.0f)     ,{ 1.0f, 0.0f, 0.0f, 1.0F },KVector::GREEN };
				ArrVtx[22] = { KVector(0.5f, 0.5f, 0.5f, 1.0f)      ,{ 1.0f, 1.0f, 0.0f, 1.0F },KVector::GREEN };
				ArrVtx[23] = { KVector(-0.5f, 0.5f, 0.5f, 1.0f)     ,{ 0.0f, 1.0f, 0.0f, 1.0F },KVector::GREEN };
			}

		}

		KVertexBuffer::Create(L"BOX", ArrVtx, D3D11_USAGE_DYNAMIC);

		std::vector<UINT> ArrIdx = { 0, 1, 2, 0, 2, 3 , 6, 5, 4 , 7, 6, 4 ,8, 9, 10 , 8, 10, 11 , 14, 13, 12 , 15, 14, 12 , 16, 17, 18 , 16, 18, 19 , 22, 21, 20 , 23, 22, 20 };
		KIndexBuffer::Create(L"BOX", ArrIdx, D3D11_USAGE_DYNAMIC);

		KMesh::Create(L"BOX");

	}


	{
		std::vector<KVertex2D> m_RECT;
		m_RECT.push_back({ { -1.f, 1.f, 0.0f, 1.0F },  { 0.0f, 0.0f, 0.0f, 1.0F }, KVector::RED });
		m_RECT.push_back({ { 1.f, 1.f, 0.0f, 1.0F },   { 1.0f, 0.0f, 0.0f, 1.0F }, KVector::RED });
		m_RECT.push_back({ { 1.f, -1.f, 0.0f, 1.0F },  { 1.0f, 1.0f, 0.0f, 1.0F }, KVector::GREEN });
		m_RECT.push_back({ { -1.f, -1.f, 0.0f, 1.0F }, { 0.0f, 1.0f, 0.0f, 1.0F }, KVector::GREEN });
		KVertexBuffer::Create(L"FULLRECT", m_RECT);

		// 인덱스 버퍼 완료
		std::vector<UINT> m_IDX = { 0, 1, 2, 0, 2, 3 };
		KIndexBuffer::Create(L"FULLRECT", m_IDX);

		KMesh::Create(L"FULLRECT");

	}

	{
		KFBX3DVertex fbxVertex;
		std::vector<KFBX3DVertex> sphereVertex;
		std::vector<DWORD> sphereIndex;

		float fRadius = 0.5f;

		fbxVertex.Pos = KVector{ 0.0f, fRadius, 0.0f, 1.0f };
		fbxVertex.Uv = KVector{ 0.5f, 0.0f };
		fbxVertex.Color = KVector{ 1.0f, 1.0f, 1.0f, 1.0f };
		fbxVertex.Normal = fbxVertex.Pos;
		fbxVertex.Normal.Normalize();
		fbxVertex.Normal.w = 0.0f;
		fbxVertex.Tangent = KVector{ 1.0f, 0.0f, 0.0f, 0.0f };
		fbxVertex.BiNormal = KVector{ 0.0f, 0.0f, 1.0f, 0.0f };

		sphereVertex.push_back(fbxVertex);

		UINT iStackCount = 40; // 가로 분할 개수
		UINT iSliceCount = 40; // 세로 분할 개수

		float yRotAngle = DirectX::XM_PI / (float)iStackCount;
		float zRotAngle = DirectX::XM_2PI / (float)iSliceCount;

		float yUvRatio = 1.0f / (float)iSliceCount;
		float zUvRatio = 1.0f / (float)iStackCount;

		// PointToDir

		for (UINT y = 1; y < iStackCount; ++y)
		{
			float phi = y * yRotAngle;

			for (UINT z = 0; z < iSliceCount + 1; ++z)
			{
				float theta = z * zRotAngle;

				fbxVertex.Pos = KVector
				{
					fRadius * sinf(y * yRotAngle) * cosf(z * zRotAngle),
					fRadius * cosf(y * yRotAngle),
					fRadius * sinf(y * yRotAngle) * sinf(z * zRotAngle),
					1.0f
				};


				fbxVertex.Pos.w = 1.0f;
				fbxVertex.Uv = KVector{ yUvRatio * z, zUvRatio * y };
				fbxVertex.Color = KVector{ 1.0f, 1.0f, 1.0f, 1.0f };
				fbxVertex.Normal = fbxVertex.Pos;
				fbxVertex.Normal.Normalize();
				fbxVertex.Normal.w = 0.0f;

				fbxVertex.Tangent.x = -fRadius * sinf(phi) * sinf(theta);
				fbxVertex.Tangent.y = 0.0f;
				fbxVertex.Tangent.z = fRadius * sinf(phi) * cosf(theta);
				fbxVertex.Tangent.Normalize();
				fbxVertex.Tangent.w = 0.0f;

				fbxVertex.BiNormal = KVector::Cross3D(fbxVertex.Tangent, fbxVertex.Normal);
				fbxVertex.BiNormal.Normalize();
				fbxVertex.BiNormal.w = 0.0f;

				sphereVertex.push_back(fbxVertex);
			}
		}

		// 남극점
		fbxVertex.Pos = KVector{ 0.0f, -fRadius, 0.0f, 1.0f };
		fbxVertex.Uv = KVector{ 0.5f, 1.0f };
		fbxVertex.Color = KVector{ 1.0f, 1.0f, 1.0f, 1.0f };
		fbxVertex.Normal = fbxVertex.Pos;
		fbxVertex.Normal.Normalize();
		fbxVertex.Normal.w = 0.0f;
		sphereVertex.push_back(fbxVertex);

		// 인덱스 버퍼 만들기
		sphereIndex.clear();
		for (UINT i = 0; i < iSliceCount; i++)
		{
			sphereIndex.push_back(0);
			sphereIndex.push_back(i + 2);
			sphereIndex.push_back(i + 1);
		}


		for (UINT y = 0; y < iStackCount - 2; ++y)
		{
			for (UINT z = 0; z < iSliceCount; ++z)
			{
				sphereIndex.push_back((iSliceCount + 1) * y + z + 1);
				sphereIndex.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);
				sphereIndex.push_back((iSliceCount + 1) * (y + 1) + z + 1);

				sphereIndex.push_back((iSliceCount + 1) * y + z + 1);
				sphereIndex.push_back((iSliceCount + 1) * y + (z + 1) + 1);
				sphereIndex.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);
			}
		}

		// 북극점 인덱스
		UINT iBotStart = (UINT)sphereVertex.size() - 1;

		for (UINT i = 0; i < iSliceCount; ++i)
		{
			sphereIndex.push_back(iBotStart);
			sphereIndex.push_back(iBotStart - (i + 2));
			sphereIndex.push_back(iBotStart - (i + 1));
		}

		KVertexBuffer::Create(L"SPHERE3D", sphereVertex);
		KIndexBuffer::Create(L"SPHERE3D", sphereIndex);
		KMesh::Create(L"SPHERE3D");
	}

}