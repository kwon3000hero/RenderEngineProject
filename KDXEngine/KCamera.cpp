#include "KCamera.h"
#include <assert.h>
#include "KTransform.h"
#include <KGameWindow.h>
#include "KRenderTarget.h"
#include "KLight.h"

KPostEffect::KPostEffect() : m_IsUpdate(true)
{
	m_EffectTarget = make_KPTR<KRenderTarget>();
	m_EffectTarget->Create(
		KVector{ KGameWindow::MainWindow()->Size().x, KGameWindow::MainWindow()->Size().y },
		KVector::ZERO);
}

KPostEffect::~KPostEffect()
{
}

KCamera::KCamera() : m_OriFov(60.0F), m_CurFov(60.0F), m_CurSize(KGameWindow::MainWindow()->Size()), m_OriSize(KGameWindow::MainWindow()->Size()), m_Near(0.1f), m_Far(10000.0f), m_eMode(CAMMODE::PROJ)
{

}

KCamera::~KCamera()
{

}

void KCamera::Init()
{
	if (nullptr == Actor()->GetComponent<KTransform>().get())
	{
		assert(false);
	}

	if (nullptr != Actor()->GetComponent<KCamera>().get())
	{
		assert(false);
	}

	Scene()->PushCamera(KPTR<KCamera>(this));

	m_currentCameraTarget = make_KPTR<KRenderTarget>();
	m_currentCameraTarget->Create(
		KVector{ KGameWindow::MainWindow()->Size().x, KGameWindow::MainWindow()->Size().y },
		KVector::ZERO);

	m_forwardCameraTarget = make_KPTR<KRenderTarget>();
	m_forwardCameraTarget->Create(
		KVector{ KGameWindow::MainWindow()->Size().x, KGameWindow::MainWindow()->Size().y },
		KVector::ZERO);

	m_forwardCameraTarget->SetDepthStencil(KGameDeviceManager::MainGameDevice()->MainTarget->DepthStencil());

	m_defferdCameraTarget = make_KPTR<KRenderTarget>();
	m_defferdCameraTarget->Create(
		KVector{ KGameWindow::MainWindow()->Size().x, KGameWindow::MainWindow()->Size().y },
		KVector::ZERO);


	m_defferdCameraTarget->CreateDepth();

	m_globalBufferCameraTarget = make_KPTR<KRenderTarget>();
	//Diffuse0
	m_globalBufferCameraTarget->Create(
		KVector{ KGameWindow::MainWindow()->Size().x, KGameWindow::MainWindow()->Size().y },
		KVector::ZERO);
	//Position1
	m_globalBufferCameraTarget->Create(
		KVector{ KGameWindow::MainWindow()->Size().x, KGameWindow::MainWindow()->Size().y },
		KVector::ZERO);
	//Normal2
	m_globalBufferCameraTarget->Create(
		KVector{ KGameWindow::MainWindow()->Size().x, KGameWindow::MainWindow()->Size().y },
		KVector::ZERO);
	//Depth3
	m_globalBufferCameraTarget->Create(
		KVector{ KGameWindow::MainWindow()->Size().x, KGameWindow::MainWindow()->Size().y },
		KVector::ZERO);
	m_globalBufferCameraTarget->SetDepthStencil(KGameDeviceManager::MainGameDevice()->MainTarget->DepthStencil());

	m_lightCameraTarget = make_KPTR<KRenderTarget>();
	//diffuse
	m_lightCameraTarget->Create(
		KVector{ KGameWindow::MainWindow()->Size().x, KGameWindow::MainWindow()->Size().y },
		KVector::ZERO);
	//specular
	m_lightCameraTarget->Create(
		KVector{ KGameWindow::MainWindow()->Size().x, KGameWindow::MainWindow()->Size().y },
		KVector::ZERO);

	//ambient
	m_lightCameraTarget->Create(
		KVector{ KGameWindow::MainWindow()->Size().x, KGameWindow::MainWindow()->Size().y },
		KVector::ZERO);
	//ÇÕÄ£ °Í
	m_lightCameraTarget->Create(
		KVector{ KGameWindow::MainWindow()->Size().x, KGameWindow::MainWindow()->Size().y },
		KVector::ZERO);
	m_lightCameraTarget->CreateDepth();

	m_defferdLightPlayer = make_KPTR<KRenderer>(L"FULLRECT", L"DEFFERDCALCULATEDLIGHT");
	m_defferdLightPlayer->SetSampler(L"Smp", L"LCSMP");
	m_defferdLightPlayer->SetTexture(L"POSTEX", m_globalBufferCameraTarget->Texture(1));
	m_defferdLightPlayer->SetTexture(L"NORMALTEX", m_globalBufferCameraTarget->Texture(2));
	m_defferdLightPlayer->SetTexture(L"DEPTHTEX", m_globalBufferCameraTarget->Texture(3));

	m_defferdMergePlayer = make_KPTR<KRenderer>(L"FULLRECT", L"DEFFERDMERGE");
	m_defferdMergePlayer->SetSampler(L"Smp", L"PCSMP");
	m_defferdMergePlayer->SetTexture(L"ColorTex", m_globalBufferCameraTarget->Texture(0));
	m_defferdMergePlayer->SetTexture(L"LightTex", m_lightCameraTarget->Texture(3));


}

