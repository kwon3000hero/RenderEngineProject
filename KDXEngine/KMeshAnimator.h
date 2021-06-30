#pragma once
#include "KRender.h"
#include <map>
#include <KGAMEDIR.h>

class KFBX;
struct KFBXAnimationData;
class KMeshAnimator : public KActorGameComponent
{
public:
	struct BoneFrameData
	{
		KMatrix AnimationMatrix;
		KMatrix AnimationWorldMatrix;
		KVector AnimationScale;
		KVector AnimationPosition;
		KVector AnimationEuler;
		KVector AnimationQuaternion;

		KVector Scale;
		KVector Pos;
		KVector Euler;
		KVector Quaternion;
	};

private:	
	class KMeshAnimation : public KGameReference
	{
	public:
		KMeshAnimator* m_pMeshAnimation;

		KFBX* m_pFbx;
		KGameString m_animationName;
		KFBXAnimationData* m_pCurrentAnimationData;

		int m_Start;
		int m_End;

		float m_StartTime;
		float m_EndTime;

		float m_Speed;
		float m_CurrentTime;
		bool m_Loop;

		int m_CurrentFrame;
		int m_NextFrame;

		std::vector<BoneFrameData> m_FrameBoneData;
		std::vector<KMatrix> m_FrameMatrixData;
		KPTR<KTexture> m_BoneDataTexture;

		std::vector<KPTR<KRenderPlayer>> m_RenderPlayerContainer;

	public:
		void RenderPlayerOff();
		void Reset();
		void Update();
		void PrevUpdate();
	};

	friend KMeshAnimation;

private:
	KPTR<KRenderManager> m_RenderManager;

	KPTR<KMeshAnimation> m_CurrentAnimation;

	bool m_isPlay;


	std::map<KGameString, KPTR<KTexture>> m_AnimationBoneDataTextureContainer;
	std::map<KGameString, std::vector<KPTR<KRenderPlayer>>> m_RenderPlayerGroupContainer;
	std::map<KGameString, KPTR<KMeshAnimation>> m_AnimationContainer;

	KPTR<KMeshAnimation> FindAnimation(const KGameString& _AniName)
	{
		if (m_AnimationContainer.end() == m_AnimationContainer.find(_AniName))
		{
			return nullptr;
		}

		return m_AnimationContainer[_AniName];
	}

public:
	bool m_FrameSetCheck;
	void SetCurrentFrame(int _frame);

	bool IsPlay()
	{
		return m_isPlay;
	}

	void SetPlay()
	{
		m_isPlay = true;
	}

	void SetStop()
	{
		m_isPlay = false;
	}

	KPTR<KMeshAnimation> CurrentAnimation()
	{
		return m_CurrentAnimation;
	}

	void ChangeAnimation(const KGameString& _ani);
	std::vector<KPTR<KRenderPlayer>> CreateAnimation(const KGAMEDIR& _dir, const KGameString& _fbxName, const KGameString& _takeName,
		const KGameString& _aniName, int _start, int _end,
		const KGameString& _materialName = L"3DMESHFORWARD", float _speed = 1.0f, bool _loop = true);

public:
	BoneFrameData BoneFrameDataMatrix(const KGameString& _boneName);


public:
	void Init() override;
	void Update() override;
	void PrevUpdate() override;

public:
	KMeshAnimator();
	virtual ~KMeshAnimator();
};
