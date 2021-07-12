#include "KMeshAnimator.h"
#include "KFBX.h"
#include <KGAMETIME.h>
#include "KGameDebug3D.h"
#include "KTexture.h"

void KMeshAnimator::KMeshAnimation::RenderPlayerOff()
{
	for (auto& renderPlayer : m_RenderPlayerContainer)
	{
		renderPlayer->Off();
	}
}

void KMeshAnimator::KMeshAnimation::Reset()
{
	m_CurrentTime = m_StartTime;

	for (auto& renderPlayer : m_RenderPlayerContainer)
	{
		renderPlayer->On();
	}
}

void KMeshAnimator::KMeshAnimation::Update()
{
	m_CurrentTime += KGAMETIME::DeltaTime();

	if (m_EndTime <= m_CurrentTime)
	{
		if (false == m_Loop)
		{
			m_CurrentTime = m_EndTime;
		}
		else
		{
			m_CurrentTime = m_StartTime;
		}
	}

	float calcultedTime = ((float)m_pCurrentAnimationData->StartTime.GetSecondDouble()) + m_CurrentTime;

	m_CurrentFrame = (int)(calcultedTime * m_pCurrentAnimationData->FbxModeCount);

	m_NextFrame = m_CurrentFrame;
	m_NextFrame += 1;

	if (m_CurrentFrame >= m_pCurrentAnimationData->EndTimeCount - 1)
	{
		if (true == m_Loop)
		{
			m_CurrentFrame = m_Start;
			m_CurrentTime = 0.0f;
		}
		else
		{
			m_CurrentFrame = m_CurrentFrame - 1;
			m_NextFrame = m_CurrentFrame;
		}
	}

}

void KMeshAnimator::KMeshAnimation::PrevUpdate()
{
	KGameDebug3D::DebugText({ 0, 0 }, 20.0f, KVector::RED, L"FrameCount : %d", m_pCurrentAnimationData->FrameCount);
	KGameDebug3D::DebugText({ 0, 20 }, 20.0f, KVector::RED, L"CurFrame : %d", m_CurrentFrame);
	KGameDebug3D::DebugText({ 0, 40 }, 20.0f, KVector::RED, L"NextFrame : %d", m_NextFrame);
	KGameDebug3D::DebugText({ 0, 60 }, 20.0f, KVector::RED, L"AniStart : %d", m_pCurrentAnimationData->StartTimeCount);
	KGameDebug3D::DebugText({ 0, 80 }, 20.0f, KVector::RED, L"AniEnd : %d", m_pCurrentAnimationData->EndTimeCount);
	KGameDebug3D::DebugText({ 0, 100 }, 20.0f, KVector::RED, L"AniStart Time : %f", (float)m_pCurrentAnimationData->StartTime.GetSecondDouble());
	KGameDebug3D::DebugText({ 0, 120 }, 20.0f, KVector::RED, L"AniEnd Time : %f", (float)m_pCurrentAnimationData->EndTime.GetSecondDouble());
	


	size_t boneDataSize = m_pFbx->m_UserBoneContainer.size();
	for (size_t i = 0; i < boneDataSize; ++i)
	{
		if (true == m_pCurrentAnimationData->BoneFrameDataContainer[i].BoneFrameInfomationContainer.empty())
		{
			m_FrameMatrixData[i] = m_pFbx->m_UserBoneContainer[i].Bone;
			continue;
		}

		BoneFrameInfomation& currentFrameInformation = m_pCurrentAnimationData->BoneFrameDataContainer[i].BoneFrameInfomationContainer[m_CurrentFrame];
		BoneFrameInfomation& nextFrameInformation = m_pCurrentAnimationData->BoneFrameDataContainer[i].BoneFrameInfomationContainer[m_NextFrame];

		float nextFrameTime = (float)nextFrameInformation.Time;

		float percent = (float)(m_CurrentTime - nextFrameTime) * (float)m_pCurrentAnimationData->FbxModeCount;

		KVector S = KVector::Lerp(currentFrameInformation.Scale, nextFrameInformation.Scale, percent);
		KVector Q = KVector::QSlerp(currentFrameInformation.Quaternion, nextFrameInformation.Quaternion, percent);
		KVector T = KVector::Lerp(currentFrameInformation.Translation, nextFrameInformation.Translation, percent);

		m_FrameBoneData[i].AnimationScale = S;
		m_FrameBoneData[i].AnimationQuaternion = Q;
		m_FrameBoneData[i].AnimationPosition = T;

		m_FrameMatrixData[i] = KMatrix::Affine(S, Q, T);
		m_FrameBoneData[i].AnimationMatrix = m_FrameMatrixData[i];
		m_FrameMatrixData[i] = m_pFbx->m_UserBoneContainer[i].Offset * m_FrameMatrixData[i];

	}

	m_BoneDataTexture->SetPixel(&m_FrameMatrixData[0], (int)(sizeof(KMatrix) * m_FrameMatrixData.size()));
}

