#include "KFBX.h"
#include "KGameDebug3D.h"
#include "KMesh.h"

KFBX::KFBX() : m_pManager(nullptr), m_pImporter(nullptr), m_pIOSettings(nullptr), m_pScene(nullptr), m_MeshCount(0)
{
}

KFBX::~KFBX()
{
	SAFE_DESTROY(m_pImporter);
	SAFE_DESTROY(m_pScene);
	SAFE_DESTROY(m_pIOSettings);
	SAFE_DESTROY(m_pManager);
}

void KFBX::Load()
{
	KGameDebug3D::ConsoleText(m_Path + " LoadStart");

	m_RootMatrix.SetRow(0, fbxsdk::FbxVector4{ 1.0, 0.0, 0.0, 0.0 });
	m_RootMatrix.SetRow(1, fbxsdk::FbxVector4{ 0.0, 1.0, 0.0, 0.0 });
	m_RootMatrix.SetRow(2, fbxsdk::FbxVector4{ 0.0, 0.0, 1.0, 0.0 });
	m_RootMatrix.SetRow(3, fbxsdk::FbxVector4{ 0.0, 0.0, 0.0, 1.0 });

	m_pManager = fbxsdk::FbxManager::Create();

	m_pIOSettings = fbxsdk::FbxIOSettings::Create(m_pManager, IOSROOT);
	m_pScene = fbxsdk::FbxScene::Create(m_pManager, "");
	m_pImporter = fbxsdk::FbxImporter::Create(m_pManager, "");

	if (false == m_pImporter->Initialize(CW2A(m_Path, CP_UTF8).m_psz, -1, m_pManager->GetIOSettings()))
	{
		AssertMsg(m_Path + "FBX 파일 이니셜라이즈 실패");
	}

	m_pImporter->Import(m_pScene);

	eLocalSpaceType = m_pScene->GetGlobalSettings().GetAxisSystem();

	if (eLocalSpaceType != fbxsdk::FbxAxisSystem::eMax)
	{
		m_pScene->GetGlobalSettings().SetAxisSystem(fbxsdk::FbxAxisSystem::eMax);
	}

	CheckAnimation();
	KGameDebug3D::ConsoleText(L"애니메이션 체크 완료 애니메이션 개수 : %d\n", m_UserAnimationContainer.size());

	int materialCount = 0;
	Triangulate(m_pScene->GetRootNode(), materialCount);
	KGameDebug3D::ConsoleText(L"삼각화 완료");

	m_UserMaterialDataContainer.resize(materialCount);

	int materialIndex = 0;
	CheckMaterial(m_pScene->GetRootNode(), materialIndex);
	KGameDebug3D::ConsoleText(L"머티리얼 체크 완료");

	MeshVertexData(m_pScene->GetRootNode());

	if (0 < m_UserAnimationContainer.size())
	{
		CheckBone();
		KGameDebug3D::ConsoleText(L"본 체크 완료 본 개수 : %d", m_UserBoneContainer.size());

		ResizeBoneFrameData();

		m_IndexWeightContainer.resize(m_FbxVertexContainer.size());

		m_MeshCount = 0;
		CheckAnimationClip(m_pScene->GetRootNode());
		KGameDebug3D::ConsoleText(L"애니메이션 클립 체크 완료");

		m_MeshCount = 0;
		CalculateWeightAndIndex();
		KGameDebug3D::ConsoleText(L"Weight, 색인 완료");

		size_t userAnimationContainerSize = m_UserAnimationContainer.size();
		for (size_t i = 0; i < userAnimationContainerSize; ++i)
		{
			m_UserAnimationMapContainer[m_UserAnimationContainer[i].AnimationName] = &m_UserAnimationContainer[i];
		}

		KGameDebug3D::ConsoleText(L"본 애니메이션 완료 본 개수");

		m_IndexWeightContainer.clear();
	}

	CreateMesh();
	KGameDebug3D::ConsoleText(m_Path + L"매쉬 완료");
}