void KCamera::UpdateViewAndProjection()
{
	KWeakPTR<KTransform> transform(Actor()->GetComponent<KTransform>());
	m_ViewMat.View(transform->WorldPosition(), transform->WFORWARD(), transform->WUP());

	switch (m_eMode)
	{
	case CAMMODE::ORTH:
		m_ProjMat.Orth(m_CurSize.x, m_CurSize.y, m_Near, m_Far);
		break;
	case CAMMODE::PROJ:
		m_ProjMat.ProjDeg(m_CurFov, m_CurSize.x, m_CurSize.y, m_Near, m_Far);
		break;
	default:
		break;
	}
}

KPTR<KRenderTarget> KCamera::CurrentCameraTarget()
{
	return m_currentCameraTarget;
}

KPTR<KRenderTarget> KCamera::GlobalBufferTarget()
{
	return m_globalBufferCameraTarget;
}

KPTR<KRenderTarget> KCamera::DefferdCameraTarget()
{
	return m_defferdCameraTarget;
}

KPTR<KRenderTarget> KCamera::LightTarget()
{
	return m_lightCameraTarget;
}

void KCamera::SetForwardCameraTarget()
{
	m_forwardCameraTarget->Clear(false);
	m_forwardCameraTarget->Setting();
}

void KCamera::SetDeffedCameraTarget()
{
	m_defferdCameraTarget->Clear(false);
	m_defferdCameraTarget->Setting();
}

void KCamera::SetGlobalCameraTarget()
{
	m_globalBufferCameraTarget->Clear(false);
	m_globalBufferCameraTarget->Setting();
}

void KCamera::LightPrevEffect()
{
	for (auto& effect : m_objectList)
	{
		if (false == effect->IsON())
		{
			continue;
		}

		effect->LightPrevEffect();
	}
}

void KCamera::LightNextEffect()
{
	for (auto& effect : m_objectList)
	{
		if (false == effect->IsON())
		{
			continue;
		}
		effect->LightNextEffect();
	}
}

void KCamera::MergePrevEffect()
{
	for (auto& effect : m_objectList)
	{
		if (false == effect->IsON())
		{
			continue;
		}

		effect->MergePrevEffect();
	}
}

void KCamera::MergeNextEffect()
{
	for (auto& effect : m_objectList)
	{
		if (false == effect->IsON())
		{
			continue;
		}

		effect->MergeNextEffect();
	}
}

void KCamera::MergeDefferd()
{
	m_defferdCameraTarget->Effect(m_defferdMergePlayer);
}

void KCamera::CalculateDefferdLight(LightData* _pLightData, KPTR<KRenderTarget> _shadowRenderTargets)
{
	m_defferdLightPlayer->SetConstantBuffer(L"CURRENTLIGHTBUFFER", (void*)_pLightData, ConstantBufferMode::Link);
	m_defferdLightPlayer->SetTexture(L"SHADOWTEX", _shadowRenderTargets->Texture(0));

	m_lightCameraTarget->Effect(m_defferdLightPlayer, false, false);
}

void KCamera::MergeTargets()
{
	m_currentCameraTarget->Clear();
	m_currentCameraTarget->AlwaysMerge(m_forwardCameraTarget);
	m_currentCameraTarget->AlwaysMerge(m_defferdCameraTarget);

}