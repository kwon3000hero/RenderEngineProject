#include "KFrameAnimator.h"
#include "KCuttingRender.h"
#include <KGAMETIME.h>

void KFrameAnimator::Init()
{
	m_Render = Actor()->GetComponent<KCuttingRender>();

	if (nullptr == m_Render.get())
	{
		AssertMsg(L"커팅 랜더러가 없습니다.");
	}
}

void KFrameAnimator::CreateAni(const KGameString& _CuttingTex, const KGameString& _AniName, size_t _Start, size_t _End, float _Time, bool _Loop /*= true*/)
{
	if (nullptr == KCuttingTexture::Find(_CuttingTex).get())
	{
		AssertMsg(L"애니메이션을 만드는데 텍스처 세팅이 안됩니다.");
	}

	if (m_AnimationContainer.end() != m_AnimationContainer.find(_AniName))
	{
		AssertMsg(L"이미 그 이름의 애니메이션을 만들었습니다.");
	}

	FrameAni& Ani = m_AnimationContainer[_AniName];
	Ani.m_AnimationName = _AniName;
	Ani.m_CuttingTexture = _CuttingTex;
	Ani.m_Start = _Start;
	Ani.m_Loop = _Loop;
	Ani.m_End = _End;
	Ani.m_Time = _Time;
}

void KFrameAnimator::ChangeAnimation(const KGameString& _AniName)
{
	if (m_AnimationContainer.end() == m_AnimationContainer.find(_AniName))
	{
		AssertMsg(L"애니메이션이 없습니다.");
	}

	CurrentAnimation = &m_AnimationContainer[_AniName];
	CurrentAnimation->m_CurrentFrame = CurrentAnimation->m_Start;
	CurrentAnimation->m_CurrentTime = CurrentAnimation->m_Time;
	CurrentAnimation->m_IsEndAnimation = false;
}


void KFrameAnimator::Update()
{
	if (nullptr == CurrentAnimation)
	{
		AssertMsg(L"애니메이션 세팅이 안되어 있습니다.");
	}

	CurrentAnimation->m_CurrentTime -= KGAMETIME::DeltaTime();

	if (0 >= CurrentAnimation->m_CurrentTime)
	{
		++CurrentAnimation->m_CurrentFrame;

		if (CurrentAnimation->m_End < CurrentAnimation->m_CurrentFrame)
		{
			if (true == CurrentAnimation->m_Loop)
			{
				CurrentAnimation->m_CurrentFrame = CurrentAnimation->m_Start;
			}
			else
			{
				CurrentAnimation->m_IsEndAnimation = true;
				--CurrentAnimation->m_CurrentFrame;
			}
		}

		CurrentAnimation->m_CurrentTime = CurrentAnimation->m_Time;
	}



	m_Render->CuttingTex(CurrentAnimation->m_CuttingTexture, CurrentAnimation->m_CurrentFrame);
}