void KFBX::LoadUserFormat(const KGameString& _path)
{
	KGameFile loadFile = KGameFile(_path, L"rb");

	//버텍스 불러오기
	int vertexContainerSize = 0;
	loadFile >> vertexContainerSize;
	m_FbxVertexContainer.resize(vertexContainerSize);

	for (int fbxVertexId = 0; fbxVertexId < vertexContainerSize; ++fbxVertexId)
	{

		int fbxVertexSize = 0;
		loadFile >> fbxVertexSize;
		m_FbxVertexContainer[fbxVertexId].resize(fbxVertexSize);

		for (int vertexId = 0; vertexId < fbxVertexSize; ++vertexId)
		{
			loadFile >> m_FbxVertexContainer[fbxVertexId][vertexId];
		}
	}

	//인덱스 불러오기
	int fbxIndexContainerSize = 0;
	loadFile >> fbxIndexContainerSize;
	m_FbxIndexContainer.resize(fbxIndexContainerSize);

	for (int fbxIndexId = 0; fbxIndexId < fbxIndexContainerSize; ++fbxIndexId)
	{
		int fbxIndexSize = 0;
		loadFile >> fbxIndexSize;

		m_FbxIndexContainer[fbxIndexId].resize(fbxIndexSize);

		for (int i = 0; i < fbxIndexSize; ++i)
		{
			int fbxIndexIndexSize = 0;
			loadFile >> fbxIndexIndexSize;
			m_FbxIndexContainer[fbxIndexId][i].resize(fbxIndexIndexSize);

			for (int j = 0; j < fbxIndexIndexSize; ++j)
			{
				loadFile >> m_FbxIndexContainer[fbxIndexId][i][j];
			}
		}
	}

	//머테리얼 불러오기
	int materialContainerSize = 0;
	loadFile >> materialContainerSize;
	m_UserMaterialDataContainer.resize(materialContainerSize);

	for (int i = 0; i < materialContainerSize; ++i)
	{
		loadFile >> m_UserMaterialDataContainer[i].DiffuseColor;
		loadFile >> m_UserMaterialDataContainer[i].AmbientColor;
		loadFile >> m_UserMaterialDataContainer[i].SpecularColor;
		loadFile >> m_UserMaterialDataContainer[i].EmissiveColor;
		loadFile >> m_UserMaterialDataContainer[i].SpecularPower;
		loadFile >> m_UserMaterialDataContainer[i].TransparencyFactor;
		loadFile >> m_UserMaterialDataContainer[i].Shininess;
		loadFile >> m_UserMaterialDataContainer[i].DiffuseTexture;
		loadFile >> m_UserMaterialDataContainer[i].NormalTexture;
		loadFile >> m_UserMaterialDataContainer[i].SpecularTexture;
	}


	int userAnimationContainerSize = 0;
	loadFile >> userAnimationContainerSize;

	if (-1 == userAnimationContainerSize)
	{
		return;
	}

	//애니메이션 불러오기
	m_UserAnimationContainer.resize(userAnimationContainerSize);

	for (int i = 0; i < userAnimationContainerSize; ++i)
	{
		loadFile >> m_UserAnimationContainer[i].AnimationName;
		loadFile >> m_UserAnimationContainer[i].StartTime;
		loadFile >> m_UserAnimationContainer[i].EndTime;
		loadFile >> m_UserAnimationContainer[i].StartTimeCount;
		loadFile >> m_UserAnimationContainer[i].EndTimeCount;
		loadFile >> m_UserAnimationContainer[i].FrameCount;
		loadFile >> m_UserAnimationContainer[i].TimeMode;
		loadFile >> m_UserAnimationContainer[i].FbxModeCount;

		int boneFrameDataContainerSize = 0;
		loadFile >> boneFrameDataContainerSize;
		m_UserAnimationContainer[i].BoneFrameDataContainer.resize(boneFrameDataContainerSize);

		for (int j = 0; j < boneFrameDataContainerSize; j++)
		{
			loadFile >> m_UserAnimationContainer[i].BoneFrameDataContainer[j].BoneIndex;

			int boneFrameInfomationContainerSize = 0;
			loadFile >> boneFrameInfomationContainerSize;
			m_UserAnimationContainer[i].BoneFrameDataContainer[j].BoneFrameInfomationContainer.resize(boneFrameInfomationContainerSize);

			for (int k = 0; k < boneFrameInfomationContainerSize; k++)
			{
				loadFile >> m_UserAnimationContainer[i].BoneFrameDataContainer[j].BoneFrameInfomationContainer[k];
			}
		}
	}

	//본 저장
	int boneContainerSize = 0;
	loadFile >> boneContainerSize;
	m_UserBoneContainer.resize(boneContainerSize);

	for (int i = 0; i < boneContainerSize; i++)
	{
		loadFile >> m_UserBoneContainer[i].BoneName;
		loadFile >> m_UserBoneContainer[i].Depth;
		loadFile >> m_UserBoneContainer[i].Index;
		loadFile >> m_UserBoneContainer[i].ParentIndex;
		loadFile >> m_UserBoneContainer[i].Offset;
		loadFile >> m_UserBoneContainer[i].Bone;
	}

	for (size_t i = 0; i < m_UserAnimationContainer.size(); i++)
	{
		m_UserAnimationMapContainer[m_UserAnimationContainer[i].AnimationName] = &m_UserAnimationContainer[i];
	}

	for (size_t i = 0; i < m_UserBoneContainer.size(); i++)
	{
		m_UserBoneMapContainer[m_UserBoneContainer[i].BoneName] = &m_UserBoneContainer[i];
	}

	CreateMesh();
}

