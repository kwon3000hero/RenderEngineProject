#include "KLight.h"

KLightManager::KLightManager()
{
	m_LightData.LightPower.x = 0.7f;
	m_LightData.LightPower.y = 1.0f;
	m_LightData.LightPower.z = 1.0f;
	m_LightData.LightPower.w = 1.0f;
	m_LightData.LightRange.y = 80.0f;
	m_LightData.AmbientColor = KVector(0.0f, 0.0f, 0.0f);

	m_LightData.ShadowOption.x = 1.0f;
}

KLightManager::~KLightManager()
{
}

KMatrix KLightManager::View()
{
	return m_LightData.LightView;
}

KMatrix KLightManager::Projection()
{
	return m_LightData.LightProj;
}

void KLightManager::SetShadowTarget()
{
	m_ShadowTarget->Clear();
	m_ShadowTarget->Setting();
}

void KLightManager::Init()
{
	m_ShadowTargetSize = { 4096.0F, 4096.0F };

	if (nullptr == Actor()->GetComponent<KTransform>().get())
	{
		assert(false);
	}

	m_ShadowTarget = make_KPTR<KRenderTarget>();
	m_ShadowTarget->Create(m_ShadowTargetSize, { 0,0,0,0 }, DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	m_ShadowTarget->CreateDepth();

	Scene()->PushLightManager(KPTR<KLightManager>(this));
}

void KLightManager::PrevRender()
{
	KWeakPTR<KTransform> transform(Actor()->GetComponent<KTransform>());

	m_LightData.LightPos = transform->WorldPosition();
	m_LightData.LightDir = transform->WFORWARD();
	m_LightData.LightDirInv = transform->WBACK();

	m_LightData.LightView.View(
		transform->WorldPosition(),
		transform->WFORWARD(),
		transform->WUP());

	float Near = 0.1f;
	float Far = 10000.0f;
	float Width = m_ShadowTargetSize.x;
	float Height = m_ShadowTargetSize.y;

	float halfWidth = Width * 0.5f;
	float halfHeight = Height * 0.5f;

	m_LightData.LightProj.Orth(Width, Height, Near, Far);
	m_LightData.LightVP = m_LightData.LightView * m_LightData.LightProj;
}