void KMeshAnimator::SetCurrentFrame(int _frame)
{
	if (nullptr != m_CurrentAnimation.get())
	{
		m_CurrentAnimation->m_CurrentFrame = _frame;
		m_CurrentAnimation->m_NextFrame = _frame + 1;

		m_CurrentAnimation->m_CurrentTime = m_CurrentAnimation->m_pCurrentAnimationData->FrameTime(m_CurrentAnimation->m_CurrentFrame);
	}
}

void KMeshAnimator::ChangeAnimation(const KGameString& _ani)
{
	if (nullptr != m_CurrentAnimation.get())
	{
		m_CurrentAnimation->RenderPlayerOff();
	}

	m_CurrentAnimation = FindAnimation(_ani);

	if (nullptr == m_CurrentAnimation.get())
	{
		AssertMsg(_ani + L" 존재하지 않는 애니메이션으로 체인지 하려고 했습니다.");
	}

	m_CurrentAnimation->Reset();
}

std::vector<KPTR<KRenderer>> KMeshAnimator::CreateAnimation(const KGAMEDIR& _dir, const KGameString& _fbxName, const KGameString& _takeName, const KGameString& _aniName, int _start, int _end, const KGameString& _materialName, float _speed, bool _loop)
{
	KPTR<KFBX> fbx = KGameResourceManager<KFBX>::Instance().Find(_fbxName);

	if (nullptr == fbx.get())
	{
		AssertMsg(L"존재하지 않는 fbx에서 애니메이션을 만들려고 했습니다.");
	}

	KFBXAnimationData* pAnimationData = fbx->FindAnimationData(_takeName);

	if (nullptr == pAnimationData)
	{
		AssertMsg(_takeName + L" 존재하지 않는 애니메이션 에서 애니메이션을 만들려고 했습니다.");
	}

	if (nullptr != FindAnimation(_aniName).get())
	{
		AssertMsg(_takeName + L", " + _aniName + L" 이미 존재하는 애니메이션을 또 만들려고 했습니다.");
	}

	KPTR<KMeshAnimation> newAnimation = new KMeshAnimation();

	newAnimation->m_pFbx = fbx;
	newAnimation->m_pCurrentAnimationData = pAnimationData;
	newAnimation->m_animationName = _aniName;
	newAnimation->m_Loop = _loop;
	newAnimation->m_Speed = _speed;
	newAnimation->m_Start = _start;
	newAnimation->m_End = _end;

	newAnimation->m_StartTime = newAnimation->m_Start / (float)pAnimationData->FrameModeCount();
	newAnimation->m_EndTime = newAnimation->m_End / (float)pAnimationData->FrameModeCount();
	newAnimation->m_FrameBoneData.resize(fbx->m_UserBoneContainer.size());
	newAnimation->m_FrameMatrixData.resize(fbx->m_UserBoneContainer.size());

	std::map<KGameString, std::vector<KPTR<KRenderer>>>::iterator FindRenderPlayer = m_RenderPlayerGroupContainer.find(_fbxName);
	if (FindRenderPlayer == m_RenderPlayerGroupContainer.end())
	{
		m_AnimationBoneDataTextureContainer[_fbxName] = new KTexture();
		m_AnimationBoneDataTextureContainer[_fbxName]->Create({ (float)(fbx->m_UserBoneContainer.size() * 4), 1 }, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DYNAMIC);

		m_RenderPlayerGroupContainer[_fbxName] = m_RenderManager->CreateRenderPlayerToFbx(_dir, _fbxName, _materialName, L"LWSMP");

		for (auto& renderPlayer : m_RenderPlayerGroupContainer[_fbxName])
		{
			renderPlayer->m_RenderOption[static_cast<int>(RenderOption::Animation)] = true;
			renderPlayer->SetTexture(L"FrameAnimationTexture", m_AnimationBoneDataTextureContainer[_fbxName]);
		}

		FindRenderPlayer = m_RenderPlayerGroupContainer.find(_fbxName);
	}

	newAnimation->m_BoneDataTexture = m_AnimationBoneDataTextureContainer[_fbxName];
	newAnimation->m_pMeshAnimation = this;
	newAnimation->m_RenderPlayerContainer = FindRenderPlayer->second;

	m_AnimationContainer[_aniName] = newAnimation;

	return m_RenderPlayerGroupContainer[_fbxName];
}