void KFBX::SaveUserFormat(const KGameString& _path)
{
	KGameFile saveFile(_path, "wb");

	//버텍스 저장
	size_t fbxVertexContainerSize = m_FbxVertexContainer.size();
	saveFile << (int)fbxVertexContainerSize;

	for (size_t fbxVertexId = 0; fbxVertexId < fbxVertexContainerSize; ++fbxVertexId)
	{
		size_t fbxVertexSize = m_FbxVertexContainer[fbxVertexId].size();
		saveFile << (int)fbxVertexSize;

		for (size_t i = 0; i < fbxVertexSize; ++i)
		{
			saveFile << m_FbxVertexContainer[fbxVertexId][i];
		}
	}

	//인덱스 저장
	size_t fbxIndexContainerSize = m_FbxIndexContainer.size();
	saveFile << (int)fbxIndexContainerSize;

	for (size_t fbxIndexId = 0; fbxIndexId < fbxIndexContainerSize; ++fbxIndexId)
	{
		size_t fbxIndexSize = m_FbxIndexContainer[fbxIndexId].size();
		saveFile << (int)fbxIndexSize;

		for (size_t i = 0; i < fbxIndexSize; ++i)
		{
			size_t fbxIndexIndexSize = m_FbxIndexContainer[fbxIndexId][i].size();
			saveFile << (int)fbxIndexIndexSize;

			for (size_t j = 0; j < fbxIndexSize; ++j)
			{
				saveFile << m_FbxIndexContainer[fbxIndexId][i][j];
			}
		}
	}

	//머테리얼 저장
	size_t materialContainerSize = m_UserMaterialDataContainer.size();
	saveFile << (int)materialContainerSize;

	for (size_t i = 0; i < materialContainerSize; ++i)
	{
		saveFile << m_UserMaterialDataContainer[i].DiffuseColor;
		saveFile << m_UserMaterialDataContainer[i].AmbientColor;
		saveFile << m_UserMaterialDataContainer[i].SpecularColor;
		saveFile << m_UserMaterialDataContainer[i].EmissiveColor;
		saveFile << m_UserMaterialDataContainer[i].SpecularPower;
		saveFile << m_UserMaterialDataContainer[i].TransparencyFactor;
		saveFile << m_UserMaterialDataContainer[i].Shininess;
		saveFile << m_UserMaterialDataContainer[i].DiffuseTexture;
		saveFile << m_UserMaterialDataContainer[i].NormalTexture;
		saveFile << m_UserMaterialDataContainer[i].SpecularTexture;
	}

	if (0 >= m_UserAnimationContainer.size())
	{
		saveFile << -1;
		return;
	}

	//애니메이션 저장
	size_t animationDataContainerSize = m_UserAnimationContainer.size();
	saveFile << (int)animationDataContainerSize;
	for (size_t i = 0; i < animationDataContainerSize; ++i)
	{
		saveFile << m_UserAnimationContainer[i].AnimationName;
		saveFile << m_UserAnimationContainer[i].StartTime;
		saveFile << m_UserAnimationContainer[i].EndTime;
		saveFile << m_UserAnimationContainer[i].StartTimeCount;
		saveFile << m_UserAnimationContainer[i].EndTimeCount;
		saveFile << m_UserAnimationContainer[i].FrameCount;
		saveFile << m_UserAnimationContainer[i].TimeMode;
		saveFile << m_UserAnimationContainer[i].FbxModeCount;

		saveFile << (int)m_UserAnimationContainer[i].BoneFrameDataContainer.size();
		for (size_t j = 0; j < m_UserAnimationContainer[i].BoneFrameDataContainer.size(); j++)
		{
			saveFile << m_UserAnimationContainer[i].BoneFrameDataContainer[j].BoneIndex;
			saveFile << (int)m_UserAnimationContainer[i].BoneFrameDataContainer[j].BoneFrameInfomationContainer.size();
			for (size_t k = 0; k < m_UserAnimationContainer[i].BoneFrameDataContainer[j].BoneFrameInfomationContainer.size(); k++)
			{
				saveFile << m_UserAnimationContainer[i].BoneFrameDataContainer[j].BoneFrameInfomationContainer[k];
			}
		}
	}

	//본 저장
	size_t boneContainerSize = m_UserBoneContainer.size();
	saveFile << (int)boneContainerSize;

	for (size_t i = 0; i < boneContainerSize; i++)
	{
		saveFile << m_UserBoneContainer[i].BoneName;
		saveFile << m_UserBoneContainer[i].Depth;
		saveFile << m_UserBoneContainer[i].Index;
		saveFile << m_UserBoneContainer[i].ParentIndex;
		saveFile << m_UserBoneContainer[i].Offset;
		saveFile << m_UserBoneContainer[i].Bone;
	}
}

void KFBX::CheckAnimation()
{
	fbxsdk::FbxArray<FbxString*> animationNameArray;

	m_pScene->FillAnimStackNameArray(animationNameArray);

	if (0 == animationNameArray.Size())
	{
		return;
	}

	m_UserAnimationContainer.resize(animationNameArray.Size());

	int count = animationNameArray.GetCount();
	for (int i = 0; i < count; ++i)
	{
		FbxAnimStack* pAnimationStack = m_pScene->FindMember<FbxAnimStack>(animationNameArray[i]->Buffer());

		if (nullptr == pAnimationStack)
		{
			AssertMsg(L"pAniStack is null");
			continue;
		}

		m_UserAnimationContainer[i].AnimationName = CA2W(pAnimationStack->GetName(), CP_UTF8).m_psz;

		FbxTakeInfo* TakeInfo = m_pScene->GetTakeInfo(pAnimationStack->GetName());

		m_UserAnimationContainer[i].StartTime = TakeInfo->mLocalTimeSpan.GetStart();
		m_UserAnimationContainer[i].EndTime = TakeInfo->mLocalTimeSpan.GetStop();
		m_UserAnimationContainer[i].TimeMode = m_pScene->GetGlobalSettings().GetTimeMode();
		m_UserAnimationContainer[i].StartTimeCount = m_UserAnimationContainer[i].StartTime.GetFrameCount(m_UserAnimationContainer[i].TimeMode);
		m_UserAnimationContainer[i].EndTimeCount = m_UserAnimationContainer[i].EndTime.GetFrameCount(m_UserAnimationContainer[i].TimeMode);
		m_UserAnimationContainer[i].FrameCount = m_UserAnimationContainer[i].EndTimeCount - m_UserAnimationContainer[i].StartTimeCount;
		m_UserAnimationContainer[i].FbxModeCount = m_UserAnimationContainer[i].FrameModeCount();
	}

	for (int i = 0; i < count; ++i)
	{
		delete animationNameArray[i];
	}

}

void KFBX::CheckBone()
{
	int boneCount = 0;
	LoadBoneCount(m_pScene->GetRootNode(), boneCount);

	if (0 < boneCount)
	{
		m_UserBoneContainer.resize(boneCount);
	}

	int startBoneIndex = 0;
	LoadBoneData(m_pScene->GetRootNode(), startBoneIndex);
}

