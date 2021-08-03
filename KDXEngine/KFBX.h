#pragma once

#include <fbxsdk.h>
#include <DirectXTex.h>
#include "KGameResource.h"
#include "KGameDeviceManager.h"
#include "KShader.h"
#include "KRenderTarget.h"
#include "KVertexHeader.h"

#pragma comment(lib, "libfbxsdk-md.lib")

#define SAFE_DESTROY(pObject) { if(nullptr != pObject) { pObject->Destroy();}}

struct BoneFrameInfomation
{
	KVector Scale;
	KVector Quaternion;
	KVector Translation;
	double Time;
	KMatrix FrameMatrix;
};

class BoneFrameData
{
public:
	int BoneIndex;
	std::vector<BoneFrameInfomation> BoneFrameInfomationContainer;
};


struct KFBXAnimationData
{
	KGameString AnimationName;
	fbxsdk::FbxTime StartTime;
	fbxsdk::FbxTime EndTime;
	fbxsdk::FbxLongLong StartTimeCount;
	fbxsdk::FbxLongLong EndTimeCount;
	fbxsdk::FbxLongLong FrameCount;
	fbxsdk::FbxTime::EMode TimeMode;
	int FbxModeCount;

	std::vector<BoneFrameData> BoneFrameDataContainer;

	float FrameTime(int _frame)
	{
		fbxsdk::FbxTime time = {};
		time.SetFrame(_frame, TimeMode);

		return (float)time.GetSecondDouble();
	}

	int FrameModeCount()
	{
		switch (TimeMode)
		{
		case fbxsdk::FbxTime::eDefaultMode:
			return 0;
		case fbxsdk::FbxTime::eFrames120:
			return 120;
		case fbxsdk::FbxTime::eFrames100:
			return 100;
		case fbxsdk::FbxTime::eFrames60:
			return 60;
		case fbxsdk::FbxTime::eFrames50:
			return 50;
		case fbxsdk::FbxTime::eFrames48:
			return 48;
		case fbxsdk::FbxTime::eFrames30:
			return 30;
		case fbxsdk::FbxTime::eFrames30Drop:
			return 30;
		case fbxsdk::FbxTime::eNTSCDropFrame:
			return 0;
		case fbxsdk::FbxTime::eNTSCFullFrame:
			return 0;
		case fbxsdk::FbxTime::ePAL:
			return 0;
		case fbxsdk::FbxTime::eFrames24:
			return 24;
		case fbxsdk::FbxTime::eFrames1000:
			return 0;
		case fbxsdk::FbxTime::eFilmFullFrame:
			return 0;
		case fbxsdk::FbxTime::eCustom:
			return 0;
		case fbxsdk::FbxTime::eFrames96:
			return 0;
		case fbxsdk::FbxTime::eFrames72:
			return 0;
		case fbxsdk::FbxTime::eFrames59dot94:
			return 0;
		case fbxsdk::FbxTime::eModesCount:
			return 0;
		default:
			AssertMsg(L"처리할수 없는 프레임 카운트 모드입니다.");
			return 0;
		}
	}
};

struct KFBXIndexWeight
{
	int Index;
	double Weight;
};

struct KFBXBoneData
{
	KGameString BoneName;
	int Depth;
	int Index;
	int ParentIndex;
	KMatrix Offset;
	KMatrix Bone;
};

struct KFBXMaterialData
{
	KVector DiffuseColor;
	KVector AmbientColor;
	KVector SpecularColor;
	KVector EmissiveColor;

	float SpecularPower;
	float TransparencyFactor;
	float Shininess;

	KGameString DiffuseTexture;
	KGameString NormalTexture;
	KGameString SpecularTexture;
};

class KMesh;
class KVertexBuffer;
class KIndexBuffer;
class KRenderPipeline;
class KFBX : public KGameResource<KFBX>
{
public:
	static void Load(const KGameString& _path)
	{
		KFBX* pFbx = new KFBX();
		pFbx->m_Path = _path;
		pFbx->SetName(KGAMEPATH::GetFileName(_path));
		pFbx->Load();
		pFbx->InsertResource();
	}