KMeshAnimator::BoneFrameData KMeshAnimator::BoneFrameDataMatrix(const KGameString& _boneName)
{
	BoneFrameData Data;

	auto& Test = m_CurrentAnimation->m_pFbx->m_UserBoneContainer;

	KFBXBoneData* Ptr = m_CurrentAnimation->m_pFbx->m_UserBoneMapContainer.find(_boneName)->second;

	m_CurrentAnimation->m_FrameBoneData[Ptr->Index].Pos = m_RenderManager->MatrixData().Final.Multi(m_CurrentAnimation->m_FrameBoneData[Ptr->Index].AnimationPosition, 1.0f);
	m_CurrentAnimation->m_FrameBoneData[Ptr->Index].Scale = m_RenderManager->MatrixData().Final.Multi(m_CurrentAnimation->m_FrameBoneData[Ptr->Index].AnimationScale, 0.0f);

	KVector rot = m_RenderManager->WorldRotation();
	KVector quaternian = m_RenderManager->WorldQuaternion();

	m_CurrentAnimation->m_FrameBoneData[Ptr->Index].Quaternion = m_RenderManager->WorldQuaternion().MultiQuaternion(m_CurrentAnimation->m_FrameBoneData[Ptr->Index].AnimationQuaternion);

	m_CurrentAnimation->m_FrameBoneData[Ptr->Index].AnimationEuler = m_CurrentAnimation->m_FrameBoneData[Ptr->Index].AnimationQuaternion.QuaternionToEulerRad();
	m_CurrentAnimation->m_FrameBoneData[Ptr->Index].Euler = m_CurrentAnimation->m_FrameBoneData[Ptr->Index].Quaternion.QuaternionToEulerRad();
	m_CurrentAnimation->m_FrameBoneData[Ptr->Index].AnimationMatrix = m_CurrentAnimation->m_FrameBoneData[Ptr->Index].AnimationMatrix;
	m_CurrentAnimation->m_FrameBoneData[Ptr->Index].AnimationWorldMatrix = m_CurrentAnimation->m_FrameBoneData[Ptr->Index].AnimationWorldMatrix * m_RenderManager->MatrixData().Final;

	return m_CurrentAnimation->m_FrameBoneData[Ptr->Index];
}

void KMeshAnimator::Init()
{
	m_RenderManager = Actor()->GetComponent<KRenderManager>();

	if (nullptr == m_RenderManager.get())
	{
		AssertMsg(L"랜더러가 없으면 3D애니메이션을 만들수 없습니다.");
	}
}

void KMeshAnimator::Update()
{
	if (nullptr == m_CurrentAnimation.get())
	{
		return;
	}

	if (false == m_isPlay)
	{
		return;
	}

	m_CurrentAnimation->Update();
}

void KMeshAnimator::PrevUpdate()
{
	if (nullptr == m_CurrentAnimation.get())
	{
		AssertMsg(L"애니메이션이 지정되지 않았습니다.");
	}

	m_CurrentAnimation->PrevUpdate();
}

KMeshAnimator::KMeshAnimator() : m_isPlay(true)
{
}

KMeshAnimator::~KMeshAnimator()
{
}