void KFBX::Triangulate(fbxsdk::FbxNode* _pNode, int& _matCount)
{
	fbxsdk::FbxNodeAttribute* _pAttribute = _pNode->GetNodeAttribute();

	if (nullptr != _pAttribute &&
		(_pAttribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh ||
			_pAttribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eNurbs ||
			_pAttribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eNurbsSurface)
		)
	{
		if ((_pAttribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eNurbs ||
			_pAttribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eNurbsSurface))
		{
			int a = 0;
		}
		FbxGeometryConverter Con(m_pManager);
		Con.Triangulate(_pAttribute, true);
	}

	_matCount += _pNode->GetMaterialCount();

	int ChildCount = _pNode->GetChildCount();

	for (int i = 0; i < ChildCount; i++)
	{
		Triangulate(_pNode->GetChild(i), _matCount);
	}


}

void KFBX::CheckMaterial(fbxsdk::FbxNode* _pNode, int& _count)
{
	fbxsdk::FbxNodeAttribute* _pAttribute = _pNode->GetNodeAttribute();

	int materialCount = _pNode->GetMaterialCount();

	if (materialCount > 0)
	{
		for (int i = 0; i < materialCount; ++i)
		{
			FbxSurfaceMaterial* pMaterial = _pNode->GetMaterial(i);

			if (nullptr == pMaterial)
			{
				AssertMsg(L"pMaterial is null");
			}

			m_UserMaterialDataContainer[_count].DiffuseColor = MaterialColor(pMaterial, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
			m_UserMaterialDataContainer[_count].AmbientColor = MaterialColor(pMaterial, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
			m_UserMaterialDataContainer[_count].SpecularColor = MaterialColor(pMaterial, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);
			m_UserMaterialDataContainer[_count].EmissiveColor = MaterialColor(pMaterial, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor);
			m_UserMaterialDataContainer[_count].SpecularPower = MaterialFactor(pMaterial, FbxSurfaceMaterial::sSpecularFactor);
			m_UserMaterialDataContainer[_count].Shininess = MaterialFactor(pMaterial, FbxSurfaceMaterial::sShininess);
			m_UserMaterialDataContainer[_count].TransparencyFactor = MaterialFactor(pMaterial, FbxSurfaceMaterial::sTransparencyFactor);

			m_UserMaterialDataContainer[_count].DiffuseTexture = MaterialTexture(pMaterial, FbxSurfaceMaterial::sDiffuse);
			m_UserMaterialDataContainer[_count].NormalTexture = MaterialTexture(pMaterial, FbxSurfaceMaterial::sNormalMap);
			m_UserMaterialDataContainer[_count].SpecularTexture = MaterialTexture(pMaterial, FbxSurfaceMaterial::sSpecular);

			++_count;
		}
	}

	int childCount = _pNode->GetChildCount();

	for (int i = 0; i < childCount; ++i)
	{
		CheckMaterial(_pNode->GetChild(i), _count);
	}
}

void KFBX::LoadBoneCount(fbxsdk::FbxNode* _pNode, int& _count)
{
	fbxsdk::FbxNodeAttribute* pAttribute = _pNode->GetNodeAttribute();

	if (nullptr != pAttribute && (pAttribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton))
	{
		++_count;
	}

	int childCount = _pNode->GetChildCount();

	for (int i = 0; i < childCount; ++i)
	{
		LoadBoneCount(_pNode->GetChild(i), _count);
	}
}

KVector KFBX::MaterialColor(FbxSurfaceMaterial* _pMtrl, const char* _ColorName, const char* _FactorName)
{
	FbxDouble3 vResult(0, 0, 0);
	double dFactor = 0;
	FbxProperty ColorPro = _pMtrl->FindProperty(_ColorName);
	FbxProperty FactorPro = _pMtrl->FindProperty(_FactorName);

	if (true == ColorPro.IsValid() && true == FactorPro.IsValid())
	{
		vResult = ColorPro.Get<FbxDouble3>();
		dFactor = FactorPro.Get<FbxDouble>();

		if (dFactor != 1)
		{
			vResult[0] *= dFactor;
			vResult[1] *= dFactor;
			vResult[2] *= dFactor;
		}
	}

	return KVector((float)vResult[0], (float)vResult[1], (float)vResult[2]);
}

float KFBX::MaterialFactor(FbxSurfaceMaterial* _pMtrl, const char* _FactorName)
{
	double dFactor = 0;
	FbxProperty FactorPro = _pMtrl->FindProperty(_FactorName);

	if (true == FactorPro.IsValid())
	{
		dFactor = FactorPro.Get<FbxDouble>();
	}

	return (float)dFactor;
}

KGameString KFBX::MaterialTexture(FbxSurfaceMaterial* _pMtrl, const char* _FactorName)
{
	FbxProperty TexturePro = _pMtrl->FindProperty(_FactorName);
	KGameString Str;
	if (true == TexturePro.IsValid())
	{
		int iTexCount = TexturePro.GetSrcObjectCount<FbxFileTexture>();

		if (iTexCount > 0)
		{
			FbxFileTexture* pFileTex = TexturePro.GetSrcObject<FbxFileTexture>(0);

			if (nullptr != pFileTex)
			{
				Str = pFileTex->GetFileName();
			}
		}
		else
		{
			AssertMsg(L"if (iTexCount > 0)");
		}

	}
	else
	{
		AssertMsg(L"if (true != TexturePro.IsValid())");
	}

	return Str;
}

KFBXBoneData* KFBX::FindBone(const KGameString& _Name)
{
	if (m_UserBoneMapContainer.end() == m_UserBoneMapContainer.find(_Name))
	{
		AssertMsg(_Name + L" 해당 본 없음.");
	}

	return m_UserBoneMapContainer[_Name];
}

KMatrix KFBX::FbxMatirxToKMatrix(const fbxsdk::FbxAMatrix& _BaseTrans)
{
	KMatrix matrix;

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			matrix.Arr2D[y][x] = (float)_BaseTrans.Get(y, x);
		}
	}

	return matrix;
}

