#include "KBloomEffect.h"
#include <KGameInput.h>
#include "KRenderPipeline.h"

void KBloomEffect::Init()
{
	KVector BloomSize = { KGameWindow::MainWindow()->Size().x, KGameWindow::MainWindow()->Size().y };

	m_SmallTarget = new KRenderTarget();
	m_SmallTarget->Create(
		BloomSize,
		KVector::ZERO);

	m_BloomEffectTarget1 = new KRenderTarget();
	m_BloomEffectTarget1->Create(
		BloomSize,
		KVector::ZERO);

	m_BloomEffectTarget2 = new KRenderTarget();
	m_BloomEffectTarget2->Create(
		BloomSize,
		KVector::ZERO);

	m_CheckPlayer = new KRenderPlayer(L"FULLRECT", L"EMISSIONBLOOMCHECK");
	m_CheckPlayer->SetTexture(L"LightTex", CameraPtr()->LightTarget()->Texture(3));
	m_CheckPlayer->SetTexture(L"DepthTex", CameraPtr()->GlobalBufferTarget()->Texture(3));
	m_CheckPlayer->SetSampler(L"Smp", L"LWSMP");

	m_BloomPlayer = new KRenderPlayer(L"FULLRECT", L"EMISSIONBLOOM");
	m_BloomPlayer->SetTexture(L"CheckTex", m_SmallTarget->Texture(0));
	m_BloomPlayer->SetSampler(L"Smp", L"LWSMP");
	m_BloomPlayer->SetConstantBuffer(L"ScreenSize", &BloomSize, CBMode::CM_NEW);

	m_LightBloomPlayer = new KRenderPlayer(L"FULLRECT", L"LIGHTBLOOM");
	m_LightBloomPlayer->SetTexture(L"BloomTarget", m_BloomEffectTarget2->Texture(0));
	m_LightBloomPlayer->SetSampler(L"Smp", L"LWSMP");


}

void KBloomEffect::MergeNextEffect()
{
	m_SmallTarget->Effect(m_CheckPlayer);
	m_BloomPlayer->SetTexture(L"CheckTex", m_SmallTarget->Texture(0));
	m_BloomEffectTarget1->Effect(m_BloomPlayer);
	m_BloomPlayer->SetTexture(L"CheckTex", m_BloomEffectTarget1->Texture(0));
	m_BloomEffectTarget2->Effect(m_BloomPlayer);
	KRenderPipeline::Reset();
	CameraPtr()->LightTarget()->Effect(0, m_LightBloomPlayer, false, false);

	//if (true == KGAMEINPUT::IsPress("TESTFULLTARGET"))
	{
		// HGAMEDEBUG3D::DebugTexture(m_BloomEffectTarget->TEX(0), { 0, 0 }, HVECTOR{ 1280, 720 });
	//	// HGAMEINPUT::CreateKey("TESTFULLTARGET", 'K');
	}
}
