#include "KRendererManager.h"

KRenderManager::KRenderManager(const KGameString& _MeshName, const KGameString& _RenderPipeLineName, int _Order/* = 0*/)
{
	SetOrder(_Order);
	CreateRenderPlayer(_MeshName, _RenderPipeLineName);
}

KRenderManager::KRenderManager(const KGAMEDIR& _dir, const KGameString& _fbxName, const KGameString& _MaterialName, const KGameString& _SamplerName, int _order/*=0*/)
{
	SetOrder(_order);
	CreateRenderPlayerToFbx(_dir, _fbxName, _MaterialName, _SamplerName);
}

KRenderManager::KRenderManager(int _Order)
{
	SetOrder(_Order);
}

KRenderManager::KRenderManager()
{
}

KRenderManager::~KRenderManager()
{

}

void KRenderManager::Init()
{
	KPTR<KTransform> PTR = Actor()->GetComponent<KTransform>();

	if (nullptr == PTR.get())
	{
		assert(false);
	}

	Parent(PTR);
	Scene()->PushRenderManager(KPTR<KRenderManager>(this));
}

void KRenderManager::Render(KPTR<KCamera> _ViewCam)
{
	SetCameraMatrix(_ViewCam);

	for (size_t i = 0; i < m_RenderPlayerContainer.size(); i++)
	{
		if (false == m_RenderPlayerContainer[i]->IsUpdate())
		{
			continue;
		}

		m_RenderPlayerContainer[i]->Render();
	}
}

void KRenderManager::ForwardRender(KPTR<KCamera> _ViewCam)
{
	if (0 >= m_ForwardRenderPlayerContainer.size())
	{
		return;
	}

	SetCameraMatrix(_ViewCam);

	for (size_t i = 0; i < m_ForwardRenderPlayerContainer.size(); i++)
	{
		if (false == m_ForwardRenderPlayerContainer[i]->IsUpdate())
		{
			continue;
		}

		m_ForwardRenderPlayerContainer[i]->Render();
	}
}

void KRenderManager::DefferdRender(KPTR<KCamera> _ViewCam)
{
	if (0 >= m_DefferdRenderPlayerContainer.size())
	{
		return;
	}

	SetCameraMatrix(_ViewCam);

	for (size_t i = 0; i < m_DefferdRenderPlayerContainer.size(); i++)
	{
		if (false == m_DefferdRenderPlayerContainer[i]->IsUpdate())
		{
			continue;
		}

		m_DefferdRenderPlayerContainer[i]->Render();
	}
}

void KRenderManager::ShadowRender(KPTR<KLightManager> _light)
{
	if (0 >= m_ShadowRenderPlayerContainer.size())
	{
		return;
	}

	SetLightMatrix(_light);

	for (size_t i = 0; i < m_ShadowRenderPlayerContainer.size(); i++)
	{
		if (false == m_ShadowRenderPlayerContainer[i]->IsUpdate())
		{
			continue;
		}
		m_ShadowRenderPlayerContainer[i]->ShadowRender();
	}
}

bool KRenderManager::IsBump()
{
	for (int i = 0; i < (int)m_RenderPlayerContainer.size(); i++)
	{
		if (1 != m_RenderPlayerContainer[i]->m_RenderOption[static_cast<int>(RenderOption::Normal)])
		{
			return false;
		}
	}

	return true;
}

void KRenderManager::BumpOn()
{
	for (int i = 0; i < (int)m_RenderPlayerContainer.size(); i++)
	{
		RenderPlayerBumpOn(i);
	}
}

void KRenderManager::RenderPlayerBumpOn(int _index)
{
	m_RenderPlayerContainer[_index]->m_RenderOption[static_cast<int>(RenderOption::Normal)] = true;
}

void KRenderManager::BumpOff()
{
	for (int i = 0; i < (int)m_RenderPlayerContainer.size(); i++)
	{
		RenderPlayerBumpOff(i);
	}
}

void KRenderManager::RenderPlayerBumpOff(int _index)
{
	m_RenderPlayerContainer[_index]->m_RenderOption[static_cast<int>(RenderOption::Normal)] = false;
}

void KRenderManager::ShadowOn()
{
	for (int i = 0; i < (int)m_RenderPlayerContainer.size(); i++)
	{
		RenderPlayerShadowOn(i);
	}
}

void KRenderManager::RenderPlayerShadowOn(int _index)
{
	m_RenderPlayerContainer[_index]->ShadowOn();
}



void KRenderManager::SetTexture(const KGameString& _NAME, const KGameString& _RESNAME, int _PlayerIndex /*= 0*/)
{
	if (_PlayerIndex >= m_RenderPlayerContainer.size())
	{
		AssertMsg(L"존재하지 않는 렌더 플레이어를 처리하려고 했습니다.");
	}

	if (false == m_RenderPlayerContainer[_PlayerIndex]->IsExistTexture(_NAME))
	{
		AssertMsg(L"쉐이더에 세팅할 텍스처중 " + _NAME + "은 존재하지 않습니다.");
	}

	m_RenderPlayerContainer[_PlayerIndex]->SetTexture(_NAME, _RESNAME);
}
void KRenderManager::SetSampler(const KGameString& _NAME, const KGameString& _RESNAME, int _PlayerIndex /*= 0*/)
{
	if (_PlayerIndex >= m_RenderPlayerContainer.size())
	{
		AssertMsg(L"존재하지 않는 렌더 플레이어를 처리하려고 했습니다.");
	}

	if (false == m_RenderPlayerContainer[_PlayerIndex]->IsExistSampler(_NAME))
	{
		AssertMsg(L"쉐이더에 세팅할 텍스처중 " + _NAME + "은 존재하지 않습니다.");
	}

	m_RenderPlayerContainer[_PlayerIndex]->SetSampler(_NAME, _RESNAME);
}

void KRenderManager::SetConstantBuffer(const KGameString& _NAME, void* Data, ConstantBufferMode _Mode /*= CBMODE::CM_LINK*/, int _PlayerIndex /*= 0*/)
{
	if (_PlayerIndex >= m_RenderPlayerContainer.size())
	{
		AssertMsg(L"존재하지 않는 렌더 플레이어를 처리하려고 했습니다.");
	}

	m_RenderPlayerContainer[_PlayerIndex]->SetConstantBuffer(_NAME, Data, _Mode);
}