KVector KFBX::FbxVectorToKVector(const fbxsdk::FbxVector4& _baseVector)
{
	KVector vector;
	for (int i = 0; i < 4; ++i)
	{
		vector.Arr[i] = (float)_baseVector.mData[i];
	}

	return vector;
}

KVector KFBX::FbxQuaternionToKVector(const fbxsdk::FbxQuaternion& _baseQuaternion)
{
	KVector vector;
	for (int i = 0; i < 4; ++i)
	{
		vector.Arr[i] = (float)_baseQuaternion.mData[i];
	}

	return vector;
}

void KFBX::CreateMesh()
{
	if (m_FbxIndexContainer.size() != m_FbxVertexContainer.size())
	{
		AssertMsg(L"인덱스 버퍼 버텍스 버퍼 매칭 오류");
		return;
	}

	m_vertexBufferContainer.resize(m_FbxVertexContainer.size());
	m_indexBufferContainer.resize(m_FbxIndexContainer.size());

	size_t fbxVertexContainerSize = m_FbxVertexContainer.size();
	for (size_t fbxVertexId = 0; fbxVertexId < fbxVertexContainerSize; ++fbxVertexId)
	{
		KPTR<KVertexBuffer> vertexBuffer = make_KPTR<KVertexBuffer>();
		vertexBuffer->Create(m_FbxVertexContainer[fbxVertexId].size(), sizeof(KFBX3DVertex), &m_FbxVertexContainer[fbxVertexId][0], D3D11_USAGE::D3D11_USAGE_DEFAULT);
		m_vertexBufferContainer.push_back(vertexBuffer);

		size_t fbxIndexContainerSize = m_FbxIndexContainer[fbxVertexId].size();
		for (size_t fbxIndexId = 0; fbxIndexId < fbxIndexContainerSize; ++fbxIndexId)
		{
			KPTR<KIndexBuffer> indexBuffer = make_KPTR<KIndexBuffer>();
			indexBuffer->SetFormat(DXGI_FORMAT::DXGI_FORMAT_R32_UINT);
			indexBuffer->Create(m_FbxIndexContainer[fbxVertexId][fbxIndexId].size(), sizeof(UINT), &m_FbxIndexContainer[fbxVertexId][fbxIndexId][0], D3D11_USAGE::D3D11_USAGE_DEFAULT);
			m_indexBufferContainer.push_back(indexBuffer);

			KPTR<KMesh> pMesh = make_KPTR<KMesh>();
			pMesh->AddVertexBuffer(vertexBuffer);
			pMesh->AddIndexBuffer(indexBuffer);

			m_MeshContainer.push_back(pMesh);
		}
	}

	if (m_MeshContainer.size() != m_UserMaterialDataContainer.size())
	{
		AssertMsg(L"매쉬와 메테리얼의 크기가 다릅니다.");
		return;
	}

	KGameDebug3D::ConsoleText(L"버텍스 및 인덱스 버퍼 버퍼 매쉬생성 완료");
}

void KFBX::LoadBoneData(fbxsdk::FbxNode* _pNode, int& _index, int _depth/* = 0*/, int _parent /*= -1*/)
{
	fbxsdk::FbxNodeAttribute* pAttribute = _pNode->GetNodeAttribute();

	int parentIndex = _index;

	if (nullptr != pAttribute && (pAttribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton))
	{
		m_UserBoneContainer[_index].BoneName = CA2W(_pNode->GetName()).m_psz;
		m_UserBoneContainer[_index].Index = _index;
		m_UserBoneContainer[_index].ParentIndex = parentIndex;
		m_UserBoneContainer[_index].Depth = _depth;

		m_UserBoneMapContainer[m_UserBoneContainer[_index].BoneName] = &m_UserBoneContainer[_index];

		++_index;
	}

	int childCount = _pNode->GetChildCount();
	for (int i = 0; i < childCount; ++i)
	{
		LoadBoneData(_pNode->GetChild(i), _index, _depth + 1, _index);
	}
}

void KFBX::MeshVertexData(fbxsdk::FbxNode* _pNode)
{
	fbxsdk::FbxNodeAttribute* _pAttribute = _pNode->GetNodeAttribute();

	if (nullptr != _pAttribute &&
		(_pAttribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh ||
			_pAttribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eNurbs ||
			_pAttribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eNurbsSurface)
		)
	{
		FbxMesh* _pMesh = _pNode->GetMesh();

		if (nullptr == _pMesh)
		{
			AssertMsg(L"if (nullptr == _Mesh)");
		}

		VertexData(_pMesh);
	}

	int ChildCount = _pNode->GetChildCount();
	for (int i = 0; i < ChildCount; i++)
	{
		MeshVertexData(_pNode->GetChild(i));
	}
}

