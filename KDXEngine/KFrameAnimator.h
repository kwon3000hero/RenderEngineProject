#pragma once
#include "KGameComponentManager.h"
#include <map>

class KFBX;
struct KFBXAnimationData;
class KCuttingRender;
class KFrameAnimator : public KActorGameComponent
{
private:
	class FrameAni
	{
	public:
		KGameString m_CuttingTexture;
		KGameString m_AnimationName;
		size_t m_Start;
		size_t m_End;
		float m_Time;
		bool m_Loop;
		bool m_IsEndAnimation;

		size_t m_CurrentFrame;
		float m_CurrentTime;
	};

	std::map<KGameString, FrameAni> m_AnimationContainer;
	FrameAni* CurrentAnimation;

public:
	KGameString CurAniName()
	{
		return CurrentAnimation->m_AnimationName;
	}

private:
	KPTR<KCuttingRender> m_Render;

public:
	void CreateAni(const KGameString& _CuttingTex, const KGameString& _AniName, size_t _Start, size_t _End, float _Time, bool _Loop = true);
	void ChangeAnimation(const KGameString& _AniName);

public:
	void Init() override;
	void Update() override;
};

