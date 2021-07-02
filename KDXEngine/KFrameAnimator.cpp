#include "KFrameAnimator.h"
#include "KCuttingRender.h"
#include <KGAMETIME.h>

void KFrameAnimator::Init()
{
	m_Render = Actor()->GetComponent<KCuttingRender>();

	if (nullptr == m_Render.get())
	{
		AssertMsg(L"Ŀ�� �������� �����ϴ�.");
	}
}

void KFrameAnimator::CreateAni(const KGameString& _CuttingTex, const KGameString& _AniName, size_t _Start, size_t _End, float _Time, bool _Loop /*= true*/)
{
	if (nullptr == KCuttingTexture::Find(_CuttingTex).get())
	{
		AssertMsg(L"�ִϸ��̼��� ����µ� �ؽ�ó ������ �ȵ˴ϴ�.");
	}

	if (m_AnimationContainer.end() != m_AnimationContainer.find(_AniName))
	{
		AssertMsg(L"�̹� �� �̸��� �ִϸ��̼��� ��������ϴ�.");
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
		AssertMsg(L"�ִϸ��̼��� �����ϴ�.");
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
		AssertMsg(L"�ִϸ��̼� ������ �ȵǾ� �ֽ��ϴ�.");
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