void KFBX::VertexData(fbxsdk::FbxMesh* _pMesh)
{
	m_FbxVertexContainer.push_back(std::vector<KFBX3DVertex>());
	m_FbxIndexContainer.push_back(std::vector<std::vector<UINT>>());

	std::vector<KFBX3DVertex>& vertexData = m_FbxVertexContainer[m_FbxVertexContainer.size() - 1];
	std::vector<std::vector<UINT>>& indexData = m_FbxIndexContainer[m_FbxIndexContainer.size() - 1];

	int iVtxCount = _pMesh->GetControlPointsCount();

	FbxVector4* pVertexPos = _pMesh->GetControlPoints();
	vertexData.resize(iVtxCount);

	for (int i = 0; i < vertexData.size(); i++)
	{
		//Max 축에서 변환
		vertexData[i].Pos.x = (float)pVertexPos[i].mData[0];
		vertexData[i].Pos.y = (float)pVertexPos[i].mData[2];
		vertexData[i].Pos.z = (float)pVertexPos[i].mData[1];
		vertexData[i].Pos.w = 1.0F;
	}

	UINT vertexId = 0;

	FbxGeometryElementMaterial* pMaterial = _pMesh->GetElementMaterial();
	int materialCount = _pMesh->GetNode()->GetMaterialCount();
	indexData.resize(materialCount);

	int polygonCount = _pMesh->GetPolygonCount();
	for (int polgonIndex = 0; polgonIndex < polygonCount; polgonIndex++)
	{
		int iPolyGonSize = _pMesh->GetPolygonSize(polgonIndex);
		//삼각형이 아닐 경우
		if (3 != iPolyGonSize)
		{
			AssertMsg(L"삼각형이 아닌 면이 발견됐습니다.");
		}

		int index[3] = { 0, };
		for (int TriIndex = 0; TriIndex < iPolyGonSize; TriIndex++)
		{
			int iControlIndex = _pMesh->GetPolygonVertex(polgonIndex, TriIndex);

			index[TriIndex] = iControlIndex;

			LoadNormal(_pMesh, vertexData, vertexId, iControlIndex);
			LoadUV(_pMesh, vertexData, vertexId, iControlIndex);
			LoadTangent(_pMesh, vertexData, vertexId, iControlIndex);
			LoadBinormal(_pMesh, vertexData, vertexId, iControlIndex);
			++vertexId;
		}

		int materialId = pMaterial->GetIndexArray().GetAt(polgonIndex);
		indexData[materialId].push_back(index[0]);
		indexData[materialId].push_back(index[2]);
		indexData[materialId].push_back(index[1]);
	}
}

void KFBX::CheckAnimationClip(fbxsdk::FbxNode* _pNode)
{
	fbxsdk::FbxNodeAttribute* pAttribute = _pNode->GetNodeAttribute();

	if (nullptr != pAttribute &&
		(pAttribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh ||
			pAttribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eNurbs ||
			pAttribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eNurbsSurface)
		)
	{
		fbxsdk::FbxMesh* pMesh = _pNode->GetMesh();

		if (nullptr == pMesh)
		{
			AssertMsg(L"Mesh를 얻을 수 없습니다.");
		}

		CalculateAnimationClip(pMesh);
	}

	int childCount = _pNode->GetChildCount();

	for (int i = 0; i < childCount; ++i)
	{
		CheckAnimationClip(_pNode->GetChild(i));
	}
}

void KFBX::CalculateAnimationClip(fbxsdk::FbxMesh* _pMesh)
{
	int skinCount = _pMesh->GetDeformerCount(fbxsdk::FbxDeformer::eSkin);


	if (0 >= skinCount)
	{
		AssertMsg(L"스키닝 정보 없음.");
	}

	int vertexCount = (int)m_FbxVertexContainer[m_MeshCount].size();

	fbxsdk::FbxNode* pNode = _pMesh->GetNode();
	const fbxsdk::FbxVector4 scale = pNode->GetGeometricScaling(fbxsdk::FbxNode::eSourcePivot);
	const fbxsdk::FbxVector4 rotate = pNode->GetGeometricRotation(fbxsdk::FbxNode::eSourcePivot);
	const fbxsdk::FbxVector4 translation = pNode->GetGeometricTranslation(fbxsdk::FbxNode::eSourcePivot);

	FbxAMatrix matrix = FbxAMatrix(translation, rotate, scale);

	for (int i = 0; i < skinCount; ++i)
	{
		fbxsdk::FbxSkin* pSkin = (fbxsdk::FbxSkin*)_pMesh->GetDeformer(i, fbxsdk::FbxDeformer::eSkin);

		if (nullptr == pSkin)
		{
			continue;
		}

		fbxsdk::FbxSkin::EType skinningType = pSkin->GetSkinningType();

		if (skinningType == fbxsdk::FbxSkin::eRigid ||
			skinningType == fbxsdk::FbxSkin::eLinear)
		{
			int clusterCount = pSkin->GetClusterCount();

			for (int j = 0; j < clusterCount; j++)
			{
				fbxsdk::FbxCluster* pCluster = pSkin->GetCluster(j);

				if (nullptr == pCluster->GetLink())
				{
					continue;
				}

				KGameString boneName = CA2W(pCluster->GetLink()->GetName()).m_psz;

				KFBXBoneData* pBone = FindBone(boneName);

				if (nullptr == pBone)
				{
					continue;
				}

				LoadWeightAndIndex(pCluster, pBone->Index, m_FbxVertexContainer[m_MeshCount]);
				LoadOffsetMatrix(pCluster, matrix, pBone);
				LoadTimeTransForm(pNode, pCluster, matrix, pBone);
			}
		}
	}

	++m_MeshCount;
}

void KFBX::CalculateWeightAndIndex()
{
	for (auto& indexWeight : m_IndexWeightContainer[m_MeshCount])
	{
		if (indexWeight.second.size() > 4)
		{
			AssertMsg(L"가중치 계산에 4개 이상의 값이 들어왔습니다.");

			std::sort(indexWeight.second.begin(), indexWeight.second.end(),
				[](const KFBXIndexWeight& _left, const KFBXIndexWeight& _right)
				{
					return _left.Weight > _right.Weight;
				}
			);

			double sum = 0.0;

			size_t indexWeightSize = indexWeight.second.size();
			for (size_t i = 0; i < indexWeightSize; ++i)
			{
				sum += indexWeight.second[i].Weight;
			}

			double interpolate = 1.0f - sum;

			std::vector<KFBXIndexWeight>::iterator iterErase = indexWeight.second.begin() + 4;

			indexWeight.second.erase(iterErase, indexWeight.second.end());
			indexWeight.second[0].Weight += interpolate;
		}

		float weight[4] = { 0.0f, };
		int index[4] = { 0, };

		size_t indexWeightSize = indexWeight.second.size();
		for (size_t i = 0; i < indexWeightSize; ++i)
		{
			weight[i] = (float)indexWeight.second[i].Weight;
			index[i] = indexWeight.second[i].Index;
		}

		memcpy_s(m_FbxVertexContainer[m_MeshCount][indexWeight.first].Weight.Arr, sizeof(KVector), weight, sizeof(KVector));
		memcpy_s(m_FbxVertexContainer[m_MeshCount][indexWeight.first].Index, sizeof(KVector), index, sizeof(KVector));
	}
}