	static void LoadFromUserFormat(const KGameString& _path, const KGameString& _userPath)
	{
		KFBX* pFbx = new KFBX();
		pFbx->m_Path = _path;
		pFbx->SetName(KGAMEPATH::GetFileName(_path));
		pFbx->LoadUserFormat(_userPath);
		pFbx->InsertResource();
	}

private:
	fbxsdk::FbxAMatrix m_RootMatrix;


private:
	fbxsdk::FbxManager* m_pManager;
	fbxsdk::FbxIOSettings* m_pIOSettings;
	fbxsdk::FbxScene* m_pScene;
	fbxsdk::FbxImporter* m_pImporter;
	fbxsdk::FbxAxisSystem eLocalSpaceType;

public:
	std::vector<KFBXAnimationData> m_UserAnimationContainer;
	std::map<KGameString, KFBXAnimationData*> m_UserAnimationMapContainer;
	std::vector<KFBXBoneData> m_UserBoneContainer;
	std::map<KGameString, KFBXBoneData*> m_UserBoneMapContainer;
	std::vector<KFBXMaterialData> m_UserMaterialDataContainer;

	std::vector<std::vector<KFBX3DVertex>> m_FbxVertexContainer;
	std::vector<std::vector<std::vector<UINT>>> m_FbxIndexContainer;

	std::vector<KPTR<KVertexBuffer>> m_vertexBufferContainer;
	std::vector<KPTR<KIndexBuffer>> m_indexBufferContainer;

	std::vector<KPTR<KMesh>> m_MeshContainer;

	std::vector<std::map<int, std::vector<KFBXIndexWeight>>> m_IndexWeightContainer;

	int m_MeshCount;

public:
	KFBX();
	~KFBX();

public:
	void Load();
	void LoadUserFormat(const KGameString& _path);
	void SaveUserFormat(const KGameString& _path);

	KFBXAnimationData* FindAnimationData(const KGameString& _name)
	{
		if (m_UserAnimationMapContainer.end() == m_UserAnimationMapContainer.find(_name))
		{
			return nullptr;
		}

		return m_UserAnimationMapContainer[_name];
	}

	size_t UserAnimationContainerSize()
	{
		return m_UserAnimationContainer.size();
	}

	KGameString UserAnimationDataName(size_t _index)
	{
		return m_UserAnimationContainer[_index].AnimationName;
	}


private:
	void CheckAnimation();
	void CheckBone();
	void CheckMaterial(fbxsdk::FbxNode* _pNode, int& _count);
	void CheckAnimationClip(fbxsdk::FbxNode* _pNode);

	void Triangulate(fbxsdk::FbxNode* _pNode, int& _matCount);

	void MeshVertexData(fbxsdk::FbxNode* _pNode);
	void VertexData(fbxsdk::FbxMesh* _pMesh);

	void CalculateAnimationClip(fbxsdk::FbxMesh* _pMesh);
	void CalculateWeightAndIndex();

	void ResizeBoneFrameData();

	void LoadBoneCount(fbxsdk::FbxNode* _pNode, int& _count);
	void LoadBoneData(fbxsdk::FbxNode* _pNode, int& _index, int _depth = 0, int _parent = -1);
	void LoadNormal(fbxsdk::FbxMesh* _pMesh, std::vector<KFBX3DVertex>& _vertexArray, int _vertexId, int _index);
	void LoadUV(fbxsdk::FbxMesh* _pMesh, std::vector<KFBX3DVertex>& _vertexArray, int _vertexId, int _index);
	void LoadTangent(fbxsdk::FbxMesh* _pMesh, std::vector<KFBX3DVertex>& _vertexArray, int _vertexId, int _index);
	void LoadBinormal(fbxsdk::FbxMesh* _pMesh, std::vector<KFBX3DVertex>& _vertexArray, int _vertexId, int _index);
	void LoadWeightAndIndex(fbxsdk::FbxCluster* _pCluster, int _BoneIndex, std::vector<KFBX3DVertex>& _vertexArray);
	void LoadOffsetMatrix(fbxsdk::FbxCluster* _pCluster, const fbxsdk::FbxAMatrix& _BaseMatrix, KFBXBoneData* _pBone);
	void LoadTimeTransForm(fbxsdk::FbxNode* _pNode, fbxsdk::FbxCluster* _pCluster, const fbxsdk::FbxAMatrix& _BaseMatrix, KFBXBoneData* _pBone);

	KVector MaterialColor(FbxSurfaceMaterial* _pMtrl, const char* _ColorName, const char* _FactorName);
	float MaterialFactor(FbxSurfaceMaterial* _pMtrl, const char* _FactorName);

	KGameString MaterialTexture(FbxSurfaceMaterial* _pMtrl, const char* _FactorName);

	KFBXBoneData* FindBone(const KGameString& _Name);

	static KMatrix FbxMatirxToKMatrix(const fbxsdk::FbxAMatrix& _baseTrans);
	static KVector FbxVectorToKVector(const fbxsdk::FbxVector4& _baseVector);
	static KVector FbxQuaternionToKVector(const fbxsdk::FbxQuaternion& _baseQuaternion);

public:
	void CreateMesh();
};