void KFBX::ResizeBoneFrameData()
{
	size_t userAnimationDataSize = m_UserAnimationContainer.size();

	for (size_t animationIndex = 0; animationIndex < userAnimationDataSize; ++animationIndex)
	{
		m_UserAnimationContainer[animationIndex].BoneFrameDataContainer.resize(m_UserBoneContainer.size());

		size_t boneContainerSize = m_UserBoneContainer.size();
		for (size_t boneIndex = 0; boneIndex < boneContainerSize; ++boneIndex)
		{
			m_UserAnimationContainer[animationIndex].BoneFrameDataContainer[boneIndex].BoneFrameInfomationContainer.resize(m_UserAnimationContainer[animationIndex].FrameCount + 1);
		}
	}
}

void KFBX::LoadNormal(fbxsdk::FbxMesh* _pMesh, std::vector<KFBX3DVertex>& _vertexArray, int _vertexId, int _index)
{
	int count = _pMesh->GetElementNormalCount();

	if (0 == count)
	{
		AssertMsg(L"GetElementNormalCount가 여러개 입니다.");
	}

	FbxGeometryElementNormal* pElement = _pMesh->GetElementNormal();
	int dataIndex = _vertexId;

	// 폴리곤 기반으로 로드.
	if (pElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			dataIndex = pElement->GetIndexArray().GetAt(_vertexId);
		}
	}
	else if (pElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (FbxGeometryElement::eDirect == pElement->GetReferenceMode())
		{
			dataIndex = _index;
		}
		else if (FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			dataIndex = pElement->GetIndexArray().GetAt(_index);
		}
		else {
			AssertMsg(L"맵핑 방식중 처리할수 없는 방식입니다.");
		}
	}

	FbxVector4 Normal = pElement->GetDirectArray().GetAt(dataIndex);

	_vertexArray[_index].Normal.x = (float)Normal.mData[0];
	_vertexArray[_index].Normal.y = (float)Normal.mData[2];
	_vertexArray[_index].Normal.z = (float)Normal.mData[1];

	_vertexArray[_index].Normal.Normalize();
}

void KFBX::LoadUV(fbxsdk::FbxMesh* _pMesh, std::vector<KFBX3DVertex>& _vertexArray, int _vertexId, int _index)
{
	int count = _pMesh->GetElementUVCount();

	if (0 == count)
	{
		AssertMsg(L"UV가 여러개 입니다.");
	}

	FbxGeometryElementUV* pElementUV = _pMesh->GetElementUV();
	int dataIndex = _vertexId;

	if (pElementUV->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (FbxGeometryElement::eIndexToDirect == pElementUV->GetReferenceMode())
		{
			dataIndex = pElementUV->GetIndexArray().GetAt(_vertexId);
		}
		else if (FbxGeometryElement::eDirect == pElementUV->GetReferenceMode())
		{
			dataIndex = _index;
		}
	}
	else if (pElementUV->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (FbxGeometryElement::eDirect == pElementUV->GetReferenceMode())
		{
			dataIndex = _index;
		}
		else if (FbxGeometryElement::eIndexToDirect == pElementUV->GetReferenceMode())
		{
			dataIndex = pElementUV->GetIndexArray().GetAt(_index);
		}
		else {
			AssertMsg(L"맵핑 방식중 처리할수 없는 방식입니다.");
		}
	}

	FbxVector2 vUV = pElementUV->GetDirectArray().GetAt(dataIndex);

	_vertexArray[_index].Uv.x = (float)(vUV.mData[0] - (int)vUV.mData[0]);
	_vertexArray[_index].Uv.y = (float)(1.0f - (vUV.mData[1] - (int)vUV.mData[1]));
}

void KFBX::LoadTangent(fbxsdk::FbxMesh* _pMesh, std::vector<KFBX3DVertex>& _vertexArray, int _vertexId, int _index)
{
	int count = _pMesh->GetElementTangentCount();

	if (0 == count)
	{
		AssertMsg(L"GetElementTangentCount가 여러개 입니다.");
	}

	FbxGeometryElementTangent* pElementTangent = _pMesh->GetElementTangent();
	int dataIndex = _vertexId;

	if (pElementTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (FbxGeometryElement::eIndexToDirect == pElementTangent->GetReferenceMode())
		{
			dataIndex = pElementTangent->GetIndexArray().GetAt(_vertexId);
		}
	}
	else if (pElementTangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (FbxGeometryElement::eDirect == pElementTangent->GetReferenceMode())
		{
			dataIndex = _index;
		}
		else if (FbxGeometryElement::eIndexToDirect == pElementTangent->GetReferenceMode())
		{
			dataIndex = pElementTangent->GetIndexArray().GetAt(_index);
		}
		else {
			AssertMsg(L"맵핑 방식중 처리할수 없는 방식입니다.");
		}
	}

	FbxVector4 Tangent = pElementTangent->GetDirectArray().GetAt(dataIndex);

	_vertexArray[_index].Tangent.x = (float)Tangent.mData[0];
	_vertexArray[_index].Tangent.y = (float)Tangent.mData[2];
	_vertexArray[_index].Tangent.z = (float)Tangent.mData[1];

	_vertexArray[_index].Tangent.Normalize();
}

void KFBX::LoadBinormal(fbxsdk::FbxMesh* _pMesh, std::vector<KFBX3DVertex>& _vertexArray, int _vertexId, int _index)
{
	int iCount = _pMesh->GetElementBinormalCount();

	if (0 == iCount)
	{
		AssertMsg(L"GetElementBinormalCount가 여러개 입니다.");
	}

	FbxGeometryElementBinormal* pElement = _pMesh->GetElementBinormal();
	int iDataIndex = _vertexId;


	if (pElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			iDataIndex = pElement->GetIndexArray().GetAt(_vertexId);
		}
	}
	else if (pElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (FbxGeometryElement::eDirect == pElement->GetReferenceMode())
		{
			iDataIndex = _index;
		}
		else if (FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			iDataIndex = pElement->GetIndexArray().GetAt(_index);
		}
		else {
			AssertMsg(L"맵핑 방식중 처리할수 없는 방식입니다.");
		}
	}

	// 노말은 가져왔고.
	FbxVector4 BiNormal = pElement->GetDirectArray().GetAt(iDataIndex);

	_vertexArray[_index].BiNormal.x = (float)BiNormal.mData[0];
	_vertexArray[_index].BiNormal.y = (float)BiNormal.mData[2];
	_vertexArray[_index].BiNormal.z = (float)BiNormal.mData[1];
	_vertexArray[_index].BiNormal.Normalize();
}

void KFBX::LoadWeightAndIndex(fbxsdk::FbxCluster* _pCluster, int _BoneIndex, std::vector<KFBX3DVertex>& _vertexArray)
{
	int indexCount = _pCluster->GetControlPointIndicesCount();
	int clusterIndex = 0;

	for (int i = 0; i < indexCount; ++i)
	{
		KFBXIndexWeight indexWeight;

		indexWeight.Index = _BoneIndex;
		indexWeight.Weight = _pCluster->GetControlPointIndices()[i];

		clusterIndex = _pCluster->GetControlPointIndices()[i];
		m_IndexWeightContainer[m_MeshCount][clusterIndex].push_back(indexWeight);
	}
}

void KFBX::LoadOffsetMatrix(fbxsdk::FbxCluster* _pCluster, const fbxsdk::FbxAMatrix& _BaseMatrix, KFBXBoneData* _pBone)
{
	FbxAMatrix clusterMatrix;
	FbxAMatrix linkClusterMatrix;

	_pCluster->GetTransformMatrix(clusterMatrix);
	_pCluster->GetTransformLinkMatrix(linkClusterMatrix);

	FbxAMatrix Offset;
	Offset = linkClusterMatrix.Inverse() * clusterMatrix * _BaseMatrix;
	Offset = m_RootMatrix * Offset * m_RootMatrix;
	_pBone->Offset = FbxMatirxToKMatrix(Offset);
	_pBone->Bone = FbxMatirxToKMatrix(_BaseMatrix);
}

void KFBX::LoadTimeTransForm(fbxsdk::FbxNode* _pNode, fbxsdk::FbxCluster* _pCluster, const fbxsdk::FbxAMatrix& _BaseMatrix, KFBXBoneData* _pBone)
{
	size_t animationContainerSize = m_UserAnimationContainer.size();
	for (size_t animationIndex = 0; animationIndex < animationContainerSize; animationIndex++)
	{
		m_UserAnimationContainer[animationIndex].BoneFrameDataContainer[_pBone->Index].BoneIndex = _pBone->Index;

		fbxsdk::FbxLongLong start = m_UserAnimationContainer[animationIndex].StartTimeCount;
		fbxsdk::FbxLongLong end = m_UserAnimationContainer[animationIndex].EndTimeCount;
		for (fbxsdk::FbxLongLong timeCount = start; timeCount <= end; ++timeCount)
		{
			fbxsdk::FbxTime time = {};
			time.SetFrame(timeCount, m_UserAnimationContainer[animationIndex].TimeMode);

			fbxsdk::FbxAMatrix offsetMatrix = _pNode->EvaluateGlobalTransform(time) * _BaseMatrix;
			fbxsdk::FbxAMatrix currentMatrix = offsetMatrix.Inverse() * _pCluster->GetLink()->EvaluateGlobalTransform(time);

			currentMatrix = m_RootMatrix * currentMatrix * m_RootMatrix;

			m_UserAnimationContainer[animationIndex].BoneFrameDataContainer[_pBone->Index].BoneFrameInfomationContainer[timeCount].Time = time.GetSecondDouble();
			m_UserAnimationContainer[animationIndex].BoneFrameDataContainer[_pBone->Index].BoneFrameInfomationContainer[timeCount].FrameMatrix = FbxMatirxToKMatrix(currentMatrix);
			m_UserAnimationContainer[animationIndex].BoneFrameDataContainer[_pBone->Index].BoneFrameInfomationContainer[timeCount].Scale = FbxVectorToKVector(currentMatrix.GetS());
			m_UserAnimationContainer[animationIndex].BoneFrameDataContainer[_pBone->Index].BoneFrameInfomationContainer[timeCount].Quaternion = FbxQuaternionToKVector(currentMatrix.GetQ());
			m_UserAnimationContainer[animationIndex].BoneFrameDataContainer[_pBone->Index].BoneFrameInfomationContainer[timeCount].Translation = FbxVectorToKVector(currentMatrix.GetT());
		}
	}
}
